using UnityEngine.UI;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.Match;
public class JoinGame : MonoBehaviour
{
    private NetworkManager networkManager;
    
    List<GameObject> roomList = new List<GameObject>();

    [SerializeField]private Text status;

    [SerializeField] private GameObject roomListItemPrefab;

    [SerializeField] private Transform roomListParent;
    private void Start()
    {
        networkManager = NetworkManager.singleton;
        if(networkManager.matchMaker == null)
        {
            networkManager.StartMatchMaker();
        }

        RefreshRoomList();
    }

    public void RefreshRoomList()
    {
        ClearRoomList();
        status.text = "Loading...";
        networkManager.matchMaker.ListMatches(0,20,"",true,0,0,OnMatchList);
    }

    public void OnMatchList(bool success, string extendedInfo, List<MatchInfoSnapshot> matchList)
    {
        status.text = "";
        if(!success ||matchList == null)
        {
            status.text = "Couldn't get match list.";
            return;
        }


        foreach(MatchInfoSnapshot match in matchList)
        {
            GameObject _roomListItemGO = Instantiate(roomListItemPrefab);
            _roomListItemGO.transform.SetParent(roomListParent);

            RoomListItem _roomListItem = _roomListItemGO.GetComponent<RoomListItem>();
            if(_roomListItem != null)
            {
                _roomListItem.MatchSetup(match,JoinMatch);
            }
            // that will take care of setting up the name/amouint of users
            // as well as setting up a callback function that will join the game.
            roomList.Add(_roomListItemGO);
        }

        if(roomList.Count == 0)
        {
            status.text = "No rooms at the moment. ";
        }
    }

    void ClearRoomList()
    {
        for (int i = 0; i< roomList.Count; i++)
        {
            Destroy(roomList[i]);
        }

        roomList.Clear(); 
    }

    public void JoinMatch(MatchInfoSnapshot _match)
    {
        networkManager.matchMaker.JoinMatch(_match.networkId, "", "", "", 0, 0, networkManager.OnMatchJoined);
        ClearRoomList();
        status.text = "Joining...";
    }
}
