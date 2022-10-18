using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.Match;

public class PauseMenu : MonoBehaviour
{
    public static bool IsPauseOn = false;

    private NetworkManager networkManager;

    private void Start()
    {
        networkManager = NetworkManager.singleton;
    }

    public void LeaveMatch()
    {
        Debug.Log("Leave Match Activated!");
        MatchInfo matchInfo = networkManager.matchInfo;
        networkManager.matchMaker.DropConnection(networkManager.matchInfo.networkId, matchInfo.nodeId, 0, networkManager.OnDropConnection); ;
        networkManager.StopHost();// @ host migration will be considered. 
    }
}
