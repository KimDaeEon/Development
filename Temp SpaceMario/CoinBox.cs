using UnityEngine;
using System.Collections;

public class CoinBox : MonoBehaviour {
	public GameObject poppedStatePrefab;

	void CoinPop(){
		poppedStatePrefab.SetActive (true);
		this.gameObject.SetActive (false);
	}

	void OnTriggerEnter2D(Collider2D collider){
		Vector3 heading = this.transform.position - collider.gameObject.transform.position;

		float distance = heading.magnitude;
		Vector3 direction = heading / distance; // make it unit vector


		if ((direction.x < 0.1 && direction.x > -1.1) &&
		   (direction.y < 1.1 && direction.y > 0.4) &&
		   (collider.tag == "Player")) {
			CoinPop ();
		}

	}
}
