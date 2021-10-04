using UnityEngine;
using System.Collections;

public class PlayerHealthManager : MonoBehaviour {
    public int playerMaxHealth = 300;
    public int playerCurrentHealth;
	public Collider2D swordCollider;
    public bool reloading;
	private float ftime = 0;
	public Animator thisAnimator;

    // Use this for initialization
    void Start () {
		SetMaxHealth ();
	}
	
	// Update is called once per frame
	void Update () {
	    if(playerCurrentHealth <= 0)
        {
			thisAnimator.SetBool ("Skill", false);
			thisAnimator.SetBool ("Attack",false);
			swordCollider.enabled = false;
			Application.LoadLevel ("Game_Over");
			SetMaxHealth ();
		}

      }

    public void HurtPlayer(int damageToGive)
    {
        playerCurrentHealth -= damageToGive;
    }

    public void SetMaxHealth()
    {
        playerCurrentHealth = playerMaxHealth;
    }
}
