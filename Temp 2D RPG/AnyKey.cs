using UnityEngine;
using System.Collections;

public class AnyKey : MonoBehaviour {
	private float ftime = 0;
	private float ftime2 = 0;

	public PlayerContoller thePlayer;
	public CameraController theCamera;

	// Use this for initialization
	void Start () {
		thePlayer = FindObjectOfType<PlayerContoller>();
		theCamera = FindObjectOfType<CameraController> ();		

		thePlayer.gameObject.SetActive (false);
		theCamera.gameObject.SetActive (false);

	}
	
	// Update is called once per frame
	void Update () {
		ftime += Time.deltaTime;
		if (ftime > 1) {
			if (Input.anyKeyDown) {
				thePlayer.GetComponentInParent<PlayerStarter> ().isPlayerEnabled = false;
				theCamera.GetComponentInParent<CameraStarter> ().isCameraEnabled = false;
			
				Application.LoadLevel ("FirstStage");
			}
				
		}
	}
}
