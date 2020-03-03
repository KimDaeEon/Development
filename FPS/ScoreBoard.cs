using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScoreBoard : MonoBehaviour
{
    [SerializeField] GameObject playerScoreBoardItem;
    [SerializeField] Transform scoreBoardItemListTransform;
    private void OnEnable()
    {
        PlayerManager[] players = GameManager.GetAllPlayers();

        foreach (PlayerManager player in players)
        {
            GameObject itemGO = (GameObject)Instantiate(playerScoreBoardItem, scoreBoardItemListTransform);
            ScoreBoardItem item = itemGO.GetComponent<ScoreBoardItem>();
            if(item != null)
            {
                item.SetUp(player.userName, player.kills, player.deaths);
            }
        }
    }

    private void OnDisable()
    {
        foreach (Transform child in scoreBoardItemListTransform)
        {
            Destroy(child.gameObject);
        }
    }

}
