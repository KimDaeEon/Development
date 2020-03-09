using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KillFeed : MonoBehaviour
{

    [SerializeField] GameObject killFeedItemPrefab;
    void Start()
    {
        GameManager.instance.onPlayerKilledCallBack = OnKill; 
        //if '=' is not +=, is there a problem? @@ 
    } 

    
    public void OnKill(string _killedPlayerName, string _killerName)
    {
        GameObject go = Instantiate(killFeedItemPrefab, this.transform);
        go.GetComponent<KillFeedItem>().SetUp(_killedPlayerName, _killerName);

        Destroy(go, 4f);
    }
}
