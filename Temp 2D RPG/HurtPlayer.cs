using UnityEngine;
using System.Collections;

public class HurtPlayer : MonoBehaviour {
    public int damageToGive;

    public float waitToReaload;
    private static bool reloading;
    private GameObject thePlayer;

    // Use this for initialization
    void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        if (reloading)
        {
            waitToReaload -= Time.deltaTime;
            if (waitToReaload <= 0)
            {
                Application.LoadLevel(Application.loadedLevel);
                thePlayer.SetActive(true);
                reloading = false;
                thePlayer.gameObject.GetComponent<PlayerHealthManager>().reloading = false;
            }
        }

    }

    void OnCollisionEnter2D(Collision2D other)
    {
        if(other.gameObject.name == "Player")
        {
            thePlayer = other.gameObject;
            thePlayer.gameObject.GetComponent<PlayerHealthManager>().HurtPlayer(damageToGive);
            reloading = thePlayer.gameObject.GetComponent<PlayerHealthManager>().reloading;

        }
    }
}
