using UnityEngine;
using System.Collections;

public class PlayerStartPoint : MonoBehaviour {

    private PlayerContoller thePlayer;
    private CameraController theCamera;

    public Vector2 startDirection;

    public string startPointName;

	// Use this for initialization
	void Start () {
        thePlayer = FindObjectOfType<PlayerContoller>();

        if (thePlayer.startPoint == startPointName)
        {
            thePlayer.transform.position = transform.position;
            thePlayer.lastMove = startDirection;

            theCamera = FindObjectOfType<CameraController>();
            theCamera.transform.position = new Vector3(transform.position.x, transform.position.y, theCamera.transform.position.z);
        }
    }
	
	// Update is called once per frame
	void Update () {
	
	}
}
