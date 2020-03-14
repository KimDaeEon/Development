using UnityEngine.Networking;
using UnityEngine;
using System.Collections;
public class WeaponManager : NetworkBehaviour
{
    [SerializeField] private string weaponLayerName = "Weapon";
    public Transform weaponHolder;
    [SerializeField] private PlayerWeapon startWeapon;

    public PlayerWeapon currentWeapon;
    private WeaponGraphics currentWeaponGraphics;

    public bool isReloading = false;

    public int maxWeaponCount = 4;

    //for weaponSwitching
    [SyncVar] public int selectedWeapon = 0;
    public int previousSelectedWeapon = 0;


    // Start is called before the first frame update
    void Start()
    {
        if (startWeapon != null)
            EquipWeapon(startWeapon);
        else
        {
            Debug.Log("Start Weapon is NULl!!!!");
        }
        SelectWeapon();
    }

    void Update()
    {

        if (isLocalPlayer)
        {
            if (Input.GetAxis("Mouse ScrollWheel") > 0f)
            {
                if (selectedWeapon >= weaponHolder.childCount - 1) CmdSwitchWeaponBroadCast(0);
                else CmdSwitchWeaponBroadCast(selectedWeapon + 1);
            }
            if (Input.GetAxis("Mouse ScrollWheel") < 0f)
            {
                if (selectedWeapon <= 0) CmdSwitchWeaponBroadCast(weaponHolder.childCount - 1);
                else CmdSwitchWeaponBroadCast(selectedWeapon - 1);
            }

            if (Input.GetKeyDown(KeyCode.Alpha1))
            {
                CmdSwitchWeaponBroadCast(0);
            }
            if (Input.GetKeyDown(KeyCode.Alpha2) && weaponHolder.childCount >= 2)
            {
                CmdSwitchWeaponBroadCast(1);
            }
            if (Input.GetKeyDown(KeyCode.Alpha3) && weaponHolder.childCount >= 3)
            {
                CmdSwitchWeaponBroadCast(2);
            }
            if (Input.GetKeyDown(KeyCode.Alpha4) && weaponHolder.childCount >= 4)
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


    public PlayerWeapon GetCurrentWeapon()
    {
        return currentWeapon;
    }

    public WeaponGraphics GetCurrentWeaponGraphics()
    {
        return currentWeaponGraphics;
    }

    public void EquipWeapon(PlayerWeapon _weapon)
    {
        currentWeapon = _weapon;

        //GameObject _weaponIns = Instantiate(_weapon.graphics, weaponHolder.position, weaponHolder.rotation);
        //_weaponIns.transform.SetParent(weaponHolder);

        currentWeaponGraphics = GetComponentInChildren<WeaponGraphics>();
        if (currentWeaponGraphics == null)
        {
            Debug.LogError("No WeaponGraphics component on the weapon object: " + currentWeapon.name);
        }

        if (isLocalPlayer)
        {
            Utility.SetLayerRecursively(currentWeapon.gameObject, LayerMask.NameToLayer(weaponLayerName));
        }
    }

    public void GetWeapon(GameObject _obj)
    {

        GameObject _weaponIns = GameObject.Instantiate(_obj);
        _weaponIns.GetComponent<PlayerWeapon>().animator.enabled = false;

        _weaponIns.transform.position = weaponHolder.position;
        _weaponIns.transform.rotation = weaponHolder.rotation;
        _weaponIns.transform.SetParent(weaponHolder);

        _weaponIns.GetComponent<PlayerWeapon>().animator.enabled = true;

        if (isLocalPlayer)
        {
            Utility.SetLayerRecursively(_weaponIns, LayerMask.NameToLayer(weaponLayerName));
        }

        if (currentWeapon != null) // if i equip some weapon, disable it
        {
            currentWeapon.gameObject.SetActive(false);
        }

        //Equip New Weapon
        EquipWeapon(_weaponIns.GetComponent<PlayerWeapon>());

    }





    public void Reload()
    {
        if (isReloading) return;
        StartCoroutine(ReloadCoroutine());
    }

    private IEnumerator ReloadCoroutine()
    {
        Debug.Log("Reloading...");
        isReloading = true;

        CmdOnReload();

        yield return new WaitForSeconds(currentWeapon.reloadTime);

        currentWeapon.currentBullets = currentWeapon.maxBullets;

        isReloading = false;
    }

    [Command]
    void CmdOnReload()
    {
        RpcOnReload();
    }

    [ClientRpc]
    void RpcOnReload()
    {
        Animator animator = currentWeaponGraphics.GetComponent<Animator>();
        if (animator != null)
        {
            animator.SetTrigger("Reloading");
        }
    }

    public void OnScopedBroadCast()
    {
        CmdOnScopedBroadCast();
    }
    [Command]
    public void CmdOnScopedBroadCast()
    {
        RpcOnScopedBroadCast();
    }

    [ClientRpc]
    public void RpcOnScopedBroadCast()
    {
        Animator animator = currentWeaponGraphics.GetComponent<Animator>(); ;
        if (animator != null)
        {
            animator.SetBool("Scoped", true);
            Debug.Log(this.name + "'s Scope Animation is set to True!");
        }
        else
        {
            Debug.Log(this.name + "'s animator is null!");
        }
    }


    public void OnUnScopedBroadCast()
    {
        CmdOnUnScopedBroadCast();
    }
    [Command]
    public void CmdOnUnScopedBroadCast()
    {
        RpcOnUnScopedBroadCast();
    }

    [ClientRpc]
    public void RpcOnUnScopedBroadCast()
    {
        Animator animator = currentWeaponGraphics.GetComponent<Animator>();
        animator.SetBool("Scoped", false);
    }


    [Command]
    void CmdSwitchWeaponBroadCast(int _selectedWeapon)
    {
        selectedWeapon = _selectedWeapon;
    }


    public void SelectWeapon()
    {
        int i = 0;
        foreach (Transform weapon in weaponHolder)
        {
            if (i == selectedWeapon)
            {
                weapon.gameObject.SetActive(true);
                this.EquipWeapon(weapon.GetComponent<PlayerWeapon>());
            }
            else weapon.gameObject.SetActive(false);

            i++;
        }
    }
}
