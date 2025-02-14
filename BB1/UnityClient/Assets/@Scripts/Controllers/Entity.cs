using System;
using Spine;
using Spine.Unity;
using UnityEngine;
using UnityEngine.Rendering;
using Event = Spine.Event;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;
using Google.Protobuf.Protocol;


// TODO: 컴포넌트 구조는 Unity 내부에서 알아서 스크립트로 붙일 수 있으니, 그거 제외한 서버 Entity 코드 일단 옮긴다. 추가 필요한 거 생길 수 있으니 지속적으로 확인 필요
public abstract class Entity : MonoBehaviour
{
    protected Protocol.EntityInfo _entityInfo;
    public SkeletonAnimation SkeletonAnim { get; set; }

    public abstract void Init(ulong dataSheetId);

    public int ExtraCells { get; protected set; } = 0; // 여러 칸을 차지하는 Entity의 경우에 사용

    public void SetVector2IntToPosition(Vector2Int pos)
    {
        MoveInfo.Position.X = pos.x;
        MoveInfo.Position.Y = pos.y;
    }

    public Vector2Int GetVector2IntFromPosition()
    {
        return new Vector2Int(MoveInfo.Position.X, MoveInfo.Position.Y);
    }

    public Protocol.EntityInfo EntityInfo
    {
        get
        {
            return _entityInfo;
        }
        set
        {
            _entityInfo = value;
        }
    }

    public Protocol.MoveInfo MoveInfo
    {
        get
        {
            return _entityInfo.MoveInfo;
        }
        set
        {
            _entityInfo.MoveInfo = value;
        }
    }

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

    // TODO: Map 과 연동된 이동 관련 요소 아래에 추가 필요
}

