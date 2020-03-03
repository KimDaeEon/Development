using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class FloatingNumber : MonoBehaviour {

    public float moveSpeed;
    public int damageNumber;
    public Text displayNumber;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        displayNumber.text = "" + damageNumber + " damage";
        transform.position = new Vector3(transform.position.x, transform.position.y + (moveSpeed * Time.deltaTime),transform.position.z);
	}
}
