using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Define;

public class ArrowController : ActorController
{
    protected override void Init()
    {
        switch (_lastDir)
        {
            case MoveDir.Up:
                transform.rotation = Quaternion.Euler(0, 0, 0);
                break;

            case MoveDir.Down:
                transform.rotation = Quaternion.Euler(0, 0, 180);
                break;

            case MoveDir.Left:
                transform.rotation = Quaternion.Euler(0, 0, 90);
                break;

            case MoveDir.Right:
                transform.rotation = Quaternion.Euler(0, 0, 270);
                break;
        }

        State = ActorState.Moving;
        _speed = 15.0f;
        base.Init();
    }

    // 애니메이션 필요 없으므로 오버라이딩해서 제거
    protected override void UpdateAnimation()
    {
    }

    protected override void MoveToNextPosition()
    {
        if (_dir != MoveDir.None)
        {
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
                GameObject obj = Managers.ObjectManager.Find(destPos);
                if (obj == null)
                {
                    CellPos = destPos;
                }
                else
                {
                    ActorController ac = obj.GetComponent<ActorController>();
                    if (ac != null)
                    {
                        ac.OnDamaged();
                    }
                    Managers.Resource.Destroy(gameObject);
                }
            }
            else
            {
                Managers.Resource.Destroy(gameObject);
            }
        }
    }
}
