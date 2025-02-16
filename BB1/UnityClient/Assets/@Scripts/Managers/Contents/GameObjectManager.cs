using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;
using Google.Protobuf.Protocol;

public class GameObjectManager
{
    #region Roots
    public Transform GetRootTransform(string name)
    {
        GameObject root = GameObject.Find(name);
        if (root == null)
            root = new GameObject { name = name };

        return root.transform;
    }

    public Transform CharacterRoot { get { return GetRootTransform("@Characters"); } }
    public Transform MonsterRoot { get { return GetRootTransform("@Monsters"); } }
    public Transform ProjectileRoot { get { return GetRootTransform("@Projectiles"); } }
    public Transform EnvRoot { get { return GetRootTransform("@Envs"); } }
    public Transform EffectRoot { get { return GetRootTransform("@Effects"); } }
    public Transform NpcRoot { get { return GetRootTransform("@Npc"); } }
    public Transform ItemHolderRoot { get { return GetRootTransform("@ItemHolders"); } }
    #endregion

    public GameObjectManager()
    {
    }

    public MyCharacter MyCharacter { get; set; }

    private Dictionary<ulong, GameObject> _objects = new Dictionary<ulong, GameObject>();

    public GameObject FindByGameId(ulong id)
    {
        GameObject go = null;
        _objects.TryGetValue(id, out go);
        return go;
    }

    public void Clear()
    {
        foreach (GameObject obj in _objects.Values)
            Managers.Resource.Destroy(obj);

        _objects.Clear();
        MyCharacter = null;
    }

    public MyCharacter SpawnMyCharacter(Protocol.EntityInfo entityInfo)
    {
        // 여기서 CharacterInfo 맞는지 체크
        var charInfo = entityInfo?.ActorInfo.CharacterInfo;

        if (charInfo == null)
        {
            Debug.LogError("Invalid entityInfo for MyCharacter");
            return null;
        }

        // TODO: 추후 데이터 Sheet 기반으로 Instanitate 하는 것 추가 필요
        GameObject go = Managers.Resource.Instantiate("KnightPrefab");
        go.name = charInfo.Name;
        go.transform.parent = CharacterRoot;

        MyCharacter = Utils.GetOrAddComponent<MyCharacter>(go);
        MyCharacter.EntityInfo = entityInfo;

        _objects.Add(entityInfo.GameId, go);

        return MyCharacter;
    }

    public Entity Spawn(Protocol.EntityInfo entityInfo)
    {
        if (entityInfo == null)
        {
            Debug.LogError("Invalid entityInfo");
            return null;
        }

        // Character인지 Monster인지 판단
        Entity entity = null;
        GameObject go = null;

        // TODO: 프리팹은 데이터로 다른 걸로 세팅하는 로직 추가 필요
        if (entityInfo.ActorInfo.CharacterInfo != null)
        {
            go = Managers.Resource.Instantiate("KnightPrefab"); // 캐릭터 프리팹
            go.transform.parent = CharacterRoot;
            go.name = entityInfo.GameId.ToString();
            entity = Utils.GetOrAddComponent<Character>(go);

        }
        else if (entityInfo.ActorInfo.MonsterInfo != null)
        {
            go = Managers.Resource.Instantiate("KnightPrefab"); // 몬스터 프리팹 추후 세팅 필요
            go.transform.parent = MonsterRoot;
            go.name = entityInfo.GameId.ToString();
            entity = Utils.GetOrAddComponent<Monster>(go);

        }
        else
        {
            Debug.LogError("Unknown entity type");
            return null;
        }

        _objects.Add(entityInfo.GameId, go);

        entity.EntityInfo = entityInfo;

        return entity;
    }

    public void Despawn(List<ulong> gameIds)
    {
        foreach (ulong gameId in gameIds)
        {
            if (_objects.TryGetValue(gameId, out GameObject go))
            {
                Managers.Resource.Destroy(go);

                // TODO: 여기서 MyCharacter 지웠을 때에 필요한 로직 생긴다면 추가하자.
                _objects.Remove(gameId);
            }
        }
    }
}