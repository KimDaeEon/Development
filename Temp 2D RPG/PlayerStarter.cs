using UnityEngine;
using System.Collections;

public class PlayerStarter : MonoBehaviour {
	public GameObject thePlayer;
	public bool isPlayerEnabled = false;
	private Transform firstStartPoint;
	// Use this for initialization
	void Start () {
		}
	
	// Update is called once per frame
	void Update () {
		if (isPlayerEnabled != true) {
			thePlayer = transform.Find ("Player").gameObject;
			thePlayer.SetActive (true);
			thePlayer.GetComponentInChildren<Animator> ().enabled = true;

			firstStartPoint = GameObject.FindGameObjectWithTag ("FirstStartPoint").transform;
			thePlayer.transform.position = firstStartPoint.position;
			isPlayerEnabled = true;
			GameObject.FindGameObjectWithTag ("Swing").SetActive (false);
		}
	}
}
