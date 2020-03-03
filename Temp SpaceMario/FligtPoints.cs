using UnityEngine;
using System.Collections;

public class FligtPoints : MonoBehaviour {
	public GameObject waypointA;
	public GameObject waypointB;
	public float speed = 1;
	private bool directionAB = true;
	public bool ChangeFacing = false;

	void FixedUpdate () {
		if ( (this.transform.position == waypointA.transform.position && directionAB == false) || 
			 (this.transform.position == waypointB.transform.position && directionAB == true)) {
			directionAB = !directionAB;
			if (this.ChangeFacing == true) {
				this.gameObject.GetComponent<EnemyController> ().Flip ();
			}
		}

		if (directionAB == true) {
			this.transform.position = Vector3.MoveTowards (this.transform.position, waypointB.transform.position, Time.fixedDeltaTime*speed);
		} else {
			this.transform.position = Vector3.MoveTowards (this.transform.position, waypointA.transform.position, Time.fixedDeltaTime*speed);
		}
	}
}
