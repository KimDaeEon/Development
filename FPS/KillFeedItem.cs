using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class KillFeedItem : MonoBehaviour
{
    [SerializeField] Text text;

    public void SetUp(string _killedPlayerName, string _killerName)
    {
        text.text = "<b>" + _killerName + "</b>" + " killed " + "<i>" + _killedPlayerName + "</i>";
    }
}
