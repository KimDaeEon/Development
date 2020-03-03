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

    // Start is called before the first frame update
    void Start()
    {
        if (startWeapon != null)
            EquipWeapon(startWeapon);
        else
        {
            Debug.Log("Start Weapon is NULl!!!!");
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
}
