using Google.Protobuf.Protocol;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Define;

public class ArrowController : BaseController
{
    protected override void Init()
    {
        switch (Dir)
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

        base.Init();
    }

    // 애니메이션 필요 없으므로 오버라이딩해서 제거
    protected override void UpdateAnimation()
    {
    }
}
