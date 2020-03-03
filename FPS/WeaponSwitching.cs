using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class WeaponSwitching : NetworkBehaviour
{

    [SyncVar] public int selectedWeapon = 0;
    public int previousSelectedWeapon = 0;
    public WeaponManager weaponManager;
    public Transform weaponHolderTransform;
    void Start()
    {
        SelectWeapon();
    }

    // Update is called once per frame
    void Update()
    {

        if (isLocalPlayer)
        {
            if (Input.GetAxis("Mouse ScrollWheel") > 0f)
            {
                if (selectedWeapon >= weaponHolderTransform.childCount - 1) CmdSwitchWeaponBroadCast(0);
                else CmdSwitchWeaponBroadCast(selectedWeapon + 1);
            }
            if (Input.GetAxis("Mouse ScrollWheel") < 0f)
            {
                if (selectedWeapon <= 0) CmdSwitchWeaponBroadCast(weaponHolderTransform.childCount - 1);
                else CmdSwitchWeaponBroadCast(selectedWeapon - 1);
            }

            if (Input.GetKeyDown(KeyCode.Alpha1))
            {
                CmdSwitchWeaponBroadCast(0);
            }
            if (Input.GetKeyDown(KeyCode.Alpha2) && weaponHolderTransform.childCount >= 2)
            {
                CmdSwitchWeaponBroadCast(1);
            }
            if (Input.GetKeyDown(KeyCode.Alpha3) && weaponHolderTransform.childCount >= 3)
            {
                CmdSwitchWeaponBroadCast(2);
            }
            if (Input.GetKeyDown(KeyCode.Alpha4) && weaponHolderTransform.childCount >= 4)
            {
                CmdSwitchWeaponBroadCast(3);
            }
        }



        if (previousSelectedWeapon != selectedWeapon)
        {
            SelectWeapon();
            previousSelectedWeapon = selectedWeapon;
        }
    }

    [Command]
    void CmdSwitchWeaponBroadCast(int _selectedWeapon)
    {
        selectedWeapon = _selectedWeapon;
    }

 
    public void SelectWeapon()
    {
        int i = 0;
        foreach (Transform weapon in weaponHolderTransform)
        {
            if (i == selectedWeapon)
            {
                weapon.gameObject.SetActive(true);
                weaponManager.EquipWeapon(weapon.GetComponent<PlayerWeapon>());
            }
            else weapon.gameObject.SetActive(false);

            i++;
        }
    }
}
