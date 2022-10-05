using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerManager
{
    MyPlayer _myPlayer;
    Dictionary<int, Player> _players = new Dictionary<int, Player>();

    public static PlayerManager Instance { get; } = new PlayerManager();

    public void Add(S_PlayerList pkt)
    {
        Object obj = Resources.Load("Player");

        foreach (S_PlayerList.Player player in pkt.players)
        {
            GameObject gameObject = Object.Instantiate(obj) as GameObject;
            if (player.isSelf)
            {
                MyPlayer myPlayer = gameObject.AddComponent<MyPlayer>();
                myPlayer.transform.position = new Vector3(player.posX, player.posY, player.posZ);
                myPlayer.PlayerId = player.playerId;
                _myPlayer = myPlayer;
            }
            else
            {
                Player tempPlayer = gameObject.AddComponent<Player>();
                tempPlayer.PlayerId = player.playerId;
                tempPlayer.transform.position = new Vector3(player.posX, player.posY, player.posZ);
                _players.Add(player.playerId, tempPlayer);
            }
        }
    }

    internal void LeaveGame(S_BroadcastLeaveGame pkt)
    {
        if(_myPlayer.PlayerId == pkt.playerId)
        {
            GameObject.Destroy(_myPlayer.gameObject);
            _myPlayer = null;
        }
        else
        {
            Player player = null;
            if(_players.TryGetValue(pkt.playerId, out player))
            {
                GameObject.Destroy(player.gameObject);
                _players.Remove(pkt.playerId);
            }
        }
    }

    internal void EnterGame(S_BroadcastEnterGame pkt)
    {
        if (pkt.playerId == _myPlayer.PlayerId)
        {
            return;
        }

        Object obj = Resources.Load("Player");
        GameObject gameObject = Object.Instantiate(obj) as GameObject;

        Player player = gameObject.AddComponent<Player>();
        player.PlayerId = pkt.playerId;
        player.transform.position = new Vector3(pkt.posX, pkt.posY, pkt.posZ);
        _players.Add(pkt.playerId, player);
    }

    internal void Move(S_BroadcastMove pkt)
    {
        if(_myPlayer.PlayerId == pkt.playerId)
        {
            _myPlayer.transform.position = new Vector3(pkt.posX, pkt.posY, pkt.posZ);
        }
        else
        {
            Player player = null;
            if(_players.TryGetValue(pkt.playerId, out player))
            {
                player.transform.position = new Vector3(pkt.posX, pkt.posY, pkt.posZ);
            }
        }
    }
}
