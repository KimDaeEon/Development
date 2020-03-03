using UnityEngine;
using System.Collections;

public class CoinCounter : MonoBehaviour {
	#region PUBLIC VARIABLES

	public Sprite[] spriteDigits;		// Array of Sprites representing the GUI's coin numerals
	public int value = 0;				// The total value of coins collected; updated when coins are collected
	public float spacing = 0.4f;		// The distance between the digits in the GUI's coin numerals

	#endregion
	#region PRIVATE VARIABLES

	private int displayValue = -1;		// The value used to determine what the GUI shows; used to determine when the value has changed

	public SpriteRenderer[] renderers;
	#endregion


	// Update is called once per frame
	void Update () {
		if(displayValue != value){
			string digits = value.ToString ();

			renderers = GetComponentsInChildren<SpriteRenderer> ();

			int numRenderers = renderers.Length;

			if (numRenderers < digits.Length) {
				while (numRenderers < digits.Length) {
					GameObject spr = new GameObject ();

					spr.AddComponent<SpriteRenderer> ();

					spr.transform.parent = transform;

					spr.transform.localPosition = new Vector3 (numRenderers * spacing, 0.0f, 0.0f);

					spr.layer = 5;

					numRenderers = numRenderers + 1;
				}

				renderers = GetComponentsInChildren<SpriteRenderer>();
			}
			else if(numRenderers > digits.Length){
				while(numRenderers > digits.Length){
					renderers[numRenderers-1].sprite = null;
					numRenderers = numRenderers -1;
				} //while
			}// else if
		
			int rendererIndex = 0;

			foreach(char digit in digits){
				int spriteIndex = int.Parse(digit.ToString());
				renderers[rendererIndex].sprite = spriteDigits[spriteIndex];

				rendererIndex = rendererIndex + 1;
			}//foreach
			displayValue = value;
		}//if

	}
}
