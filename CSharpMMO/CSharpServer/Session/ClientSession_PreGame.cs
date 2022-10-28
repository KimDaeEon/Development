using CSharpServer.Game;
using CSharpServer.Data;
using CSharpServer.DB;
using CSharpServer.Utils;
using Google.Protobuf.Protocol;
using Microsoft.EntityFrameworkCore;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer
{
    public partial class ClientSession : PacketSession
    {
        public int AccountDbId { get; private set; }

        // 아래 정보를 받아두지 않으면 해당 플레이어 외의 데이터를 참조해서 건드릴 수도 있게 된다.
        public List<LobbyPlayerInfo> LobbyPlayers { get; set; } = new List<LobbyPlayerInfo>();
        public void HandleLogin(C_Login loginPacket)
        {
            Console.WriteLine($"{loginPacket}");

            // TODO: 보안 체크
            if (ServerState != PlayerServerState.ServerStateLogin)
            {
                return;
            }

            LobbyPlayers.Clear();

            // TODO: 비동기로 빼야함..
            using (AppDbContext db = new AppDbContext())
            {
                AccountDb foundAccount = db.Accounts
                    .Include(a => a.Players)
                    .Where(a => a.AccountName == loginPacket.UniqueId).FirstOrDefault();

                if (foundAccount != null)
                {
                    AccountDbId = foundAccount.AccountDbId;

                    S_Login resLoginPacket = new S_Login() { LoginOk = true };
                    foreach (PlayerDb playerDb in foundAccount.Players)
                    {
                        LobbyPlayerInfo lobbyPlayer = new LobbyPlayerInfo()
                        {
                            PlayerDbId = playerDb.PlayerDbId,
                            Name = playerDb.PlayerName,
                            StatInfo = new StatInfo()
                            {
                                Level = playerDb.Level,
                                Hp = playerDb.Hp,
                                MaxHp = playerDb.MaxHp,
                                Attack = playerDb.Attack,
                                TotalExp = playerDb.TotalExp,
                                Speed = playerDb.Speed,
                            }

                        };

                        // 로비 관련 정보 패킷에 추가
                        resLoginPacket.Players.Add(lobbyPlayer);

                        // 메모리에도 추가, 이후 추가 패킷이 오면 해당 정보로 처리
                        LobbyPlayers.Add(lobbyPlayer);
                    }

                    Send(resLoginPacket);
                    
                    // 로비로 이동
                    ServerState = PlayerServerState.ServerStateLobby;
                }
                else
                {
                    // TODO: 단순히 하려고 이렇게 했는데, 후에는 계정 관리는 다른 방식으로 해야할 것 같다.
                    AccountDb newAccount = new AccountDb() { AccountName = loginPacket.UniqueId };
                    db.Accounts.Add(newAccount);
                    db.SaveChangesEx();

                    AccountDbId = newAccount.AccountDbId;

                    S_Login resLoginPacket = new S_Login() { LoginOk = true };
                    Send(resLoginPacket);

                    // 로비로 이동
                    ServerState = PlayerServerState.ServerStateLobby;
                }
            }
        }

        public void HandleCreatePlayer(C_CreatePlayer createPlayerPacket)
        {
            if (ServerState != PlayerServerState.ServerStateLobby)
            {
                return;
            }

            using (AppDbContext db = new AppDbContext())
            {
                PlayerDb foundPlayer = db.Players
                    .Where(p => p.PlayerName == createPlayerPacket.Name)
                    .FirstOrDefault();

                if(foundPlayer != null)
                {
                    Send(new S_CreatePlayer());
                }
                else
                {
                    StatInfo stat;
                    // 1레벨 스탯 설정
                    DataManager.StatDict.TryGetValue(1, out stat);

                    PlayerDb newPlayerDb = new PlayerDb()
                    {
                        PlayerName = createPlayerPacket.Name,
                        Level = stat.Level,
                        Hp = stat.Hp,
                        MaxHp = stat.MaxHp,
                        Attack = stat.Attack,
                        Speed = stat.Speed, 
                        TotalExp = 0,
                        AccountDbId = AccountDbId,
                    };

                    db.Players.Add(newPlayerDb);
                    db.SaveChangesEx(); 

                    // 위에서 새로 만들어진 정보를 메모리에서도 들고 있게 변경
                    LobbyPlayerInfo lobbyPlayer = new LobbyPlayerInfo()
                    {
                        PlayerDbId = newPlayerDb.PlayerDbId,
                        Name = newPlayerDb.PlayerName,
                        StatInfo = new StatInfo()
                        {
                            Level = newPlayerDb.Level,
                            Hp = newPlayerDb.Hp,
                            MaxHp = newPlayerDb.MaxHp,
                            Attack = newPlayerDb.Attack,
                            TotalExp = newPlayerDb.TotalExp,
                            Speed = newPlayerDb.Speed,
                        }
                    };

                    LobbyPlayers.Add(lobbyPlayer);

                    S_CreatePlayer resCreatePlayerPacket = new S_CreatePlayer()
                    {
                        Player = new LobbyPlayerInfo()
                    };

                    resCreatePlayerPacket.Player.MergeFrom(lobbyPlayer);

                    Send(resCreatePlayerPacket);
                }
            }
        }

        public void HandleEnterGame(C_EnterGame enterGamePacket)
        {
            if (ServerState != PlayerServerState.ServerStateLobby)
            {
                return;
            }

            LobbyPlayerInfo lobbyPlayerInfo = LobbyPlayers.Find(p => p.Name == enterGamePacket.Name);

            if(lobbyPlayerInfo == null)
            {
                return;
            }
            
            CurrentPlayer = ActorManager.Instance.Add<Player>();
            CurrentPlayer.PlayerDbId = lobbyPlayerInfo.PlayerDbId;
            CurrentPlayer.Info.Name = lobbyPlayerInfo.Name;
            CurrentPlayer.Info.PosInfo.MoveDir = MoveDir.Down;
            CurrentPlayer.Info.PosInfo.PosX = 0;
            CurrentPlayer.Info.PosInfo.PosX = 0;
            CurrentPlayer.Info.PosInfo.State = ActorState.Idle;
            CurrentPlayer.Session = this;

            CurrentPlayer.Stat.MergeFrom(lobbyPlayerInfo.StatInfo); // Protobuf 는 MergeFrom 으로 값 복사 가능

            S_ItemList itemListPacket = new S_ItemList();

            // 아이템 목록 로딩, 게임 접속 전이므로 동기 방식도 상관은 없다.
            using (AppDbContext db = new AppDbContext())
            {
                List<ItemDb> items = db.Items
                    .Where(i => i.OwnerDbId == lobbyPlayerInfo.PlayerDbId)
                    .ToList();

                foreach (ItemDb itemDb in items)
                {
                    Item item = Item.MakeItem(itemDb);
                    if(item != null)
                    {
                        CurrentPlayer.Inven.Add(item);
                        ItemInfo info = new ItemInfo();
                        info.MergeFrom(item.Info);
                        itemListPacket.Items.Add(info);
                    }
                }
            }

            // 아이템 목록 전송
            Send(itemListPacket);

            ServerState = PlayerServerState.ServerStateGame;

            RoomManager.Instance.Push(() =>
            {
                // TODO: 후에 Room 선택하는 방식으로
                Room room = RoomManager.Instance.Find(1);
                if (room != null)
                {
                    room.Push(room.EnterGame, CurrentPlayer, true);
                }
            });
        }
    }
}
