using UnityEngine;
using System.Collections;

public class TitleScreenScript : MonoBehaviour {

	public GUISkin Skin;


	// Update is called once per frame
	void Update () {
		if (Input.anyKeyDown) {
			Application.LoadLevel (Constants.SCENE_LEVEL_1);
		}
	}


	void OnGUI(){
		// Choose a skin for using
		GUI.skin = Skin;
		GUILayout.BeginArea (new Rect (300, 480, Screen.width, Screen.height));
		GUILayout.BeginVertical ();
		GUILayout.Space (10);
		GUILayout.BeginHorizontal ();
		GUILayout.FlexibleSpace ();
		GUILayout.Label ("Press Any Key To Begin", GUILayout.ExpandWidth (true));
		GUILayout.EndHorizontal ();
		GUILayout.EndVertical ();
		GUILayout.EndArea ();
	}
}
