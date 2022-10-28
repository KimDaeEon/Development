using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public class Actor
    {
        public Room CurrentRoom { get; set; }
        public ActorType Type { get; protected set; } = ActorType.None;
        public ActorInfo Info { get; set; } = new ActorInfo();
        public PositionInfo PosInfo { get; private set; } = new PositionInfo();
        public StatInfo Stat { get; private set; } = new StatInfo();

        // TODO: 아래 부분도 능력치 세분화되면 수정 필요.
        public virtual int TotalAttack { get { return Stat.Attack; } }
        public virtual int TotalDeffence { get { return 0; } }

        public int Id
        {
            get { return Info.ActorId; }
            set { Info.ActorId = value; }
        }

        public float Speed
        {
            get { return Stat.Speed; }
            set { Stat.Speed = value; }
        }

        public MoveDir Dir
        {
            get { return PosInfo.MoveDir; }
            set { PosInfo.MoveDir = value; }
        }

        public ActorState State
        {
            get { return PosInfo.State; }
            set { PosInfo.State = value; }
        }

        public int Hp
        {
            get { return Stat.Hp; }
            set { Stat.Hp = Math.Clamp(value, 0, Stat.MaxHp); }
        }

        public Actor()
        {
            Info.PosInfo = PosInfo;
            Info.StatInfo = Stat;
        }

        public Vector2Int CellPos
        {
            get
            {
                return new Vector2Int(PosInfo.PosX, PosInfo.PosY);
            }

            set
            {
                PosInfo.PosX = value.x;
                PosInfo.PosY = value.y;
            }
        }

        public static MoveDir GetDirFromVec(Vector2Int dir)
        {
            if(dir.x > 0)
            {
                return MoveDir.Right;
            }
            else if(dir.x < 0)
            {
                return MoveDir.Left;
            }
            else if(dir.y > 0)
            {
                return MoveDir.Up;
            }
            else
            {
                return MoveDir.Down;
            }
        }

        public Vector2Int GetFronCellPos()
        {
            return GetFrontCellPos(PosInfo.MoveDir);
        }

        public Vector2Int GetFrontCellPos(MoveDir dir)
        {
            Vector2Int cellPos = CellPos;

            switch (dir)
            {
                case MoveDir.Up:
                    cellPos += Vector2Int.up;
                    break;

                case MoveDir.Down:
                    cellPos += Vector2Int.down;
                    break;

                case MoveDir.Left:
                    cellPos += Vector2Int.left;
                    break;

                case MoveDir.Right:
                    cellPos += Vector2Int.right;
                    break;
            }

            return cellPos;
        }

        public virtual void OnDamaged(Actor attacker, int damage)
        {
            if (CurrentRoom == null)
            {
                return;
            }

            damage = Math.Max(damage - TotalDeffence, 0);
            Stat.Hp = Math.Max(Stat.Hp - damage, 0);

            S_ChangeHp changePacket = new S_ChangeHp();
            changePacket.ActorId = Id;
            changePacket.Hp = Stat.Hp;
            
            // JobQueue 내에서 실행되는 것이기에 lock 필요 없다.
            CurrentRoom.Broadcast(CellPos, changePacket);

            if (Stat.Hp <= 0)
            {
                OnDead(attacker);
            }
        }

        public virtual void OnDead(Actor attacker)
        {
            if(CurrentRoom == null)
            {
                return;
            }

            S_Dead deadPacket = new S_Dead();
            deadPacket.ActorId = Id;
            deadPacket.AttackerActorId = attacker.Id;
            CurrentRoom.Broadcast(CellPos, deadPacket);

            // TODO: 임시로 죽으면 나갔다 들어오게 처리, 추후 수정 필요
            // LeaveGame 에서 CurrentRoom 을 Null 로 밀기 때문에 이렇게 해야한다.
            Room prevRoom = CurrentRoom;
            prevRoom.LeaveGame(Id);

            Stat.Hp = Stat.MaxHp;
            PosInfo.State = ActorState.Idle;
            PosInfo.MoveDir = MoveDir.Down;

            prevRoom.EnterGame(this, true);
        }

        public virtual void Update()
        {

        }

        public virtual Actor GetOwner()
        {
            return this;
        }
    }
}
