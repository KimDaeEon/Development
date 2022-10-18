using UnityEngine;
using UnityEngine.Networking;

public class HostGame : MonoBehaviour
{
    [SerializeField] private uint roomSize = 6;
    private string roomName;
    DBcontroller dbController;

    private NetworkManager networkManager;

    private void Start()
    {
        networkManager = NetworkManager.singleton;
        dbController = DBcontroller.instance;

        //This is populated if StartMatchMaker() has been called.It is used to communicate with the matchmaking service.This should be shut down after the match is complete to clean up its internal state.If this object is null then the client is not setup to communicate with MatchMaker yet.
        if(networkManager.matchMaker == null) // 
        {
            networkManager.StartMatchMaker();
        }
    }
    public void SetRoomName (string _name)
    {
        roomName = _name;
    }

    public void CreateRoom()
    {
        if(roomName != "" && roomName != null)
        {
            Debug.Log("Creating Room: " + roomName + " with room for " + roomSize + " players.");
            networkManager.matchMaker.CreateMatch(roomName,roomSize,true, "","","",0,0, networkManager.OnMatchCreate);
        }
    }

    public void LogOut()
    {
        dbController.LogOut();
    }
}
