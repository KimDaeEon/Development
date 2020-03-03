using UnityEngine;
using System.Collections;

public class EnemyHealthManager : MonoBehaviour {
    public int MaxHealth;
    public int CurrentHealth;
    public GameObject BurstEffect;

	private Animator thisAnimator;
	public bool isThisBoss;
	private float ftime = 0;

    private PlayerStat thePlayerStat;
	public int expOfEnemey;

    // Use this for initialization
    void Start()
    {
		CurrentHealth = MaxHealth;
    }

    // Update is called once per frame
    void Update()
    {
		thePlayerStat = FindObjectOfType<PlayerStat>();

        if (CurrentHealth <= 0)
        {
			thePlayerStat = FindObjectOfType<PlayerStat>();

			Destroy(gameObject);
			thePlayerStat.AddExperience(expOfEnemey);

			thisAnimator = thePlayerStat.transform.parent.gameObject.GetComponent<Animator> ();

			if (isThisBoss == true) {
					thisAnimator = thePlayerStat.transform.parent.gameObject.GetComponent<Animator> ();
					thisAnimator.SetBool ("Skill", false);
					thisAnimator.SetBool ("Attack", false);
					
					thePlayerStat.transform.parent.gameObject.GetComponent<PlayerHealthManager> ().SetMaxHealth();
					Application.LoadLevel ("Game_Win");
				}
			}

     }
  

    public void HurtEnemy(int damageToGive)
    {
        CurrentHealth -= damageToGive;
    }

    public void SetMaxHealth()
    {
        CurrentHealth = MaxHealth;
    }
}
