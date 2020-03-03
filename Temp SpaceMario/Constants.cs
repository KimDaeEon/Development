using UnityEngine;
using System.Collections;

public class Constants : MonoBehaviour {
	#region PLAYER PREFS
	public const string PREF_COINS = "Coins";
	public const string PREF_CURRENT_LEVEL = "CurrentLevel";
	#endregion

	#region SCENE NUMBERS
	public const int SCENE_LEVEL_1 = 1;
	public const int SCENE_LEVEL_2 = 2;
	public const int SCENE_LEVEL_3 = 2;
	public const int SCENE_GAME_OVER = 3;
	public const int SCENE_GAME_WIN = 0;
	public const int SCENE_TITLE = 0;
	#endregion

	#region PLAYER MOVEMENT VALUES
	public const float playerMaxSpeed = 7;
	public const float playerJumpForce = 850;
	public const float playerGroundCheckRadius = 0.2f;
	#endregion
	#region ANIMATOR VARIABLE NAMES
	public const string animSpeed = "Speed";
	public const string animJump = "Jump";
	public const string animDie = "Die";
	public const string animDamage = "Damage";
	#endregion
	#region INPUT NAMES
	public const string inputMove = "Horizontal";
	public const string inputJump = "Jump";
	#endregion
}
