using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading;

using Google.Protobuf;
using Google.Protobuf.Protocol;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    public class EntityManager : Singleton<EntityManager>
    {
        private readonly Dictionary<ulong, Entity> _entities = new();
        private readonly Dictionary<ulong, Character> _characters = new();

        private ulong _nextGameId = 1; // GameId 발급을 위한 필드
        private readonly object _lock = new();

        private ulong GenerateGameId()
        {
            lock (_lock)
            {
                return _nextGameId++;
            }
        }

        public T Spawn<T>(ulong dataSheetId = 0) where T : Entity
        {
            T entity = Activator.CreateInstance(typeof(T), nonPublic: true) as T;

            if (entity == null)
            {
                throw new InvalidOperationException("Failed to create instance.");
            }

            lock (_lock)
            {
                var gameId = GenerateGameId();
                entity.GameId = gameId;
                entity.Init(dataSheetId);

                _entities[gameId] = entity;

                // TODO: 이 부분을 나중에 ID에 비트 연산을 해서 타입 정보를 넣는 것이 좋을지.. is 연산 45ms, int + 비트 연산 15ms 정도임.
                if (entity is Character character)
                {
                    _characters[gameId] = character;
                }

                return entity;
            }
        }

        public bool Remove(ulong gameId)
        {
            lock (_lock)
            {
                if (_entities.Remove(gameId, out var entity))
                {
                    if (entity is Character)
                    {
                        _characters.Remove(gameId);
                    }
                    return true;
                }
                return false;
            }
        }

        public Entity Find(ulong gameId)
        {
            lock (_lock)
            {
                if (_entities.TryGetValue(gameId, out var entity))
                {
                    return entity;
                }
                throw new KeyNotFoundException($"Entity with GameId {gameId} not found.");
            }
        }

        public Character GetCharacter(ulong gameId)
        {
            lock (_lock)
            {
                if (_characters.TryGetValue(gameId, out var character))
                {
                    return character;
                }
                throw new KeyNotFoundException($"Character with GameId {gameId} not found.");
            }
        }

        public IEnumerable<Character> GetAllCharacters()
        {
            lock (_lock)
            {
                return _characters.Values.ToList();
            }
        }

        public IEnumerable<Entity> GetAllEntities()
        {
            lock (_lock)
            {
                return _entities.Values.ToList();
            }
        }
    }
}
