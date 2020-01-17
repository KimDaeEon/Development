using UnityEngine;
using UnityEngine.Networking;
using System.Collections;

public class PlayerManager : NetworkBehaviour
{
    [SyncVar]
    private bool _isDead = false;
    public bool isDead
    {
        get { return _isDead; }
        protected set { _isDead = value; }
    }


    [SerializeField]private int maxHealth = 100;
    [SyncVar]private int currentHealth; //sync makes this variable synchronized in clients

    [SerializeField] private Behaviour[] disableOnDeath;
    private bool[] wasEnabled;

    public void SetUp()
    {
        wasEnabled = new bool[disableOnDeath.Length];
        for (int i = 0; i < wasEnabled.Length; i++)
        {
            wasEnabled[i] = disableOnDeath[i].enabled;
        }
        SetDefaults();     
    }

    public void SetDefaults()
    {
        isDead = false;
        currentHealth = maxHealth;

        for (int i = 0; i < disableOnDeath.Length; i++)
        {
            disableOnDeath[i].enabled = wasEnabled[i];
            //In client not all players' behavour enabled is 'true'
        }

        Collider _col = GetComponent<Collider>();
        if (_col != null) _col.enabled = true;
    }

    [ClientRpc]
    public void RpcTakeDamage(int _damage)
    {
        if (isDead)
        {
            return;
        }
        currentHealth -= _damage;
        Debug.Log(transform.name + " now has " + currentHealth + " HP");
    
        if(currentHealth <= 0)
        {
            Die();
        }
    }

    private void Die()
    {
        isDead = true;

        //Disable Components

        Debug.Log(transform.name + " is DEAD!");

        for (int i = 0; i < disableOnDeath.Length; i++)
        {
            disableOnDeath[i].enabled = false;
        }

        Collider _col = GetComponent<Collider>();
        if (_col != null) _col.enabled = false;

        StartCoroutine(Respawn());
    }
    private IEnumerator Respawn()
    {
        yield return new WaitForSeconds(GameManager.instance.matchSettings.respawnTime);

        SetDefaults();
        Transform _startPoint = NetworkManager.singleton.GetStartPosition();
        transform.position = _startPoint.position;
        transform.rotation = _startPoint.rotation;

        Debug.Log(transform.name + " respawned");
    
    }
    //private void Update()
    //{
    //    if (!isLocalPlayer){
    //        return;
    //    }

    //    if (Input.GetKeyDown(KeyCode.K))
    //    {
    //        RpcTakeDamage(999);
    //    }
    //}
}
 