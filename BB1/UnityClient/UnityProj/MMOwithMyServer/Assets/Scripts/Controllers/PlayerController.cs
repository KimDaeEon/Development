using Google.Protobuf.Protocol;
using System.Collections;
using UnityEngine;
using static Define;

public class PlayerController : ActorController
{
    protected Coroutine _coSkill;
    protected bool _isRangeSkill = false;

    protected override void Init()
    {
        base.Init();
    }

    protected override void UpdateAnimation()
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
    }

    protected override void UpdateController()
    {
        base.UpdateController();
    }

    public override void OnDamaged()
    {
        Debug.Log("Player Hit !");
    }

    public override void UseSkill(int skillId)
    {
        // TODO: ������ ��Ʈ�� ������, �ӽ÷� skillId 1�� ��ġ�� ����

        switch (skillId)
        {
            case 1:
                _coSkill = StartCoroutine("CoStartPunch");
                break;

            case 2:
                _coSkill = StartCoroutine("CoStartShootArrow");
                break;

            default:
                Debug.Log($"UnKnown Skill id {skillId}");
                break;
        }
    }

    // MyPlayerController ������ ����
    protected virtual void SendMovePacketIfUpdated()
    {
    }

    IEnumerator CoStartPunch()
    {
        // ��� ����
        _isRangeSkill = false;
        State = ActorState.Skill;
        yield return new WaitForSeconds(0.5f);
        State = ActorState.Idle;
        _coSkill = null;

        // TODO: �ӽ� �ڵ�. �Ŀ� �������� ��ų ���� ���� �ٲٴ� �� �ϵ��� ���� �ʿ�
        SendMovePacketIfUpdated();
    }

    IEnumerator CoStartShootArrow()
    {
        // ��� ����
        _isRangeSkill = true;
        State = ActorState.Skill;
        yield return new WaitForSeconds(0.3f);
        State = ActorState.Idle;
        _coSkill = null;

        // TODO: �ӽ� �ڵ�. �Ŀ� �������� ��ų ���� ���� �ٲٴ� �� �ϵ��� ���� �ʿ�
        SendMovePacketIfUpdated();
    }
}
