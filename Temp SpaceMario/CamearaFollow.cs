using UnityEngine;
using System.Collections;

public class CamearaFollow : MonoBehaviour {

	public float xMargin = 1;
	public float yMargin = 1;
	public float xSmooth = 4;
	public float ySmooth = 4;

	public Vector2 maxXAndY;
	public Vector2 minXAndY;

	private Transform playerTransform;

	public float targetX;
	public float targetY;

	void Awake(){
		playerTransform = GameObject.FindGameObjectWithTag ("Player").transform;
	}

	bool CheckXMargin(){
		return Mathf.Abs (playerTransform.position.x - this.transform.position.x) > xMargin;
	}

	bool CheckYMargin(){
		return Mathf.Abs (playerTransform.position.y - this.transform.position.y) > yMargin;
	}

	void TrackPlayer(){
		targetX = transform.position.x;
		targetY = transform.position.y;

		if (CheckXMargin () == true) {
			targetX = Mathf.Lerp (transform.position.x, playerTransform.position.x, xSmooth * Time.deltaTime);
		}
		if (CheckYMargin () == true) {
			targetY = Mathf.Lerp (transform.position.y, playerTransform.position.y, xSmooth * Time.deltaTime);
		}

		targetX = Mathf.Clamp (targetX, minXAndY.x, maxXAndY.x);
		targetY = Mathf.Clamp (targetY, minXAndY.y, maxXAndY.y);
		transform.position = new Vector3 (targetX, targetY, transform.position.z);
	}
	void FixedUpdate(){
		TrackPlayer ();	
	}


}
