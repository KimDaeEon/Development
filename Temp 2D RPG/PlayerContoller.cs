using UnityEngine;
using System.Collections;

public class PlayerContoller : MonoBehaviour {

	public bool PlayerEnabled = true;
	public float moveSpeed;

    private Animator anim;
    private Rigidbody2D myRigidbody;


    private bool playerMoving;
    public Vector2 lastMove;

    private bool skilling;
    public float skillTime;
    public float skillTimeCounter;
    
    private bool attacking;
    public float attackTime;
    public float attackTimeCounter;

    private static bool playerExists;

    public string startPoint;

	void Awake(){
		if (PlayerEnabled != true) {
			this.gameObject.SetActive (true);
			PlayerEnabled = true;
		}
	}

	// Use this for initialization
	void Start () {
		anim = GetComponent<Animator>();
        myRigidbody = GetComponent<Rigidbody2D>();

        if (!playerExists)
        {
            playerExists = true;
            DontDestroyOnLoad(transform.parent.gameObject);
        }else
        {
			Destroy (transform.parent.gameObject);
        }
        
    }

    // Update is called once per frame
    void Update () {
        playerMoving = false;

        if (!attacking)
        {
			if (Input.GetAxisRaw ("Horizontal") > 0.5f || Input.GetAxisRaw ("Horizontal") < -0.5f) {
				//transform.Translate(new Vector3(Input.GetAxisRaw("Horizontal") * moveSpeed*Time.deltaTime, 0f, 0f));
				myRigidbody.velocity = new Vector2 (Input.GetAxisRaw ("Horizontal") * moveSpeed, myRigidbody.velocity.y);
				playerMoving = true;
				lastMove = new Vector2 (Input.GetAxisRaw ("Horizontal"), 0f);

			} 

			if (Input.GetAxisRaw ("Vertical") > 0.5f || Input.GetAxisRaw ("Vertical") < -0.5f) {
				//transform.Translate( new Vector3(0f,Input.GetAxisRaw("Vertical") * moveSpeed * Time.deltaTime, 0f));
				myRigidbody.velocity = new Vector2 (myRigidbody.velocity.x, Input.GetAxisRaw ("Vertical") * moveSpeed);
				playerMoving = true;
				lastMove = new Vector2 (0f, Input.GetAxisRaw ("Vertical"));
			} else {

			}

            if (Input.GetAxisRaw("Horizontal") < 0.5f && Input.GetAxisRaw("Horizontal") > -0.5f)
            {
                myRigidbody.velocity = new Vector2(0f, myRigidbody.velocity.y);
            }

            if (Input.GetAxisRaw("Vertical") < 0.5f && Input.GetAxisRaw("Vertical") > -0.5f)
            {
                myRigidbody.velocity = new Vector2(myRigidbody.velocity.x, 0f);
            }

            if (Input.GetKeyDown(KeyCode.Z))
            {
                attackTimeCounter = attackTime;
                attacking = true;
                myRigidbody.velocity = Vector2.zero;
                anim.SetBool("Attack", true);
            }

        }//if(!attacking)

        if (!skilling)
        {
            if (Input.GetAxisRaw("Horizontal") > 0.5f || Input.GetAxisRaw("Horizontal") < -0.5f)
            {
                //transform.Translate(new Vector3(Input.GetAxisRaw("Horizontal") * moveSpeed*Time.deltaTime, 0f, 0f));
                myRigidbody.velocity = new Vector2(Input.GetAxisRaw("Horizontal") * moveSpeed, myRigidbody.velocity.y);
                playerMoving = true;
                lastMove = new Vector2(Input.GetAxisRaw("Horizontal"), 0f);

            }

            if (Input.GetAxisRaw("Vertical") > 0.5f || Input.GetAxisRaw("Vertical") < -0.5f)
            {
                //transform.Translate( new Vector3(0f,Input.GetAxisRaw("Vertical") * moveSpeed * Time.deltaTime, 0f));
                myRigidbody.velocity = new Vector2(myRigidbody.velocity.x, Input.GetAxisRaw("Vertical") * moveSpeed);
                playerMoving = true;
                lastMove = new Vector2(0f, Input.GetAxisRaw("Vertical"));
            }

            if (Input.GetAxisRaw("Horizontal") < 0.5f && Input.GetAxisRaw("Horizontal") > -0.5f)
            {
                myRigidbody.velocity = new Vector2(0f, myRigidbody.velocity.y);
            }

            if (Input.GetAxisRaw("Vertical") < 0.5f && Input.GetAxisRaw("Vertical") > -0.5f)
            {
                myRigidbody.velocity = new Vector2(myRigidbody.velocity.x, 0f);
            }

            if (Input.GetKeyDown(KeyCode.X))
            {
                skillTimeCounter = skillTime;
                skilling = true;
                myRigidbody.velocity = Vector2.zero;
                anim.SetBool("Skill", true);
            }

        }// if(!skilling)


        if (attackTimeCounter > 0)
        {
            attackTimeCounter -= Time.deltaTime;
        }

        if(attackTimeCounter <= 0)
        {
            attacking = false;
            anim.SetBool("Attack", false);
        }

        if (skillTimeCounter > 0)
        {
            skillTimeCounter -= Time.deltaTime;
        }

        if (skillTimeCounter <= 0)
        {
            skilling = false;
            anim.SetBool("Skill", false);
        }


        anim.SetFloat("MoveX", Input.GetAxisRaw("Horizontal"));
        anim.SetFloat("MoveY", Input.GetAxisRaw("Vertical"));
        anim.SetBool("IsMove", playerMoving);
        anim.SetFloat("LastMoveX", lastMove.x);
        anim.SetFloat("LastMoveY", lastMove.y);

    }
}
