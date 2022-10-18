using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking.Match;
public class RoomListItem : MonoBehaviour
{
    public delegate void JoinMatchDelegate(MatchInfoSnapshot _match);
    private JoinMatchDelegate joinMatchCallback;

    private MatchInfoSnapshot match;

    [SerializeField] private Text roomNameText;
    public void MatchSetup(MatchInfoSnapshot _match, JoinMatchDelegate _joinMatchCallback)
    {
        match = _match;
        joinMatchCallback = _joinMatchCallback;

        roomNameText.text = match.name + " (" + match.currentSize + "/" + match.maxSize + ")";
    }

    public void JoinMatch()
    {
        joinMatchCallback.Invoke(match); 
    }
}
