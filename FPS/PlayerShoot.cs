using UnityEngine.Networking;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;




[RequireComponent(typeof(WeaponManager))]
public class PlayerShoot : NetworkBehaviour
{

    private const string PLAYER_TAG = "Player";

    [SerializeField] LayerMask mask;

    [SerializeField] private Camera cam;

    private PlayerWeapon currentWeapon;
    private WeaponManager weaponManager;
    private PlayerController playerController;

    private float shotDelay;
    public float remainShotDelay = 0f;

    private bool isLoad = false;
    private bool isRegister = false;


    void Start()
    {
        if (cam == null)
        {
            Debug.Log("There is no camera..");
            this.enabled = false;
        }

        playerController = GetComponent<PlayerController>();
        weaponManager = GetComponent<WeaponManager>();

    }

    private void Update()
    {
        currentWeapon = weaponManager.GetCurrentWeapon();
        if (currentWeapon == null)
        {
            return;
        }
        // Debug.Log("currentWeapon rate is " + currentWeapon.fireRate);
        shotDelay = 1f / currentWeapon.fireRate;
        if (currentWeapon.currentBullets < currentWeapon.maxBullets)
        {
            if (Input.GetKeyDown(KeyCode.R))
            {
                weaponManager.Reload();
                return;
            }
        }


        if (remainShotDelay > 0)
        {
            return;
        }

        if (currentWeapon.isOneBulletPerShoot)
        {
            if (Input.GetButtonDown("Fire1"))
            {
                Shoot();
                remainShotDelay = shotDelay;
            }
        }
        else
        {
            if (Input.GetButtonDown("Fire1"))
            {
                InvokeRepeating("Shoot", 0f, 1f / currentWeapon.fireRate);
            }
            else if (Input.GetButtonUp("Fire1"))
            {
                CancelInvoke("Shoot");
                remainShotDelay = shotDelay;
            }
        }

    }
    private void FixedUpdate()
    {
        remainShotDelay -= Time.fixedDeltaTime;
    }

    //It is called on the server when a player shoots.
    [Command]
    void CmdOnSHoot()
    {
        RpcOnShootEffect();
    }

    //It is called on all clients when we make shoot effect.
    [ClientRpc]
    void RpcOnShootEffect()
    {
        // Debug.Log("RpcOnShootEffect called!");
        WeaponGraphics currentGraphics = weaponManager.GetCurrentWeaponGraphics();
        currentGraphics.muzzleFlash.Play();
        currentGraphics.muzzleFlashSound.Play();

        playerController.cameraRotation += (currentWeapon.impactForce / 90);

    }



    //It is called on the server when we hit something
    //It takes in position and normal vector
    [Command]
    void CmdOnHit(Vector3 _pos, Vector3 _normal, GameObject hitObj)
    {
        RpcOnHitEffect(_pos, _normal, hitObj);
    }

    //It is called on all cilents
    //Here we can spawn OnHit effect
    [ClientRpc]
    void RpcOnHitEffect(Vector3 _pos, Vector3 _normal, GameObject hitObj)
    {
        GameObject _onHitEffect = Instantiate(weaponManager.GetCurrentWeaponGraphics().hitEffectPreafab, _pos, Quaternion.LookRotation(_normal));

        Destroy(_onHitEffect, 2f);
        if (hitObj != null) // if there is no networkIdentity on object, hitObj is null, make sure that networkIdentity is needed for object which will be interacted with other netwrok object.
        {
            if (hitObj.GetComponent<Rigidbody>() != null)
            {
                Debug.Log("Addforce activated to " + hitObj.name);
                hitObj.GetComponent<Rigidbody>().AddForce(-_normal * weaponManager.GetCurrentWeapon().impactForce);
            }
        }
    }

    [Client]
    void Shoot()
    {
        if (!isLocalPlayer || weaponManager.isReloading || currentWeapon == null)
        {
            return;
        }

        if (currentWeapon.currentBullets <= 0)
        {
            weaponManager.Reload();
            return;
        } //when player switch weapon while reloading, reloaidng doesn't occur, so this code is here.


        if (PauseMenu.IsPauseOn)
        {
            return;
        }

        CmdOnSHoot();
        currentWeapon.currentBullets--;

        if (currentWeapon.currentBullets <= 0)
        {
            weaponManager.Reload();
            return;
        }

        RaycastHit hit;
        if (Physics.Raycast(cam.transform.position, cam.transform.forward, out hit, currentWeapon.range, mask))
        {
            // If we hit something, call the onHit function on server
            CmdOnHit(hit.point, hit.normal, hit.transform.gameObject);

            if (hit.collider.tag == PLAYER_TAG)
            {
                CmdPlayerShot(hit.collider.name, currentWeapon.damage, transform.name); // hit.transform.name is also available
            }
        }


    }


    [Command]
    void CmdPlayerShot(string _playerID, int _damage, string _shooterName)
    {
        Debug.Log(_shooterName + "has shot " + _playerID);
        PlayerManager _player = GameManager.GetPlayer(_playerID);

        _player.RpcTakeDamage(_damage, _shooterName);
    }


}
