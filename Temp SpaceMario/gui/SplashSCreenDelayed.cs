using UnityEngine;
using System.Collections;

public class SplashSCreenDelayed : MonoBehaviour {

	public float delayTime = 5.0f;

	void Start () {
		StartCoroutine ("Delay");
	}

	IEnumerator Delay(){
		yield return new WaitForSeconds (delayTime);
		Application.LoadLevel ("1st_Level");
		Debug.Log ("Time's Up!");
	}
	// Update is called once per frame
	void Update () {
		if (Input.anyKeyDown) {
			Application.LoadLevel (0);
			Debug.Log ("A key or mouse click has been detected");
		}
	}
}
