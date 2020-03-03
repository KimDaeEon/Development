using UnityEngine;
using System.Collections;

public class EnemySlime : EnemyController {

	void FixedUpdate(){
		if (isFacingRight == true) {
			this.GetComponent<Rigidbody2D> ().velocity = new Vector2 (maxSpeed, this.GetComponent<Rigidbody2D> ().velocity.y);		
		} else {
			this.GetComponent<Rigidbody2D> ().velocity = new Vector2 (-1 *maxSpeed, this.GetComponent<Rigidbody2D> ().velocity.y);		
		}
	}//fixed update

	void OnTriggerEnter2D(Collider2D collider){
		if (collider.tag == "Wall") {
			Flip ();
		} else if (collider.tag == "Enemy") {
			Flip ();
		}
	}
}
