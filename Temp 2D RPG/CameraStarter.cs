using UnityEngine;
using System.Collections;

public class CameraStarter : MonoBehaviour {
	public GameObject theCamera;
	public bool isCameraEnabled = false;

	// Use this for initialization
	void Start () {
	}

	// Update is called once per frame
	void Update () {
		if (isCameraEnabled != true) {
			theCamera = transform.Find ("Camera").gameObject;
			theCamera.SetActive (true);
			isCameraEnabled = true;
		}


	}
}
