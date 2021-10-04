using UnityEngine;
using System.Collections;

public class VictoryTrigger : MonoBehaviour {
	
	public float delay = 1;

	private float timeElapsed;
	private bool isTriggered;

	// Update is called once per frame
	void Update () {
		if (isTriggered == true) {
			timeElapsed = timeElapsed + Time.deltaTime;
		}
		if (timeElapsed > delay) {
			Application.LoadLevel ("Game_Win");
		}
	}

	void OnTriggerEnter2D(Collider2D collider){
		if (collider.tag == "Player" && collider.isTrigger == false) {
			timeElapsed = 0;
			this.isTriggered = true;

			collider.GetComponent<PlayerController> ().enabled = false;
			collider.GetComponent<Rigidbody2D>().velocity = new Vector2 (0, 0);
			collider.GetComponent<Animator> ().SetFloat (Constants.animSpeed, 0);
			PlayerPrefs.SetInt (Constants.PREF_COINS, collider.GetComponent<PlayerStats> ().coinsCollected);

		}
	}
}
