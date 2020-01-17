using UnityEngine.Networking;
using UnityEngine;



public class PlayerShoot : NetworkBehaviour
{
    public PlayerWeapon weapon;
    private const string PLAYER_TAG = "Player";
    [SerializeField]LayerMask mask;

    [SerializeField] private Camera cam;

    void Start()
    {
        if(cam == null)
        {
            Debug.Log("There is no camera..");
            this.enabled = false;
        }    
    }

    private void Update()
    {
        if (Input.GetButtonDown("Fire1"))
        {
            Shoot();
        }
    }

    [Client]
    void Shoot(){
        RaycastHit hit;
        if (Physics.Raycast(cam.transform.position, cam.transform.forward, out hit, weapon.range, mask)){
            if (hit.collider.tag == PLAYER_TAG)
            {
                CmdPlayerShot(hit.collider.name,weapon.damage); // hit.transform.name is also available
            }      
        }

    }


    [Command]
    void CmdPlayerShot(string _playerID, int _damage)
    {
        Debug.Log(_playerID + " has been shot!");
        PlayerManager _player = GameManager.GetPlayer(_playerID);

        _player.RpcTakeDamage(_damage);
    }
  
}
