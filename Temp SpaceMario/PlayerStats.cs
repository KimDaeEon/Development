using UnityEngine;
using System.Collections;

public class PlayerStats : MonoBehaviour {
	public int health = 6;
	public int coinsCollected = 0;

	public bool isImmune = false;
	public float immunityDuration = 1.5f;
	private float immunityTime = 0f;

	private float flickerDuration = 0.1f;
	private float flickerTime = 0f;
	private SpriteRenderer spriteRenderer;

	public bool isDead = false;

	private GameObject HUDCamera;
	private GameObject HUDSprite;

	public ParticleSystem particleHit;

	private float deathTimeElapsed;


	void Start(){
		spriteRenderer = this.gameObject.GetComponent<SpriteRenderer> ();
		if (Application.loadedLevel != Constants.SCENE_LEVEL_1) {
			coinsCollected = PlayerPrefs.GetInt (Constants.PREF_COINS);
		}
		PlayerPrefs.SetInt (Constants.PREF_CURRENT_LEVEL, Application.loadedLevel);

		HUDCamera = GameObject.FindGameObjectWithTag ("HUDCamera");
		HUDSprite = GameObject.FindGameObjectWithTag ("HUDSprite");

		this.HUDSprite.GetComponent<CoinCounter>().value = this.coinsCollected;


	}

	void Update(){
		if (this.isImmune == true) {
			this.SpriteFlicker ();
			immunityTime = immunityTime + Time.deltaTime;
			if (immunityTime >= immunityDuration) {
				this.isImmune = false;
				spriteRenderer.enabled = true;
				//Debug.Log ("Immunity has ended");
			}
		}

		if (this.isDead == true) {
			this.deathTimeElapsed = this.deathTimeElapsed + Time.deltaTime;
			if (this.deathTimeElapsed >= 2.0f) {
				Application.LoadLevel ("Game_Over");
			}
		}
	}


	void PlayerIsDead(){
		this.isDead = true;
		this.gameObject.GetComponent<Animator> ().SetTrigger ("Damage");
		this.gameObject.GetComponent<CircleCollider2D> ().isTrigger = true;

		this.GetComponent<Rigidbody2D> ().velocity = new Vector2 (0, 0);
		this.GetComponent<Rigidbody2D> ().AddForce (new Vector2 (0, 600));


		GameObject.FindGameObjectWithTag ("MainCamera").GetComponent<CamearaFollow> ().enabled = false;
		GameObject.FindGameObjectWithTag ("GroundCheck").GetComponent<CircleCollider2D> ().isTrigger = true;

	}



	public void CollectCoin(int coinValue){
		this.coinsCollected = this.coinsCollected + coinValue;
		this.HUDSprite.GetComponent<CoinCounter>().value = this.coinsCollected;
	}

	public void TakeDamage(int damage, bool playHitReaction){
		if (this.isImmune == false && this.isDead == false) {
			this.health = this.health - damage;
			this.HUDCamera.GetComponent<GUIGame> ().Updatehealth (this.health);

			if (this.health <= 0) {
				PlayerIsDead ();
				this.health = 0;
				this.HUDCamera.GetComponent<GUIGame> ().Updatehealth (this.health);
			} else {
				Debug.Log ("Player Health:" + this.health.ToString ());
				if (playHitReaction == true) {
					Debug.Log ("Hit reaction called");
					this.PlayHitReaction ();
				}
			}
		}
	}


	void PlayHitReaction(){
		this.isImmune = true;
		this.immunityTime = 0f;

		PlayerController playerController = this.gameObject.GetComponent<PlayerController> ();


		this.particleHit.Play ();


		this.gameObject.GetComponent<Animator> ().SetTrigger ("Damage");
		playerController.PlayDamageAudio ();
	}

	void SpriteFlicker(){
		if (this.flickerTime < this.flickerDuration) {
			this.flickerTime = this.flickerTime + Time.deltaTime;
		} else if (this.flickerTime >= this.flickerDuration) {
			spriteRenderer.enabled = !(spriteRenderer.enabled);
			this.flickerTime = 0f;
		}
	}
}
