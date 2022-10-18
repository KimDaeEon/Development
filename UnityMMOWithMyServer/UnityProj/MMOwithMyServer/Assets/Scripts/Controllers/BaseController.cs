using Google.Protobuf.Protocol;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Define;

public class BaseController : MonoBehaviour
{
    protected bool _updated = false; // 움직이는 방향이나 상태가 바뀌면 true 로 세팅
    StatInfo _stat = new StatInfo();
    public virtual StatInfo Stat
    {
        get { return _stat; }
        set
        {
            if (_stat.Equals(value))
            {
                return;
            }

            _stat = value;
        }
    }

    public virtual int Hp
    {
        get { return Stat.Hp; }
        set
        {
            Stat.Hp = value;
        }
    }

    public float Speed
    {
        get { return Stat.Speed; }
        set { Stat.Speed = value; }
    }

    PositionInfo _positionInfo = new PositionInfo();
    public PositionInfo PosInfo
    {
        get { return _positionInfo; }
        set
        {
            if (_positionInfo.Equals(value))
            {
                return;
            }
            CellPos = new Vector3Int(value.PosX, value.PosY, 0);
            State = value.State;
            Dir = value.MoveDir; // 이를 통해 lastDir 동기화
        }
    }
    public int Id { get; set; }

    // 현재는 움직임이 부드럽게 가도록 보정해서 Update 에서 움직이는데,
    // 처음 게임 접속 같은 경우 만들어진 위치에 바로 나와야 하므로 transform.position 을 바로 바꾸는 해당 함수가 필요
    public void SyncPosInstantly()
    {
        Vector3 destPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.5f, 0.5f);
        transform.position = destPos;
    }

    public Vector3Int CellPos
    {
        get
        {
            return new Vector3Int(PosInfo.PosX, PosInfo.PosY, 0);
        }
        set
        {
            PosInfo.PosX = value.x;
            PosInfo.PosY = value.y;
            _updated = true;
        }
    }

    protected Animator _animator;
    protected SpriteRenderer _spriteRenderer;

    public MoveDir Dir
    {
        get
        {
            return PosInfo.MoveDir;
        }
        set
        {
            if (PosInfo.MoveDir == value)
            {
                return;
            }

            PosInfo.MoveDir = value;

            UpdateAnimation();
            _updated = true;
        }
    }

    public virtual ActorState State
    {
        get
        {
            return PosInfo.State;
        }

        set
        {
            if (PosInfo.State == value)
            {
                return;
            }

            PosInfo.State = value;
            UpdateAnimation();
            _updated = true;
        }
    }

    // 벡터가 주어지면 벡터에 해당하는 MoveDir 을 리턴
    public MoveDir GetDirFromVec3Int(Vector3Int dir)
    {
        if (dir.x > 0)
        {
            return MoveDir.Right;
        }
        else if (dir.x < 0)
        {
            return MoveDir.Left;
        }
        else if (dir.y > 0)
        {
            return MoveDir.Up;
        }
        else
        {
            return MoveDir.Down;
        }
    }

    public Vector3Int GetFrontCellPos()
    {
        Vector3Int cellPos = CellPos;

        switch (Dir)
        {
            case MoveDir.Up:
                cellPos += Vector3Int.up;
                break;

            case MoveDir.Down:
                cellPos += Vector3Int.down;
                break;

            case MoveDir.Left:
                cellPos += Vector3Int.left;
                break;

            case MoveDir.Right:
                cellPos += Vector3Int.right;
                break;
        }

        return cellPos;
    }

    protected virtual void UpdateIdle()
    {
    }
    protected virtual void UpdateMoving()
    {
        Vector3 destPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.5f, 0.5f);
        Vector3 moveDir = destPos - transform.position; // 정규화되지 않은 위치 차이이자, 방향 + 크기를 담은 벡터

        // 목표 지점까지 남은 거리가 한 프레임에 이동하는 거리보다 적다는 것을 목표 지점에 도착했다는 것으로 판정
        float distance = moveDir.magnitude;
        if (distance < Speed * Time.deltaTime)
        {
            transform.position = destPos;
            MoveToNextPosition();
        }
        else
        {
            // 조금씩 이동
            transform.position += moveDir.normalized * Speed * Time.deltaTime; // 여기서 moveDir 을 정규화시켜서 스피드(크기)를 곱한다.
            State = ActorState.Moving;
        }
    }
    protected virtual void UpdateSkill()
    {

    }

    protected virtual void UpdateDead()
    {

    }

    #region UnityEngine
    private void Start()
    {
        Init();
    }

    private void Update()
    {
        UpdateController();
    }
    #endregion

    protected virtual void Init()
    {
        _animator = GetComponent<Animator>();
        _spriteRenderer = GetComponent<SpriteRenderer>();
        Vector3 worldPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.5f, 0.5f);
        transform.position = worldPos;

        // 기본 프리팹이 오른쪽을 보는 애니메이션으로 해두어서 아래의 내용이 필요.
        UpdateAnimation();
    }
    protected virtual void UpdateController()
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
                break;

            case ActorState.Dead:
                break;
        }
    }

    protected virtual void UpdateAnimation()
    {
        if(_animator == null || _spriteRenderer == null)
        {
            return;
        }

        if (State == ActorState.Idle)
        {
            switch (Dir)
            {
                case MoveDir.Up:
                    _animator.Play("IDLE_BACK");
                    _spriteRenderer.flipX = false;
                    break;

                case MoveDir.Down:
                    _animator.Play("IDLE_FRONT");
                    _spriteRenderer.flipX = false;
                    break;

                case MoveDir.Left:
                    _animator.Play("IDLE_RIGHT");
                    _spriteRenderer.flipX = true;
                    break;

                case MoveDir.Right:
                    _animator.Play("IDLE_RIGHT");
                    _spriteRenderer.flipX = false;
                    break;
            }
        }
        else if (State == ActorState.Moving)
        {
            switch (Dir)
            {
                case MoveDir.Up:
                    _animator.Play("WALK_BACK");
                    _spriteRenderer.flipX = false;
                    break;

                case MoveDir.Down:
                    _animator.Play("WALK_FRONT");
                    _spriteRenderer.flipX = false;
                    break;

                case MoveDir.Left:
                    _animator.Play("WALK_RIGHT");
                    _spriteRenderer.flipX = true;
                    break;

                case MoveDir.Right:
                    _animator.Play("WALK_RIGHT");
                    _spriteRenderer.flipX = false;
                    break;
            }
        }
        else if (State == ActorState.Skill)
        {
            switch (Dir)
            {
                case MoveDir.Up:
                    _animator.Play("ATTACK_BACK");
                    _spriteRenderer.flipX = false;
                    break;

                case MoveDir.Down:
                    _animator.Play("ATTACK_FRONT");
                    _spriteRenderer.flipX = false;
                    break;

                case MoveDir.Left:
                    _animator.Play("ATTACK_RIGHT");
                    _spriteRenderer.flipX = true;
                    break;

                case MoveDir.Right:
                    _animator.Play("ATTACK_RIGHT");
                    _spriteRenderer.flipX = false;
                    break;
            }
        }
    }

    protected virtual void MoveToNextPosition()
    {

    }
}
