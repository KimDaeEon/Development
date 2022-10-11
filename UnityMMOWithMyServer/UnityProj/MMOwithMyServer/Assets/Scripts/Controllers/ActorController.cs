using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Define;

public class ActorController : MonoBehaviour
{
    public float _speed = 5.0f;
    public Vector3Int CellPos { get; set; } = Vector3Int.zero;
    protected Animator _animator;
    protected SpriteRenderer _spriteRenderer;

    protected MoveDir _dir = MoveDir.Down; // Idle 상태
    public MoveDir Dir
    {
        get { return _dir; }
        set
        {
            if (_dir == value)
            {
                return;
            }

            _dir = value;
            if (value != MoveDir.None)
            {
                _lastDir = value;
            }

            UpdateAnimation();
        }
    }

    [SerializeField]
    protected MoveDir _lastDir = MoveDir.Down; // 마지막으로 움직였을 때의 방향
    
    [SerializeField]
    protected ActorState _state = ActorState.Idle;
    public virtual ActorState State
    {
        get { return _state; }
        set
        {
            if (_state == value)
            {
                return;
            }

            _state = value;
            UpdateAnimation();
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
        else if (dir.y < 0)
        {
            return MoveDir.Down;
        }
        else
        {
            return MoveDir.None;
        }
    }

    public Vector3Int GetFrontCellPos()
    {
        Vector3Int cellPos = CellPos;

        switch (_lastDir)
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
        Vector3 destPos = Managers.MapManager.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.5f, 0.5f);
        Vector3 moveDir = destPos - transform.position; // 정규화되지 않은 위치 차이이자, 방향 + 크기를 담은 벡터

        // 목표 지점까지 남은 거리가 한 프레임에 이동하는 거리보다 적다는 것을 목표 지점에 도착했다는 것으로 판정
        float distance = moveDir.magnitude;
        if (distance < _speed * Time.deltaTime)
        {
            transform.position = destPos;
            MoveToNextPosition();
        }
        else
        {
            // 조금씩 이동
            transform.position += moveDir.normalized * _speed * Time.deltaTime; // 여기서 moveDir 을 정규화시켜서 스피드(크기)를 곱한다.
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
        Vector3 worldPos = Managers.MapManager.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.5f, 0.5f);
        transform.position = worldPos;
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
        if (_state == ActorState.Idle)
        {
            switch (_lastDir)
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
        else if (_state == ActorState.Moving)
        {
            switch (_dir)
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

                case MoveDir.None:
                    break;
            }
        }
        else if (_state == ActorState.Skill)
        {
            switch (_lastDir)
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
        if(_dir == MoveDir.None)
        {
            State = ActorState.Idle;
            return;
        }

        Vector3Int destPos = CellPos;

        switch (_dir)
        {
            case MoveDir.Up:
                destPos += Vector3Int.up;
                break;
            case MoveDir.Down:
                destPos += Vector3Int.down;
                break;
            case MoveDir.Left:
                destPos += Vector3Int.left;
                break;
            case MoveDir.Right:
                destPos += Vector3Int.right;
                break;
        }

        if (Managers.MapManager.CanGo(destPos))
        {
            if (Managers.ObjectManager.Find(destPos) == null)
            {
                CellPos = destPos;
            }
        }
    }

    public virtual void OnDamaged()
    {
    }
}
