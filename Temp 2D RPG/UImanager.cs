using UnityEngine;
using System.Collections;
using UnityEngine.UI;
public class UImanager : MonoBehaviour {

    public Slider healthBar;
    public Text HPText;
    public PlayerHealthManager playerHealth; // 이런 참조도 가능하단 것 꼭 명심!

    private PlayerStat thePlayerStat;
    public Text levelText;

    private static bool playerExists;

	// Use this for initialization
	void Start () {
        if (!playerExists)
        {
            playerExists = true;
            DontDestroyOnLoad(transform.gameObject);
        }
        else
        {
            Destroy(gameObject);
        }

        thePlayerStat = GetComponent<PlayerStat>();
    }
	
	// Update is called once per frame
	void Update () {
        healthBar.maxValue = playerHealth.playerMaxHealth;
        healthBar.value = playerHealth.playerCurrentHealth;
        HPText.text = "" + "HP: " + healthBar.value + "/" + healthBar.maxValue;
        levelText.text = "Lv: " + thePlayerStat.currentLevel;
    }
}
