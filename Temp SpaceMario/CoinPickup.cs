using UnityEngine;
using System.Collections;

public class CoinPickup : MonoBehaviour {

	public int coinValue = 1;
	public AudioClip coinCollectedSound;

	// Use this for initialization
	void OnTriggerEnter2D(Collider2D collider){
		if (collider.tag == "Player") {
			PlayerStats stats = collider.gameObject.GetComponent<PlayerStats>(); // get PlayerStats
			stats.CollectCoin (this.coinValue);
			AudioSource.PlayClipAtPoint (coinCollectedSound, collider.transform.position);
			Destroy (this.gameObject);
		}

	}
}
