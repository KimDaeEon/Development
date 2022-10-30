using CSharpServer.Data;
using CSharpServer.DB;
using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public class Monster : Actor
    {
        Player _target;
        public int TemplateId { get; private set; }
        long _nextSearchTick = 0;
        int _searchCellDistLimit = 10; // 플레이어 찾는 거리 한계
        int _chaseCellDistLimit = 15; // 플레이어 쫓아갈 때에 쫓는 거리 한계
        int _skillRange = 1;
        long _skillCoolTick = 0;

        // 이전 작업을 취소하기 위함
        IJob _job;

        public Monster()
        {
            Type = ActorType.Monster;
        }

        public void Init(int templateId)
        {
            TemplateId = templateId;

            MonsterData monsterData;
            DataManager.MonsterDict.TryGetValue(templateId, out monsterData);
            Stat.MergeFrom(monsterData.stat);
            Stat.Hp = Stat.MaxHp;

            State = ActorState.Idle;
        }

        // TODO: 지금은 FSM 으로 만들었는데, 후에 Behaviour Tree 도 적용해보고
        // 데이터 시트로 작동하도록 변경이 필요
        public override void Update()
        {
            switch (State)
            {
                case ActorState.Idle:
                    UpdateIdle();
                    break;
                case ActorState.Moving:
                    UpdateMoving();
                    break;
                case ActorState.Skill:
                    UpdateSkill();
                    break;
                case ActorState.Dead:
                    UpdateDead();
                    break;
            }

            // 5프레임 (0.2초마다 한번씩 Update)
            if (CurrentRoom != null)
                _job = CurrentRoom.PushFutureJob(200, Update);
        }

        protected virtual void UpdateIdle()
        {
            if (_nextSearchTick > Environment.TickCount64)
            {
                return;
            }

            _nextSearchTick = Environment.TickCount64 + 1000;

            Player target = CurrentRoom.FindClosestPlayer(CellPos, _searchCellDistLimit);

            if (target == null)
            {
                return;
            }

            _target = target;
            State = ActorState.Moving;
        }

        long _nextMoveTick = 0;
        protected virtual void UpdateMoving()
        {
            if (_nextMoveTick > Environment.TickCount64)
            {
                return;
            }

            int moveTick = (int)(1000 / Speed); // C#, C++, 자바 모두 정수/유리수 는 가능하다.
            _nextMoveTick = Environment.TickCount64 + moveTick;

            // 플레이어가 다른 Room 으로 이동했다면 이동 중지
            if (_target == null || _target.CurrentRoom != CurrentRoom)
            {
                _target = null;
                State = ActorState.Idle;
                BroadcastMove();
                return;
            }

            Vector2Int dir = _target.CellPos - CellPos;
            int cellDistFromZero = (_target.CellPos - CellPos).CellDistFromZero;
            if (cellDistFromZero == 0 || cellDistFromZero > _chaseCellDistLimit)
            {
                _target = null;
                State = ActorState.Idle;
                BroadcastMove();
                return;
            }

            // 목표릃 향해 최단 거리로 이동
            var path = CurrentRoom.Map.FindPath(CellPos, _target.CellPos, checkActorCollision: true, _searchCellDistLimit);
            if (path.Count < 2 || path.Count > _chaseCellDistLimit)
            {
                _target = null;
                State = ActorState.Idle;
                BroadcastMove();
                return;
            }

            // 스킬은 스킬 범위안에 들면서 상대를 쳐다보고 있는 경우에 사용
            if (cellDistFromZero <= _skillRange && (dir.x == 0 || dir.y == 0))
            {
                State = ActorState.Skill;
                _skillCoolTick = 0;
                return;
            }

            Dir = GetDirFromVec(path[1] - CellPos);
            CurrentRoom.Map.ApplyMove(this, path[1]);
            BroadcastMove();
        }

        void BroadcastMove()
        {
            // 플레이어들에게 움직였음을 통지
            S_Move movePacket = new S_Move();
            movePacket.ActorId = Id;
            movePacket.PosInfo = PosInfo;
            CurrentRoom.Broadcast(CellPos, movePacket);
        }

        protected virtual void UpdateSkill()
        {
            if (_skillCoolTick == 0)
            {
                // 유효한 타겟인지 체크
                if (_target == null || _target.CurrentRoom != CurrentRoom /*|| _target.Hp == 0*/)
                {
                    _target = null;
                    State = ActorState.Moving;
                    BroadcastMove();
                    return;
                }

                // 스킬이 아직 사용 가능한지
                Vector2Int targetDir = _target.CellPos - CellPos;
                int dist = targetDir.CellDistFromZero;
                bool canUseSkill = (dist <= _skillRange && ((targetDir.x == 0) || (targetDir.y == 0)));
                if (canUseSkill == false)
                {
                    State = ActorState.Moving;
                    BroadcastMove();
                    return;
                }

                // 목표 방향으로 재회전
                MoveDir lookDir = GetDirFromVec(targetDir);
                if (Dir != lookDir)
                {
                    Dir = lookDir;
                    BroadcastMove();
                }

                // TODO: 이 부분도 Ai 와 데이터를 이용하여 그때 그때 다른 스킬 사용하도록 변경 필요
                SkillData skillData;
                DataManager.SkillDict.TryGetValue(1, out skillData);
                _target.OnDamaged(this, skillData.damage + TotalAttack);

                // Broadcast
                S_Skill skillPacket = new S_Skill() { Info = new SkillInfo() };
                skillPacket.ActorId = Id;
                skillPacket.Info.SkillId = skillData.id;
                CurrentRoom.Broadcast(CellPos, skillPacket);

                int coolTick = (int)(skillData.cooltime * 1000);
                _skillCoolTick = Environment.TickCount64 + coolTick;
            }

            if (_skillCoolTick > Environment.TickCount64)
            {
                return;
            }

            _skillCoolTick = 0;
        }

        protected virtual void UpdateDead()
        {

        }

        public override void OnDead(Actor attacker)
        {
            if (_job != null)
            {
                _job.Cancel = true;
                _job = null;
            }

            base.OnDead(attacker);

            // 펫 같은 것이 생기면 펫이 죽였을 때에 템이 안떨어지는 사태가 생길 수 있다.
            // 그것을 대비해서 GetOwner 함수를 사용.
            Actor owner = attacker.GetOwner();

            if (owner.Type == ActorType.Player)
            {
                RewardData rewardData = GetRandomReward();
                if (rewardData != null)
                {
                    Player player = (Player)owner;

                    DbJobQueue.RewardPlayer(player, rewardData, player.CurrentRoom);
                }
            }
        }

        RewardData GetRandomReward()
        {
            MonsterData monsterData;
            DataManager.MonsterDict.TryGetValue(TemplateId, out monsterData);

            int rand = new Random().Next(0, 2001); // TODO: 이 부분도 미리 계산해서 범위를 정해야 한다.
            int probSum = 0;

            // TODO: 일단 이렇게 하였는데, 추후에는 떨구는 아이템 갯수도 정할 수 있도록 하자.
            foreach (RewardData rewardData in monsterData.rewards)
            {
                probSum += rewardData.probability;
                if (rand <= probSum)
                {
                    return rewardData;
                }
            }

            return null;
        }
    }
}
