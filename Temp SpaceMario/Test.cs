using UnityEngine;
using System.Collections;

public class Test : MonoBehaviour {

	public BoxCollider2D[] rb;
	// Use this for initialization
	void Start () {
		rb = GetComponentsInChildren<BoxCollider2D>(false);

	}

	// Update is called once per frame
	void Update () {

	}
}
