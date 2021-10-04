using UnityEngine;
using System.Collections;

public class PlayerController : MonoBehaviour
{

	#region VARIABLES
	#region PUBLIC HIDDEN VARIABLES
	[HideInInspector]
	public bool isJumping = false;
	[HideInInspector]
	public bool isGrounded = false;
	[HideInInspector]
	public bool isFacingRight = true;
	#endregion

	#region PUBLIC ARIABLES
	public AudioClip[] footstepSounds;
	public AudioClip JumpSound;
	public AudioClip damageSound;

	public float jumpForce = Constants.playerJumpForce;
	public float maxSpeed = Constants.playerMaxSpeed;
	
	public Transform groundCheck;
	public LayerMask groundLayers;

	public bool isDoubleJumping = false;
	#endregion

	#region PRIVATE VARIABLES
	private AudioSource audioSource;
	private float groundCheckRadius = Constants.playerGroundCheckRadius;
	private Animator anim;
	private float ftime = 0;
	#endregion
	#endregion

	#region INHERENT METHODS [Awake, Update, FixedUpdate]

	void Awake()
	{
		anim = this.GetComponent<Animator> ();
		audioSource = this.GetComponent<AudioSource> ();
	}

	void Update()
	{
		isGrounded = Physics2D.OverlapCircle
			(groundCheck.position, groundCheckRadius, groundLayers);
		
		if (isGrounded == true) {
			isDoubleJumping = false;
			this.anim.SetBool ("Ground", true);
			//this.anim.SetBool (Constants.animJump, false);
		}else if (isGrounded == false) {
			this.anim.SetBool ("Ground", false);
		}

		if(Input.GetButtonDown("Cancel")){
			Application.Quit();
		}

		Jump ();
	}
	
	void FixedUpdate()
	{
		isGrounded = Physics2D.OverlapCircle
			(groundCheck.position, groundCheckRadius, groundLayers);
		

		if (isGrounded == true) {
			ftime = 0;
			isDoubleJumping = false;
			this.anim.SetBool ("Ground", true);
			this.anim.SetBool (Constants.animJump, false);
		}else if (isGrounded == false) {
			this.anim.SetBool ("Ground", false);
			ftime += Time.deltaTime;
		}

		PhysicsMaterial2D material = this.gameObject.GetComponent<CircleCollider2D> ().sharedMaterial;



		if (isGrounded == true && material) {
			CircleCollider2D collision = this.gameObject.GetComponent<CircleCollider2D> ();

			collision.sharedMaterial = null;

			collision.enabled = false;
			collision.enabled = true; // immediatley initialize and activate 'collision'.
		} else if(isGrounded == false && 
			this.gameObject.GetComponent<CircleCollider2D>().sharedMaterial == null){
			CircleCollider2D collision = this.gameObject.GetComponent<CircleCollider2D> ();
	
			collision.enabled = false;
			collision.enabled = true;
		}

		try
		{
			float move = Input.GetAxis("Horizontal");
			if(ftime <= 1 ){
				this.GetComponent<Rigidbody2D>().velocity = new Vector2 (move * maxSpeed, GetComponent<Rigidbody2D>().velocity.y);
			}
			this.anim.SetFloat("Speed",Mathf.Abs(move));

			if((move > 0.0f && isFacingRight == false) || (move < 0.0f && isFacingRight == true))
			{
				Flip ();
			}
		}
		catch(UnityException error)
		{
			Debug.LogError(error.ToString());
		}
	}	
	#endregion

	#region UTILITY METHODS [Flip, Jump]
	void Flip()
	{

		isFacingRight = !isFacingRight;						
		Vector3 playerScale = transform.localScale;			
		playerScale.x = playerScale.x * -1;					
		transform.localScale = playerScale;				    
	}

	void Jump(){
		if(Input.GetButtonDown(Constants.inputJump))
		{
			if (isGrounded == true) {
				this.anim.SetBool (Constants.animJump, true);
				this.anim.SetBool ("Ground", false);	

				this.GetComponent<Rigidbody2D> ().velocity = new Vector2 (this.GetComponent<Rigidbody2D> ().velocity.x, 0);
				this.GetComponent<Rigidbody2D> ().AddForce (new Vector2 (0, jumpForce));
				PlayJumpAudio ();
			} else if (isDoubleJumping == false) {
				isDoubleJumping = true;
				this.anim.SetBool (Constants.animJump, true);
				this.anim.SetBool ("Ground", false);

				this.GetComponent<Rigidbody2D> ().velocity = new Vector2 (this.GetComponent<Rigidbody2D> ().velocity.x, 0);
				this.GetComponent<Rigidbody2D>().AddForce(new Vector2(0,jumpForce));
				PlayJumpAudio ();
			}
		}
	}
	#endregion

	#region AUDIO METHODS [PlayFootstepAudio, PlayJumpAudio, PlayDamageAudio]
	void PlayFootstepAudio(){
		this.audioSource.clip = footstepSounds[(Random.Range(0,footstepSounds.Length))];
		this.audioSource.Play ();
	}

	public void PlayDamageAudio(){
		this.audioSource.clip = damageSound;
		this.audioSource.Play ();
	}
		
	void PlayJumpAudio(){
		AudioSource.PlayClipAtPoint (this.JumpSound, this.transform.position);
		Debug.Log(this.transform.position.ToString());
	}
	#endregion

}