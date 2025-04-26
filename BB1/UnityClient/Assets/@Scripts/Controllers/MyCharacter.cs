using Google.Protobuf.Protocol;
using UnityEngine;

public class MyCharacter : Character
{
    // 여기서 내가 조종하는 캐릭터 관련 필요 정보 및 기능들이 추가 되어야 함
    public override void Init(ulong dataSheetId)
    {
        DataSheetId = dataSheetId;

        // TODO: 데이터 세팅
    }

    #region Move
    // 이동 패킷 전송 관련 (일종의 dirty flag)
    protected bool _sendMovePacket = false;
    Vector3Int _nextDestPos; // A* 로 길찾기를 할 때에 한 번에 1칸이나 2칸씩 이동하는데, 한 번에 이동할 바로 다음 위치를 말한다.
    Vector3? _targetDestPos; // 마우스로 찍었을 때에 최종 가고 싶은 목적지

    private GameObject _moveCursor;
    public Vector3Int NextDestPos
    {
        get { return _nextDestPos; }
        set
        {
            if (_nextDestPos == value)
            {
                return;
            }

            _nextDestPos = value;
            _sendMovePacket = true;
        }
    }

    protected override void Update()
    {
        // 입력 처리
        UpdateInput();

        UpdateMoveByTimeDelta(MoveSpeed);

        // 무언가 입력했으면 패킷을 보낸다.
        UpdateSendMovePacket();

        // 카메라를 따라오게 함
        UpdateCameraFollow();
    }

    void UpdateCameraFollow()
    {
        if (Camera.main != null)
        {
            Vector3 targetPosition = new Vector3(transform.position.x, transform.position.y, Camera.main.transform.position.z);
            Camera.main.transform.position = Vector3.Lerp(Camera.main.transform.position, targetPosition, Time.deltaTime * 5f);
        }
    }

    // TODO: 이 부분 추후에 좀 더 다듬어야 한다. 캐릭터가 있는 좌표를 바로 찍었으면 근처에 있는 좌표로 이동하도록 수정 필요
    void UpdateInput()
    {
        // GetMousButton 을 하면 누르고 있는 시간 동안 계속 여기로 들어와서 원하는 목표로 이동할 수 있다.
        if (Input.GetMouseButton(1))
        {
            Vector2 mouseWorldPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            _targetDestPos = mouseWorldPos;

            Vector3Int targetCellPos = Managers.Map.WorldPosToCellPos(new Vector3(mouseWorldPos.x, mouseWorldPos.y, 0));

            var path = Managers.Map.FindPath(this, CellPos, targetCellPos);

            // 경로가 2칸 이상이라면 움직일 수 있으므로 패킷을 보낸다.
            if (path.Count >= 2)
            {
                NextDestPos = path[path.Count - 1];
            }

            SpawnOrMoveMouseCursor(mouseWorldPos);
        }
    }

    void SpawnOrMoveMouseCursor(Vector2 position)
    {
        if(_moveCursor == null)
        {
            _moveCursor = Managers.Resource.Instantiate("Cursor", pooling: true);
            _moveCursor.transform.position = position;
        }
        else
        {
            _moveCursor.transform.position = position;
        }
    }

    public void DestroyMouseCursor()
    {
        Managers.Resource.Destroy(_moveCursor);
        _moveCursor = null;
    }

    void UpdateSendMovePacket()
    {
        if (_sendMovePacket)
        {
            C_Move movePkt = new C_Move();
            movePkt.EntityInfo = EntityInfoForMove;
            movePkt.EntityInfo.MoveInfo.Position.X = NextDestPos.x;
            movePkt.EntityInfo.MoveInfo.Position.Y = NextDestPos.y;
            Managers.Network.GameServer.Send(movePkt);
            _sendMovePacket = false;

            Debug.Log($"C_Move = {movePkt}");
        }
    }
    #endregion
}