using DummyClient;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class PacketHandler
{
    public static void S_HeartBeatHandler(PacketSession session, IMessage packet)
    {
        C_HeartBeat heartBeatPacket = new C_HeartBeat();
        //Console.WriteLine("Heart Beat Check");

        //Managers.Network.Send(heartBeatPacket);
    }
    public static void S_CreatePlayerHandler(PacketSession session, IMessage packet)
    {
        S_CreatePlayer createPlayerPacket = (S_CreatePlayer)packet;
        ServerSession serverSession = (ServerSession)session;

        //// TODO: 원래는 UI 상에서 재생성 시도하도록 변경이 필요
        if (createPlayerPacket.Player == null)
        {
            // 더미 클라리언트에서 아래 부분은 아직은 필요가 없을 것 같다.

            //C_CreatePlayer createPlayer = new C_CreatePlayer();
            //createPlayer.Name = $"Player_{Random.Range(0, 10000).ToString("0000")}";
            //Managers.Network.Send(createPlayer);
        }
        else
        {
            // 생성된 캐릭터로 바로 로그인
            LobbyPlayerInfo info = createPlayerPacket.Player;
            C_EnterGame enterGamePacket = new C_EnterGame();
            enterGamePacket.Name = info.Name;
            serverSession.Send(enterGamePacket);
        }
    }

    public static void S_LoginHandler(PacketSession session, IMessage packet)
    {
        S_Login loginPacket = (S_Login)packet;
        Console.WriteLine($"{loginPacket}");

        ServerSession serverSession = (ServerSession)session;

        //// TODO: 캐릭터 다양하게 존재한다면 그것들을 선택하는 로직 추가 필요.
        if (loginPacket.Players == null || loginPacket.Players.Count == 0)
        {
            C_CreatePlayer createPacket = new C_CreatePlayer();
            createPacket.Name = $"Player_{serverSession.DummyId.ToString("0000")}";
            serverSession.Send(createPacket);
        }
        else
        {
            // 캐릭터 정보 있으면 무조건 첫번째 캐릭터로 로그인
            LobbyPlayerInfo info = loginPacket.Players[0];
            C_EnterGame enterGamePacket = new C_EnterGame();
            enterGamePacket.Name = info.Name;
            serverSession.Send(enterGamePacket);
        }
    }

    public static void S_ConnectedHandler(PacketSession session, IMessage packet)
    {
        ServerSession serverSession = (ServerSession)session;
        Console.WriteLine($"S_ConnectedHandler");
        C_LoginDummy loginPacket = new C_LoginDummy();

        // TODO: 이 부분도 로그인 개선이 필요
        loginPacket.AccountName = $"DummyClinet_{serverSession.DummyId.ToString("0000")}";
        serverSession.Send(loginPacket);
    }

    public static void S_EnterGameHandler(PacketSession session, IMessage packet)
    {
        S_EnterGame enterGamePacket = (S_EnterGame)packet;

        //Managers.Object.Add(enterGamePacket.Player, isMyPlayer: true);
    }

    public static void S_LeaveGameHandler(PacketSession session, IMessage packet)
    {
        //Managers.Object.Clear();
    }

    public static void S_SpawnHandler(PacketSession session, IMessage packet)
    {
        S_Spawn spawnPacket = (S_Spawn)packet;

        //// 해당 패킷에 자신에 대한 정보는 있으면 안된다. 아래 로직에서 문제 생김.
        //foreach (ActorInfo player in spawnPacket.Actors)
        //{
        //    Managers.Object.Add(player, isMyPlayer: false);
        //}
    }

    public static void S_DespawnHandler(PacketSession session, IMessage packet)
    {
        S_Despawn despawnPacket = (S_Despawn)packet;

        //foreach (int id in despawnPacket.ActorIds)
        //{
        //    Managers.Object.Remove(id);
        //}
    }

    public static void S_MoveHandler(PacketSession session, IMessage packet)
    {
        S_Move movePacket = (S_Move)packet;

        //GameObject obj = Managers.Object.FindById(movePacket.ActorId);

        //if (obj == null)
        //{
        //    return;
        //}

        //BaseController ac = obj.GetComponent<BaseController>();
        //if (ac == null)
        //{
        //    Console.WriteLine($"{obj.name} has sent a move packet, but has no actorController.");
        //    return;
        //}

        //// 이 부분 잘 보자. State 를 모두 다 PacketHandler 에서 정하고 있는 것이다.
        //// 이로 인해 캐릭터가 멈추고 뛰고 하는 상태가 동기화되어서 보인다.
        //ac.PosInfo = movePacket.PosInfo;
    }

    public static void S_SkillHandler(PacketSession session, IMessage packet)
    {
        S_Skill skillPacket = (S_Skill)packet;

        //GameObject obj = Managers.Object.FindById(skillPacket.ActorId);
        //if (obj == null)
        //{
        //    return;
        //}

        //ActorController ac = obj.GetComponent<ActorController>();
        //if (ac != null)
        //{
        //    ac.UseSkill(skillPacket.Info.SkillId);
        //}
    }

    public static void S_ChangeHpHandler(PacketSession session, IMessage packet)
    {
        S_ChangeHp changeHpPacket = (S_ChangeHp)packet;

        //GameObject obj = Managers.Object.FindById(changeHpPacket.ActorId);
        //if (obj == null)
        //{
        //    return;
        //}

        //ActorController ac = obj.GetComponent<ActorController>();
        //if (ac != null)
        //{
        //    ac.Hp = changeHpPacket.Hp;
        //}
    }

    public static void S_DeadHandler(PacketSession session, IMessage packet)
    {
        S_Dead changeHpPacket = (S_Dead)packet;

        //GameObject obj = Managers.Object.FindById(changeHpPacket.ActorId);
        //if (obj == null)
        //{
        //    return;
        //}

        //ActorController ac = obj.GetComponent<ActorController>();
        //if (ac != null)
        //{
        //    ac.OnDead();
        //}
    }

    public static void S_ItemListHandler(PacketSession session, IMessage packet)
    {
        S_ItemList itemListPacket = (S_ItemList)packet;

        //Managers.ItemInventory.Clear();

        //foreach (ItemInfo itemInfo in itemListPacket.Items)
        //{
        //    Item item = Item.MakeItem(itemInfo);
        //    Managers.ItemInventory.Add(item);
        //}

        //if (Managers.Object.MyPlayer != null)
        //{
        //    Managers.Object.MyPlayer.RefreshStat();
        //}
    }

    public static void S_AddItemHandler(PacketSession session, IMessage packet)
    {
        S_AddItem addItemPacket = (S_AddItem)packet;

        //foreach (ItemInfo itemInfo in addItemPacket.Items)
        //{
        //    Item item = Item.MakeItem(itemInfo);
        //    Managers.ItemInventory.Add(item);
        //}

        //Console.WriteLine("아이템 획득!");

        //UI_GameScene gameSceneUI = (UI_GameScene)Managers.UI.SceneUI;
        //UI_Inventory invenUI = gameSceneUI.InvenUI;
        //invenUI.RefreshUI();
    }

    public static void S_EquipItemHandler(PacketSession session, IMessage packet)
    {
        S_EquipItem equipItemPacket = (S_EquipItem)packet;

        //Item item = Managers.ItemInventory.Get(equipItemPacket.ItemDbId);
        //if (item == null)
        //{
        //    return;
        //}

        //item.Equipped = equipItemPacket.Equipped;
        //Console.WriteLine("아이템 착용");

        //UI_GameScene gameSceneUI = (UI_GameScene)Managers.UI.SceneUI;
        //gameSceneUI.InvenUI.RefreshUI();
        //gameSceneUI.StatUI.RefreshUI();

        //if (Managers.Object.MyPlayer != null)
        //{
        //    Managers.Object.MyPlayer.RefreshStat();
        //}
    }

    public static void S_ChangeStatHandler(PacketSession session, IMessage packet)
    {
        S_ChangeStat changeStatPacket = (S_ChangeStat)packet;
    }
}

