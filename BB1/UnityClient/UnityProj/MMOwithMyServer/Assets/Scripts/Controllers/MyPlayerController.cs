using Google.Protobuf.Protocol;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;
using static Define;

public class MyPlayerController : PlayerController
{
    bool _moveKeyPressed = false;
    public int WeaponDamage { get; private set; }
    public int ArmorDefence { get; private set; }

    protected override void Init()
    {
        base.Init();
        RefreshStat();
    }
    protected override void UpdateIdle()
    {
        if (_moveKeyPressed)
        {
            State = ActorState.Moving;
            return;
        }

        if (_coSkillCooltime == null && Input.GetKey(KeyCode.K))
        {
            // TODO: 스킬 사용 가능여부 체크 로직 추가 필요
            C_Skill skillPacket = new C_Skill() { Info = new SkillInfo() };
            skillPacket.Info.SkillId = 1;
            Managers.Network.Send(skillPacket);

            _coSkillCooltime = StartCoroutine("CoInputCooltime", 0.2f);
        }
        else if (_coSkillCooltime == null && Input.GetKey(KeyCode.L))
        {
            C_Skill skillPacket = new C_Skill() { Info = new SkillInfo() };
            skillPacket.Info.SkillId = 2;
            Managers.Network.Send(skillPacket);
            _coSkill = StartCoroutine("CoStartShootArrow");
        }
    }

    Coroutine _coSkillCooltime;
    IEnumerator CoInputCooltime(float time)
    {
        yield return new WaitForSeconds(time);
        _coSkillCooltime = null;
    }

    protected override void UpdateController()
    {
        GetUIKeyInput();

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

    void GetUIKeyInput()
    {
        if (Input.GetKeyDown(KeyCode.I))
        {
            UI_GameScene gameSceneUI = (UI_GameScene)Managers.UI.SceneUI;
            UI_Inventory invenUI = gameSceneUI.InvenUI;

            if (invenUI.gameObject.activeSelf)
            {
                invenUI.gameObject.SetActive(false);
            }
            else
            {
                invenUI.gameObject.SetActive(true);
                invenUI.RefreshUI();
            }
        }

        else if (Input.GetKeyDown(KeyCode.C))
        {
            UI_GameScene gameSceneUI = (UI_GameScene)Managers.UI.SceneUI;
            UI_Stat statUI = gameSceneUI.StatUI;

            if (statUI.gameObject.activeSelf)
            {
                statUI.gameObject.SetActive(false);
            }
            else
            {
                statUI.gameObject.SetActive(true);
                statUI.RefreshUI();
            }
        }
    }

    void GetDirectionInput()
    {
        _moveKeyPressed = true;

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
            _moveKeyPressed = false;
        }
    }

    private void LateUpdate()
    {
        Camera.main.transform.position = new Vector3(transform.position.x, transform.position.y, -10);
    }

    protected override void MoveToNextPosition()
    {
        if (_moveKeyPressed == false)
        {
            State = ActorState.Idle;
            SendMovePacketIfUpdated();
            return;
        }

        Vector3Int destPos = CellPos;

        switch (Dir)
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

        if (Managers.Map.CanGo(destPos))
        {
            if (Managers.Object.FindActorByCellPos(destPos) == null)
            {
                CellPos = destPos;
            }
        }

        SendMovePacketIfUpdated();
    }

    protected override void SendMovePacketIfUpdated()
    {
        if (_updated)
        {
            C_Move movePacket = new C_Move();
            movePacket.PosInfo = PosInfo;

            Managers.Network.Send(movePacket);
            _updated = true;
        }
    }

    public void RefreshStat()
    {
        WeaponDamage = 0;
        ArmorDefence = 0;

        foreach (Item item in Managers.ItemInventory.Items.Values)
        {
            if (item.Equipped == false)
            {
                continue;
            }

            switch (item.ItemType)
            {
                case ItemType.Weapon:
                    WeaponDamage += ((WeaponItem)item).Damage;
                    break;

                case ItemType.Armor:
                    ArmorDefence += ((ArmorItem)item).Defence;
                    break;
            }
        }
    }
}
