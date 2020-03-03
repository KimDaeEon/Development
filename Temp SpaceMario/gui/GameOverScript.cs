using UnityEngine;
using System.Collections;

public class GameOverScript : MonoBehaviour {

	public GUISkin SKin;
	public float gapSize = 20f;

	void OnGUI(){
		GUI.skin = SKin;

		GUILayout.BeginArea (new Rect (0, 0 ,Screen.width, Screen.height));

		GUILayout.BeginVertical();
		GUILayout.Label("Game Over");
		GUILayout.Space(gapSize);

		// Making first button, If it is pushed, current level is reloaded
		if(GUILayout.Button("Retry!")){
			Application.LoadLevel (PlayerPrefs.GetInt (Constants.PREF_CURRENT_LEVEL));
		}

		GUILayout.Space(gapSize);

		// Making second button. If it is pushed, the game will be restarted.
		if(GUILayout.Button("Restart!")){
			Application.LoadLevel (Constants.SCENE_LEVEL_1);
		}

		GUILayout.Space (gapSize);

		// Making third button. If it is pushed, game will be done.
		if(GUILayout.Button("Quit!")){
			Application.Quit ();
		}

		GUILayout.EndVertical();
		GUILayout.EndArea();
	}

}
