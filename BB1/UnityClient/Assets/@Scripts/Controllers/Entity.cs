using System;
using Spine;
using Spine.Unity;
using UnityEngine;
using UnityEngine.Rendering;
using Event = Spine.Event;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;
using Google.Protobuf.Protocol;
using Unity.VisualScripting;


// TODO: 컴포넌트 구조는 Unity 내부에서 알아서 스크립트로 붙일 수 있으니, 그거 제외한 서버 Entity 코드 일단 옮긴다. 추가 필요한 거 생길 수 있으니 지속적으로 확인 필요
public abstract class Entity : MonoBehaviour
{
    protected Protocol.EntityInfo _entityInfo;
    public SkeletonAnimation SkeletonAnim { get; set; }

    public abstract void Init(ulong dataSheetId);

    public int ExtraCells { get; protected set; } = 0; // 여러 칸을 차지하는 Entity의 경우에 사용
    public float MoveSpeed { get; set; } = 5.0f; // TODO: 추후 데이터나 다른 할당 방식 필요하면 바꿔야 한다.

    public void SetVector3IntToPosition(Vector3Int pos)
    {
        MoveInfo.Position.X = pos.x;
        MoveInfo.Position.Y = pos.y;
    }

    public Vector3Int GetVector3IntFromPosition()
    {
        return new Vector3Int(MoveInfo.Position.X, MoveInfo.Position.Y);
    }

    public Protocol.EntityInfo EntityInfo
    {
        get
        {
            return _entityInfo;
        }
        set
        {
            if (value == null)
            {
                return;
            }

            // 초기 할당 시 해야할 것들
            if (_entityInfo == null)
            {
                _entityInfo = value;
                CellPos = new Vector3Int(_entityInfo.MoveInfo.Position.X, _entityInfo.MoveInfo.Position.Y);
                gameObject.transform.position = Managers.Map.CellPosToWorldPos(CellPos);
                return;
            }

            // Entity 기본 정보 할당
            if (value.GameId != 0)
            {
                GameId = value.GameId;
            }

            if (value.DataSheetId != 0)
            {
                DataSheetId = value.DataSheetId;
            }

            if (value.MoveInfo != null)
            {
                MoveInfo = value.MoveInfo;
            }

            if (value.StatInfo != null)
            {
                StatInfo = value.StatInfo;
            }

            // ENtity 하위 클래스 정보에 따라 할당
            if (value.EntityTypeCase == Protocol.EntityInfo.EntityTypeOneofCase.ActorInfo)
            {
                _entityInfo.ActorInfo = value.ActorInfo;
            }
            else if (value.EntityTypeCase == Protocol.EntityInfo.EntityTypeOneofCase.ItemInfo)
            {
                _entityInfo.ItemInfo = value.ItemInfo;
            }
            else if (value.EntityTypeCase == Protocol.EntityInfo.EntityTypeOneofCase.ProjectileInfo)
            {
                _entityInfo.ProjectileInfo = value.ProjectileInfo;
            }
            else if (value.EntityTypeCase == Protocol.EntityInfo.EntityTypeOneofCase.ObstacleInfo)
            {
                _entityInfo.ObstacleInfo = value.ObstacleInfo;
            }
        }
    }


    #region Move
    private bool isMoving = false;
    [SerializeField] Vector3Int _cellPos;
    public Vector3Int CellPos
    {
        get { return _cellPos; }
        protected set
        {
            _cellPos = value;
        }
    }

    public Protocol.EntityInfo EntityInfoForMove 
    {
        get
        {
            var entityInfoForMove = new Protocol.EntityInfo();
            entityInfoForMove.MoveInfo = MoveInfo.Clone();
            entityInfoForMove.GameId = GameId;

            return entityInfoForMove;
        }
    }

    protected void UpdateMoveByTimeDelta(float moveSpeed, bool canFlip = true)
    {
        if (!isMoving)
        {
            return;
        }

        // 목적지 좌표를 맵 좌표계에서 월드 좌표로 변환
        Vector3 destWorldPos = Managers.Map.CellPosToWorldPos(CellPos);

        // 현재 위치와 목적지 위치 사이의 거리 계산
        Vector3 dir = destWorldPos - transform.position;
        float distance = dir.magnitude;

        // 좌우 방향 변경 (canFlip이 true일 때만)
        if (canFlip && dir.x != 0)
        {
            Flip(dir.x < 0);
        }

        // 이동
        float moveDist = moveSpeed * Time.deltaTime;
        if (distance <= moveDist)
        {
            // 목적지에 도착한 경우 위치를 정확히 설정하고 이동 종료
            transform.position = destWorldPos;
            isMoving = false;

            // 맵 충돌 정보 갱신
            Managers.Map.ApplyMove(this, CellPos);
            return;
        }

        // Lerp를 사용하여 부드럽게 이동
		transform.position += dir.normalized * moveDist;
    }



    public Protocol.MoveInfo MoveInfo
    {
        get
        {
            return _entityInfo.MoveInfo;
        }
        set
        {
            if (!_entityInfo.MoveInfo.Equals(value))
            {
                _entityInfo.MoveInfo = value;
                isMoving = true;
                CellPos = new Vector3Int(_entityInfo.MoveInfo.Position.X, _entityInfo.MoveInfo.Position.Y);
                ActorState = value.ActorState;
            }
        }
    }

    #endregion


    public Protocol.StatInfo StatInfo
    {
        get
        {
            return _entityInfo.StatInfo;
        }
        set
        {
            _entityInfo.StatInfo = value;
        }
    }

    public ulong DataSheetId
    {
        get
        {
            return _entityInfo.DataSheetId;
        }

        set
        {
            _entityInfo.DataSheetId = value;
        }
    }

    public ulong GameId
    {
        get
        {

            return _entityInfo.GameId;
        }

        set
        {
            _entityInfo.GameId = value;
        }
    }


    #region Animation
    [SerializeField]
    protected EActorState _actorState = EActorState.ActorStateIdle;
    public virtual EActorState ActorState
    {
        get { return MoveInfo.ActorState; }
        set
        {
            if (_actorState == value)
            {
                return;
            }

            _actorState = value;
            MoveInfo.ActorState = value;
            UpdateAnimation();
        }
    }
    protected virtual void OnAnimDisable()
    {
        if (SkeletonAnim == null)
            return;
        if (SkeletonAnim.AnimationState == null)
            return;

        SkeletonAnim.AnimationState.Event -= OnAnimEventHandler;
        SkeletonAnim.AnimationState.Complete -= OnAnimCompleteHandler;
    }
    protected virtual void OnAnimEventHandler(TrackEntry trackEntry, Event e) { }
    protected virtual void OnAnimCompleteHandler(TrackEntry arg1) { }

    // TODO: 아래 코드 다시 확인 필요
    public TrackEntry PlayAnimation(int trackIndex, string animName, bool loop, bool mixDuration = true)
    {
        if (SkeletonAnim == null)
        {
            return null;
        }

        if (SkeletonAnim.AnimationState == null)
        {
            return null;
        }

        TrackEntry entry = SkeletonAnim.AnimationState.SetAnimation(trackIndex, animName, loop);

        if (mixDuration == false)
            entry.MixDuration = 0;
        else
        {
            if (animName == AnimName.DEAD || animName.Contains("skill_a") || animName.Contains("skill_b"))
                entry.MixDuration = 0;
            else
                entry.MixDuration = 0.2f;
        }

        return entry;
    }

    public TrackEntry GetCurrentAnimation()
    {
        var trackEntry = SkeletonAnim.state.GetCurrent(0);
        return trackEntry;
    }

    public TrackEntry AddAnimation(int trackIndex, string AnimName, bool loop, float delay)
    {
        return SkeletonAnim.AnimationState.AddAnimation(trackIndex, AnimName, loop, delay);
    }

    public float GetSpineHeight()
    {
        float x, y, width, height;
        float[] vertexBuffer = null;
        SkeletonAnim.skeleton.GetBounds(out x, out y, out width, out height, ref vertexBuffer);
        return height;
    }

    public void Flip(bool flag)
    {
        if (SkeletonAnim == null)
            return;

        SkeletonAnim.Skeleton.ScaleX = flag ? -1 : 1;
    }

    protected virtual void UpdateAnimation()
    {
        switch (MoveInfo.ActorState)
        {
            case EActorState.ActorStateIdle:
                PlayAnimation(0, AnimName.IDLE, true);
                break;
            case EActorState.ActorStateSkill:
                break;
            case EActorState.ActorStateMoving:
                PlayAnimation(0, AnimName.MOVE, true);
                break;
            case EActorState.ActorStateDead:
                PlayAnimation(0, AnimName.DEAD, false);
                break;
        }
    }
    #endregion

    #region MonoBehaviour
    protected virtual void Awake()
    {

    }

    protected virtual void Start()
    {

    }

    protected virtual void Update()
    {
        UpdateMoveByTimeDelta(MoveSpeed);
    }
    protected virtual void OnDisable()
    {

    }

    protected virtual void OnDestory()
    {

    }
    #endregion
    // TODO: Map 과 연동된 이동 관련 요소 아래에 추가 필요
}

