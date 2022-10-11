using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Define;

public class MonsterController : ActorController
{
    Coroutine _coPatrol;
    Coroutine _coSearch;
    Coroutine _coSkill;

    // private 이나 protected 변수나 같은 것을 유니티 에디터에서 보기 위한 특성
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
        // 타겟을 못찾을 경우 원래 이동할 위치
        Vector3Int destPos = _destCellPos;
        if (_target != null)
        {
            // 쫓아갈 대상을 찾으면, 해당 대상의 위치를 목표로 세팅
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

                // 스킬 사용할 것이므로 이동하지 않기에 아래 로직은 거치지 않는다.
                return;
            }
        }
        List<Vector3Int> path = Managers.MapManager.FindPath(CellPos, destPos, ignoreDestCollision: true);

        // 어쩌다보니 길을 못찾았거나, 상대가 멀다면 찾기 중지
        if (path.Count < 2 || path.Count > (int)_searchRange * 2)
        {
            _target = null;
            State = ActorState.Idle;
            return;
        }

        Vector3Int nextPos = path[1]; // 0 번은 현재 내가 있는 위치이다.
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
                yield break; // 아예 코루틴을 종료하는 것
            }
        }

        // 위에 for 문에서 갈 위치를 못정하면 멈춘다.
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
                    // Player 가 아니라면 false
                    PlayerController pc = obj.GetComponent<PlayerController>();
                    if (pc == null)
                    {
                        return false;
                    }

                    // 탐색 범위 내에 있지 않으면 false
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
        // 피격 판정
        GameObject obj = Managers.ObjectManager.Find(GetFrontCellPos());
        if (obj != null)
        {
            ActorController ac = obj.GetComponent<ActorController>();
            if (ac != null)
            {
                ac.OnDamaged();
            }
        }

        // 대기 시작
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

        // 대기 시작
        yield return new WaitForSeconds(0.3f);
        State = ActorState.Moving;
        _coSkill = null;
    }
}
