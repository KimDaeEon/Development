using UnityEngine;
using System.Collections;

public class PlayerStat : MonoBehaviour {
    public int currentLevel;

    public int currentExp;

    public int[] toLevelUp;

	public HurtEnemy Sword;
	public HurtEnemy Swing;


	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	    if(currentExp >= toLevelUp[currentLevel])
        {
            currentLevel += 1;
			this.GetComponentInParent<PlayerHealthManager> ().playerMaxHealth += 50;
			this.GetComponentInParent<PlayerHealthManager> ().playerCurrentHealth = this.GetComponentInParent<PlayerHealthManager> ().playerMaxHealth;

			Sword.damageToGive += 20;
			Swing.damageToGive += 60;

		}
	}

    public void AddExperience(int exp)
    {
        currentExp += exp;
    }
}
