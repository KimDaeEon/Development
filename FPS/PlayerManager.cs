using UnityEngine;
using UnityEngine.Networking;
using System.Collections;

[RequireComponent(typeof(PlayerSetup))]
public class PlayerManager : NetworkBehaviour
{
    [SyncVar] public int kills = 0;
    [SyncVar] public int deaths = 0;


    [SyncVar]
    public string userName = "Loading...";
    [SyncVar]
    public UserData currentUserInfo;

    DBcontroller dbController;
    [SyncVar]
    private bool _isDead = false;
    public bool isDead
    {
        get { return _isDead; }
        protected set { _isDead = value; }
    }


    [SerializeField] private int maxHealth = 100;
    [SyncVar] private int currentHealth; //sync makes this variable synchronized in clients
    private bool isFirstSetup = true;

    #region Death
    [SerializeField] private Behaviour[] disableOnDeath;

    private bool[] wasEnabled;
    [SerializeField] private GameObject[] disabledGameObjectsOnDeath;
    [SerializeField] private GameObject deathEffect;
    #endregion

    [SerializeField] WeaponManager weaponManager;


    #region Spawn
    [SerializeField] private GameObject spawnEffect;
    #endregion


    // Local function call Cmd function -> Server recevies Cmd funtion-> Server transfer Rpc function to all clients.


    public void SetupPlayer()
    {
        CmdBroadCastNewPlayerSetup();
    }

    [Command]
    private void CmdBroadCastNewPlayerSetup()
    {
        RpcSetupPlayerOnAllClients();
    }

    [ClientRpc]
    private void RpcSetupPlayerOnAllClients()
    {
        if (isLocalPlayer)
        {   //Switch scene canera to player camera
            GameManager.instance.SetSceneCameraActive(false); // sceneCamera turns off. player camera turns on
            GetComponent<PlayerSetup>().playerUIInstance.SetActive(true); // UI will be seen after respawn
        }

        if (isFirstSetup)
        {
            wasEnabled = new bool[disableOnDeath.Length];
            for (int i = 0; i < wasEnabled.Length; i++)
            {
                wasEnabled[i] = disableOnDeath[i].enabled;
            }
            isFirstSetup = false;
        }
        SetDefaults();
    }


    [ClientRpc]
    public void RpcTakeDamage(int _damage, string _shooterName)
    {
        if (isDead)
        {
            return;
        }
        currentHealth -= _damage;
        Debug.Log(_shooterName + "attacked!! " + transform.name + " now has " + currentHealth + " HP");

        if (currentHealth <= 0)
        {
            Die(_shooterName);
        }
    }

    public void SetDefaults()
    {
        Debug.Log(transform.name + " setDefaults activated!");
        //this.transform.name = this.currentUserData.userID;

        isDead = false;
        currentHealth = maxHealth;

        PlayerWeapon currentWeapon = weaponManager.GetCurrentWeapon();
        if (currentWeapon != null) currentWeapon.currentBullets = currentWeapon.maxBullets;


        //this.currentUserData = GameManager.GetPlayer(this.gameObject.name).currentUserData;

        //Enable the components
        for (int i = 0; i < disableOnDeath.Length; i++)
        {
            disableOnDeath[i].enabled = wasEnabled[i];
            //In client not all players' behavour enabled is 'true'
        }

        //Enable the gameObjects
        for (int i = 0; i < disabledGameObjectsOnDeath.Length; i++)
        {
            disabledGameObjectsOnDeath[i].SetActive(true);
        }

        //Enable the collider
        Collider _col = GetComponent<Collider>();
        if (_col != null) _col.enabled = true;

        //Create spawn effect
        GameObject _spawnEffect = Instantiate(spawnEffect, transform.position, Quaternion.identity);

        Destroy(_spawnEffect, 3f);
    }


    private void Die(string _shooterName)
    {
        isDead = true;
        deaths++;

        // IF player zooms in sniper scope, it disappears
        PlayerWeapon currentWeapon = weaponManager.GetCurrentWeapon();
        if (currentWeapon != null)
        {
            Scope tempScope = currentWeapon.GetComponent<Scope>();
            if (tempScope != null)
            {
                if (isLocalPlayer)
                {
                    tempScope.isScoped = false;
                    tempScope.animator.SetBool("Scoped", false); // scope animation unactiv
                    tempScope.onUnScoped();
                    currentWeapon.transform.localPosition = Vector3.zero;
                    currentWeapon.transform.localRotation = Quaternion.identity;

                    Debug.Log(this.name + "'s Scope Disappear activated!");
                }
                else
                {
                    Debug.Log("it is scoping, but it is not local player!");
                }

            }
            else
            {
                Debug.Log("Scope is NULL!");
            }
        }



        PlayerManager killer = GameManager.GetPlayer(_shooterName);
        if (killer != null)
        {
            killer.kills++;
            GameManager.instance.onPlayerKilledCallBack.Invoke(userName, killer.userName);
        }
        else
        {
            Debug.Log("Killer is NULL!!");
        }


        //Disable Components when player died
        Debug.Log(transform.name + " is DEAD!");



        for (int i = 0; i < disableOnDeath.Length; i++)
        {
            disableOnDeath[i].enabled = false;
        }

        for (int i = 0; i < disabledGameObjectsOnDeath.Length; i++)
        {
            disabledGameObjectsOnDeath[i].SetActive(false);
        }

        //Disable the collider when player died
        Collider _col = GetComponent<Collider>();
        if (_col != null) _col.enabled = false;

        //Spawn a death effect
        GameObject _deathEffect = Instantiate(deathEffect, transform.position, Quaternion.identity);
        Debug.Log(this.name + "'s Death Effect activated!");
        Destroy(_deathEffect, 3f);
        StartCoroutine(Respawn());

        //Switch to player canera to scene camera
        if (isLocalPlayer)
        {
            GameManager.instance.SetSceneCameraActive(true);
            GetComponent<PlayerSetup>().playerUIInstance.SetActive(false); // UI disappears while player is dead
        }
    }
    private IEnumerator Respawn()
    {
        Debug.Log(transform.name + " Respawn!");
        yield return new WaitForSeconds(GameManager.instance.matchSettings.respawnTime);

        //  Debug.Log("After waitForSeconds");
        Transform _startPoint = NetworkManager.singleton.GetStartPosition();
        transform.position = _startPoint.position;
        transform.rotation = _startPoint.rotation;


        yield return new WaitForSeconds(0.2f);
        Debug.Log("After yield WaitForSeconds 0.2f");

        SetupPlayer();

        Debug.Log(transform.name + " respawned");

    }

    public void UpdateCurrentUserInfo()
    {
        if (isLocalPlayer)
        {
            CurrentUserInfo.info.killCount += kills;
            CurrentUserInfo.info.deathCount += deaths;
            kills = 0;
            deaths = 0;
            print("Current Local UserInfo is updated!");
        }
    }

    public float GetHealthPercentage()
    {
        return (float)currentHealth / maxHealth;
    }

    private void Update()
    {
        if (!isLocalPlayer)
        {
            // Debug.Log(this.gameObject.name + " is not LocalPlayer!");
            return;
        }

        if (Input.GetKeyDown(KeyCode.K))
        {
            //  Debug.Log(this.gameObject.name + " pressed K key!");
            if (isServer) RpcTakeDamage(50, this.name);
            else
            {
                CmdKillMySelf();
            }
        }

    }


    [Command]
    void CmdKillMySelf()
    {

        Debug.Log(this.gameObject.name + " pressed K key!");
        RpcTakeDamage(50, this.name);

    }


    private void OnTriggerEnter(Collider other)
    {
        if (isLocalPlayer)
        {
            if (other.gameObject.tag == "WeaponPick")
            {
                WeaponPick weaponPick = other.GetComponent<WeaponPick>();
                weaponPick.weaponPickCanvas.SetActive(true);
                weaponPick.interactingObject = this.gameObject;
            }
        }

    }



    private void OnTriggerExit(Collider other)
    {
        if (isLocalPlayer)
        {
            if (other.gameObject.tag == "WeaponPick")
            {
                WeaponPick weaponPick = other.GetComponent<WeaponPick>();
                weaponPick.weaponPickCanvas.SetActive(false);
                weaponPick.interactingObject = null;
            }
        }

    }

    public void PickWeapon(GameObject _obj)
    {
        //Debug.Log("local activated! "+this.name + "destroy " + _obj.name);
        CmdPickWeapon(_obj);
    }
    [Command]
    public void CmdPickWeapon(GameObject _obj)
    {
        //Debug.Log("Command activated! " + this.name + "destroy " + _obj.name);
        WeaponPick tempWeaponPick = _obj.GetComponentInChildren<WeaponPick>();
        //if(tempWeaponPick == null)
        //{
        //    Debug.Log("Command's tempWeaponPick is null!!");
        //}
        //else
        //{
        //    Debug.Log("Command's Pefab name is" + tempWeaponPick.weaponPrefab.name + " damage is " + tempWeaponPick.weaponPrefab.GetComponent<PlayerWeapon>().damage);
        //}
        RpcPickWeapon(_obj);
    }
    [ClientRpc]
    public void RpcPickWeapon(GameObject _obj)
    {
        if (weaponManager.weaponHolder.transform.childCount < weaponManager.maxWeaponCount)
        {
            //Debug.Log("Rpc activated! " + this.name + "destroy " + _obj.name);
            WeaponPick tempWeaponPick = _obj.GetComponentInChildren<WeaponPick>();
            //if (tempWeaponPick == null)
            //{
            //    Debug.Log("Rpc's tempWeaponPick is null!!");
            //}
            //else
            //{
            //    Debug.Log("Rpc's Pefab name is" + tempWeaponPick.weaponPrefab.name + " damage is " + tempWeaponPick.weaponPrefab.GetComponent<PlayerWeapon>().damage);
            //}

            //Debug.Log(this.name + "Rpc Get Weapon activated!");

            this.gameObject.GetComponent<WeaponManager>().GetWeapon(tempWeaponPick.weaponPrefab);

            Destroy(_obj);
        }
        else
        {
            Debug.Log("Cannot Hold More Weapon!");
        }

    }

    //public void SetString(string _name1, string _name2)
    //{
    //    Debug.Log("SetString Client Activated!");
    //    temp1 = "Client";
    //    Debug.Log("ClientFunc temp1 is " + temp1);
    //    CmdSetString(_name1, _name2);
    //}
    //[Command]
    //public void CmdSetString(string _name1, string _name2)
    //{
    //    Debug.Log("SetString Command Activated!");
    //    temp1 = "Command";
    //    Debug.Log("CommandFunc temp1 is " + temp1);
    //    RpcSetString(_name1, _name2);
    //}
    //[ClientRpc]
    //public void RpcSetString(string _name1, string _name2)
    //{
    //    temp1 = "Rpc";
    //    Debug.Log("RpcFunc temp1 is " + temp1);
    //}

}
