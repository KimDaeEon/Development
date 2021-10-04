using UnityEngine;
using System.Collections;

public class SpawnTrigger : MonoBehaviour {

	public GameObject[] gameObjects;
	public bool isTriggered = false;

	void OnTriggerEnter2D(Collider2D collider){
		if(collider.tag == "Player"){
			isTriggered = true;
			foreach(GameObject gameObject in gameObjects){
				gameObject.SetActive (true);
			}
		}
	}

}
