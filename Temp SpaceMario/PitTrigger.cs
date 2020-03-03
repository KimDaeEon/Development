using UnityEngine;
using System.Collections;

public class PitTrigger : MonoBehaviour {
	GameObject GetNearestActiveCheckPoint(){
		GameObject[] checkpoints = GameObject.FindGameObjectsWithTag ("Checkpoint");

		GameObject nearestCheckpoint = null;
		float shortestDistance = Mathf.Infinity;

		foreach (GameObject checkpoint in checkpoints) {
			Vector3 checkpointPosition = checkpoint.transform.position;
			float distance = (checkpointPosition - transform.position).sqrMagnitude;

			CheckpointTrigger trigger = checkpoint.GetComponent<CheckpointTrigger> ();

			if (distance < shortestDistance && trigger.isTriggered == true) {
				nearestCheckpoint = checkpoint;
				shortestDistance = distance;
			}

		}

		return nearestCheckpoint;
	}

	void OnTriggerEnter2D(Collider2D collider){
		if (collider.tag == "Player") {
			GameObject trigger = GetNearestActiveCheckPoint ();
			if (trigger != null && collider.gameObject.GetComponent<PlayerStats>().isDead == false) {
				collider.transform.position = trigger.transform.position;
			} else {
				Debug.LogError ("No Valid Checkpoint was found!");
			}//else
		} else if (collider.tag == "GroundCheck") {
			// Do Nothing
		} else {
			Destroy (collider.gameObject);
		}
	}
}
