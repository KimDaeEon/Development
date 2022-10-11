using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Define;

public class MonsterController : ActorController
{
    Coroutine _coPatrol;
    Coroutine _coSearch;
    Coroutine _coSkill;

    // private �̳� protected ������ ���� ���� ����Ƽ �����Ϳ��� ���� ���� Ư��
    [SerializeField]
    Vector3Int _destCellPos;

    [SerializeField]
    GameObject _target;

    [SerializeField]
    float _searchRange = 10.0f;

    [SerializeField]
    float _skillRange = 1.0f;

    [SerializeField]
    bool _rangedSkill = false;

    public override ActorState State
    {
        get { return _state; }
        set
        {
            if (_state == value)
            {
                return;
            }

            base.State = value;
            if (_coPatrol != null)
            {
                StopCoroutine(_coPatrol);
                _coPatrol = null;
            }

            base.State = value;
            if (_coSearch != null)
            {
                StopCoroutine(_coSearch);
                _coSearch = null;
            }
        }
    }
    protected override void Init()
    {
        base.Init();
        State = ActorState.Idle;
        Dir = MoveDir.None;
        _speed = 3.0f;
        _rangedSkill = (Random.Range(0, 2) == 0 ? true : false);

        if (_rangedSkill)
        {
            _skillRange = 10.0f;
        }
        else
        {
            _skillRange = 1.0f;
        }
    }
    protected override void UpdateIdle()
    {
        base.UpdateIdle();

        if (_coPatrol == null)
        {
            _coPatrol = StartCoroutine("CoStartPatrol");
        }

        if (_coSearch == null)
        {
            _coSearch = StartCoroutine("CoStartSearch");
        }
    }

    protected override void MoveToNextPosition()
    {
        // Ÿ���� ��ã�� ��� ���� �̵��� ��ġ
        Vector3Int destPos = _destCellPos;
        if (_target != null)
        {
            // �Ѿư� ����� ã����, �ش� ����� ��ġ�� ��ǥ�� ����
            destPos = _target.GetComponent<ActorController>().CellPos;

            Vector3Int destDir = destPos - CellPos;
            if (destDir.magnitude <= _skillRange && (destDir.x == 0 || destDir.y == 0))
            {
                Dir = GetDirFromVec3Int(destDir);
                State = ActorState.Skill;

                if (_rangedSkill)
                {
                    _coSkill = StartCoroutine("CoStartShootArrow");
                }
                else
                {
                    _coSkill = StartCoroutine("CoStartPunch");
                }

                // ��ų ����� ���̹Ƿ� �̵����� �ʱ⿡ �Ʒ� ������ ��ġ�� �ʴ´�.
                return;
            }
        }
        List<Vector3Int> path = Managers.MapManager.FindPath(CellPos, destPos, ignoreDestCollision: true);

        // ��¼�ٺ��� ���� ��ã�Ұų�, ��밡 �ִٸ� ã�� ����
        if (path.Count < 2 || path.Count > (int)_searchRange * 2)
        {
            _target = null;
            State = ActorState.Idle;
            return;
        }

        Vector3Int nextPos = path[1]; // 0 ���� ���� ���� �ִ� ��ġ�̴�.
        Vector3Int moveCellDir = nextPos - CellPos;

        Dir = GetDirFromVec3Int(moveCellDir);

        if (Managers.MapManager.CanGo(nextPos) && Managers.ObjectManager.Find(nextPos) == null)
        {
            CellPos = nextPos;
        }
        else
        {
            State = ActorState.Idle;
        }
    }
    public override void OnDamaged()
    {
        GameObject effect = Managers.Resource.Instantiate("Effect/DeathEffect");
        effect.transform.position = transform.position;
        effect.GetComponent<Animator>().Play("Death");
        GameObject.Destroy(effect, 0.5f);

        Managers.ObjectManager.Remove(gameObject);
        Managers.Resource.Destroy(gameObject);
    }

    IEnumerator CoStartPatrol()
    {
        int waitSeconds = Random.Range(1, 4);
        yield return new WaitForSeconds(waitSeconds);

        for (int i = 0; i < 10; i++)
        {
            int xRange = Random.Range(-5, 6);
            int yRange = Random.Range(-5, 6);

            Vector3Int randPos = CellPos + new Vector3Int(xRange, yRange, 0);

            if (Managers.MapManager.CanGo(randPos) && Managers.ObjectManager.Find(randPos) == null)
            {
                _destCellPos = randPos;
                State = ActorState.Moving;
                yield break; // �ƿ� �ڷ�ƾ�� �����ϴ� ��
            }
        }

        // ���� for ������ �� ��ġ�� �����ϸ� �����.
        State = ActorState.Idle;
    }

    IEnumerator CoStartSearch()
    {
        while (true)
        {
            yield return new WaitForSeconds(1.0f);

            if (_target != null)
            {
                continue;
            }

            _target = Managers.ObjectManager.Find(
                (obj) =>
                {
                    // Player �� �ƴ϶�� false
                    PlayerController pc = obj.GetComponent<PlayerController>();
                    if (pc == null)
                    {
                        return false;
                    }

                    // Ž�� ���� ���� ���� ������ false
                    Vector3Int dir = pc.CellPos - CellPos;
                    if (dir.magnitude > _searchRange)
                    {
                        return false;
                    }

                    return true;
                }
                );
        }
    }
    IEnumerator CoStartPunch()
    {
        // �ǰ� ����
        GameObject obj = Managers.ObjectManager.Find(GetFrontCellPos());
        if (obj != null)
        {
            ActorController ac = obj.GetComponent<ActorController>();
            if (ac != null)
            {
                ac.OnDamaged();
            }
        }

        // ��� ����
        yield return new WaitForSeconds(0.5f);
        State = ActorState.Moving;
        _coSkill = null;
    }

    IEnumerator CoStartShootArrow()
    {
        GameObject obj = Managers.Resource.Instantiate("Actor/Arrow");
        ArrowController controller = obj.GetComponent<ArrowController>();
        controller.Dir = _lastDir;
        controller.CellPos = CellPos;

        // ��� ����
        yield return new WaitForSeconds(0.3f);
        State = ActorState.Moving;
        _coSkill = null;
    }
}
