using UnityEngine;
using System.Collections;

public class SlimeMovement : MonoBehaviour {

    public float moveSpeed;

    private Rigidbody2D myRigidbody;

    private bool moving;

    public float timeBetweenMove;
    private float timeBetweenMoveCounter;

    public float timeToMove;
    private float timeToMoveCounter;

    private Vector3 moveDirection;

  
	
    // Use this for initialization
	void Start () {
        myRigidbody = GetComponent<Rigidbody2D>();
        timeBetweenMoveCounter = timeBetweenMove;
        timeToMoveCounter = timeToMove;

	}
	
	// Update is called once per frame
	void Update () {
        timeToMove = Random.Range(1f, 3f);
        timeBetweenMove = Random.Range(1f, 3f);
        if (moving)
        {
            timeToMoveCounter -= Time.deltaTime;
            myRigidbody.velocity = moveDirection;

            if(timeToMoveCounter < 0f)
            {
                moving = false;
                timeBetweenMoveCounter -= Time.deltaTime;

            }
        } else{
            timeBetweenMoveCounter -= Time.deltaTime;
            myRigidbody.velocity = Vector2.zero;
            
            if(timeBetweenMoveCounter < 0f)
            {
                moving = true;
                timeToMoveCounter = timeToMove;

                moveDirection = new Vector3(Random.Range(-1f, 1f) * moveSpeed, Random.Range(-1f, 1f) * moveSpeed, 0f);
            } 
        }
       
	}//Update

    void OnCollisionEnter2D (Collision2D other)
    {
        /* if(other.gameObject.tag == "Player")
        {

            other.gameObject.SetActive(false);
            reloading = true;
            thePlayer = other.gameObject;
        } */
    }
}//Slime Movement
