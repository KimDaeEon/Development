using UnityEngine;
using System.Collections;

public class LoadNewArea : MonoBehaviour {
    public string levelToLoad;

    public string exitPoint;

    private PlayerContoller thePlayer;

	// Use this for initialization
	void Start () {
        thePlayer = FindObjectOfType<PlayerContoller>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnTriggerEnter2D(Collider2D other)
    {
        if(other.gameObject.tag == "Player")
        {
			other.GetComponent<Animator>().SetBool ("Skill", false);
            Application.LoadLevel(levelToLoad);
            thePlayer.startPoint = exitPoint;
        }
    }
}
