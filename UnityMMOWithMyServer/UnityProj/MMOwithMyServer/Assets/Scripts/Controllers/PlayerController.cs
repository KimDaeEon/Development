using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Define;

public class PlayerController : ActorController
{
    Coroutine _coSkill;
    bool _isRangeSkill = false;

    private void LateUpdate()
    {
        Camera.main.transform.position = new Vector3(transform.position.x, transform.position.y, -10);
    }

    protected override void Init()
    {
        base.Init();
    }

    protected override void UpdateAnimation()
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
                    _animator.Play(_isRangeSkill ? "ATTACK_WEAPON_BACK" : "ATTACK_BACK");
                    _spriteRenderer.flipX = false;
                    break;

                case MoveDir.Down:
                    _animator.Play(_isRangeSkill ? "ATTACK_WEAPON_FRONT" : "ATTACK_FRONT");
                    _spriteRenderer.flipX = false;
                    break;

                case MoveDir.Left:
                    _animator.Play(_isRangeSkill ? "ATTACK_WEAPON_RIGHT" : "ATTACK_RIGHT");
                    _spriteRenderer.flipX = true;
                    break;

                case MoveDir.Right:
                    _animator.Play(_isRangeSkill ? "ATTACK_WEAPON_RIGHT" : "ATTACK_RIGHT");
                    _spriteRenderer.flipX = false;
                    break;
            }
        }
        else
        {

        }
    }

    protected override void UpdateController()
    {
        switch (State)
        {
            case ActorState.Idle:
                GetDirectionInput();
                break;

            case ActorState.Moving:
                GetDirectionInput();
                break;
        }

        base.UpdateController();
    }

    protected override void UpdateIdle()
    {
        if(Dir != MoveDir.None)
        {
            State = ActorState.Moving;
            return;
        }

        if (Input.GetKey(KeyCode.K))
        {
            _coSkill = StartCoroutine("CoStartPunch");
            State = ActorState.Skill;
        }
        else if (Input.GetKey(KeyCode.L))
        {
            _coSkill = StartCoroutine("CoStartShootArrow");
            State = ActorState.Skill;
        }
    }

    public override void OnDamaged()
    {
        Debug.Log("Player Hit !");
    }

    void GetDirectionInput()
    {
        if (Input.GetKey(KeyCode.W))
        {
            Dir = MoveDir.Up;
        }
        else if (Input.GetKey(KeyCode.S))
        {
            Dir = MoveDir.Down;
        }
        else if (Input.GetKey(KeyCode.A))
        {
            Dir = MoveDir.Left;
        }
        else if (Input.GetKey(KeyCode.D))
        {
            Dir = MoveDir.Right;
        }
        else
        {
            Dir = MoveDir.None;
        }
    }

    IEnumerator CoStartPunch()
    {
        // 피격 판정
        GameObject obj = Managers.ObjectManager.Find(GetFrontCellPos());
        if(obj != null)
        {
            ActorController ac = obj.GetComponent<ActorController>();
            if (ac != null)
            {
                ac.OnDamaged();
            }
        }
        _isRangeSkill = false;

        // 대기 시작
        yield return new WaitForSeconds(0.5f);
        State = ActorState.Idle;
        _coSkill = null;
    }

    IEnumerator CoStartShootArrow()
    {
        GameObject obj = Managers.Resource.Instantiate("Actor/Arrow");
        ArrowController controller = obj.GetComponent<ArrowController>();
        controller.Dir = _lastDir;
        controller.CellPos = CellPos;
        _isRangeSkill = true;

        // 대기 시작
        yield return new WaitForSeconds(0.3f);
        State = ActorState.Idle;
        _coSkill = null;
    }
}
