using UnityEngine;
using System.Collections;

public class GUIGame : MonoBehaviour {

	public GameObject heart1;
	public GameObject heart2;
	public GameObject heart3;

	public Sprite heartFull;
	public Sprite heartHalf;
	public Sprite heartEmpty;

	public void Updatehealth(int health){
		switch (health) {
			case 0:
				heart1.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				heart2.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				heart3.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				break;
			case 1:
				heart1.GetComponent<SpriteRenderer> ().sprite = this.heartHalf;
				heart2.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				heart3.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				break;
			case 2:
				heart1.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				heart2.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				heart3.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				break;
			case 3:
				heart1.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				heart2.GetComponent<SpriteRenderer> ().sprite = this.heartHalf;
				heart3.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				break;
			case 4:
				heart1.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				heart2.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				heart3.GetComponent<SpriteRenderer> ().sprite = this.heartEmpty;
				break;
			case 5:
				heart1.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				heart2.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				heart3.GetComponent<SpriteRenderer> ().sprite = this.heartHalf;
				break;
			case 6:
				heart1.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				heart2.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				heart3.GetComponent<SpriteRenderer> ().sprite = this.heartFull;
				break;
		}

	}

	// Update is called once per frame
	void Update () {
	
	}
}
