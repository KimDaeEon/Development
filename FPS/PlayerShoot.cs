using UnityEngine.Networking;
using UnityEngine;



public class PlayerShoot : NetworkBehaviour
{
    public PlayerWeapon weapons;

    [SerializeField] private Camera cam;

    void Start()
    {
        if(cam == null)
        {
            Debug.Log("There is no camera..");
            this.enabled = false;
        }    
    }
}
