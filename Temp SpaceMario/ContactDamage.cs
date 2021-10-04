using UnityEngine;
using System.Collections;

public class ContactDamage : MonoBehaviour {

	public int damage = 1;
	public bool playHitReaction = false;

	void OnTriggerEnter2D(Collider2D collider){
		if (collider.tag == "Player") {
			PlayerStats stats = collider.gameObject.GetComponent<PlayerStats> ();
			stats.TakeDamage (this.damage, this.playHitReaction);
		}
	}	
}
