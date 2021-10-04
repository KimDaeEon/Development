using UnityEngine;
using UnityEngine.Networking;
using System.Collections;


public class PlayerWeapon : MonoBehaviour
{
    public string weaponName = "Temp";
    public int damage = 10;
    public float range = 100.0f;
    // Start is called before the first frame update

    public float fireRate = 0f; // It indicates how many times this weapon can shoot in one second when mouse button is pressed.
                                // 0 means It can only shoot once on a click.
    public bool isOneBulletPerShoot;
    public int maxBullets = 20;
    [HideInInspector] public int currentBullets;

    public GameObject graphics;

    public float impactForce = 30f;

    public float reloadTime = 1f;
    public Animator animator;
    private void Start()
    {
        currentBullets = maxBullets;
    }

}
