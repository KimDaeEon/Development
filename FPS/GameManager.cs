using UnityEngine;
using System.Collections.Generic;
using System.Linq;

public class GameManager : MonoBehaviour
{
    public static GameManager instance; // if it is not static, it cannot transfer information.



    public MatchSettings matchSettings;

    [SerializeField] private GameObject sceneCamera;

    public delegate void OnPlayerKilledCallBack(string _killedPlayerName, string _killerName);
    public OnPlayerKilledCallBack onPlayerKilledCallBack;

    private void Awake()
    {
        if(instance != null)
        {
            Debug.LogError("More than one GameManager in scene.");   
        }
        else
        {
            instance = this;
        }

        sceneCamera = GameObject.FindGameObjectWithTag("SceneCamera");
    }
    
    public void SetSceneCameraActive (bool isActive)
    {
        if(sceneCamera == null)
        {
            return;
        }

        sceneCamera.SetActive(isActive);
    }

    #region Manage Player
    
    public static Dictionary<string, PlayerManager> players = new Dictionary<string, PlayerManager>();

    public static void RegisterPlayer(string _netID, PlayerManager _player)
    {
        string playerID = _netID;
        players.Add(playerID, _player);
        _player.transform.name = playerID;
    }

   

    public static void DeRegisterPlayer(string _playerID)
    {
        players.Remove(_playerID);
    }

    public static PlayerManager GetPlayer(string _playerID)
    {
        return players[_playerID];
    }

    public static PlayerManager[] GetAllPlayers()
    {
        return players.Values.ToArray();
    }

    //public static void ShowDicitonaryList()
    //{
    //    int i = 1;
    //    foreach(KeyValuePair<string, PlayerManager> player in players)
    //    {
    //        print(i + "th user!");
    //        print("user id is" + players[player.Key].currentUserData.userID);
    //        print("kills" + players[player.Key].currentUserData.killCount);
    //        print("deaths" + players[player.Key].currentUserData.deathCount);
    //        print("netID is" + player.Key);
    //        i++;

    //    }
    //}
    //private void OnGUI()
    //{
    //    GUILayout.BeginArea(new Rect(200, 200, 200, 500));
    //    GUILayout.BeginVertical();
    //    foreach (string playerID in players.Keys)
    //    {
    //        GUILayout.Label(playerID + " - " + players[playerID].transform.name);
    //    }

    //    GUILayout.EndVertical();
    //    GUILayout.EndArea();
    //}

    #endregion



}
