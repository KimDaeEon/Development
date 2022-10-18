using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class UserInfoBoard : MonoBehaviour
{
    public Text killCounts, deathCounts, killsPerDeath, userID;
    DBcontroller dbController;
    // Start is called before the first frame update
    void Start()
    {
        print("UserInfoStart activated!");
        dbController = DBcontroller.instance;

        if (dbController != null)
        {
            UserData currentPlayer = CurrentUserInfo.info;

            if (killCounts != null) killCounts.text = currentPlayer.killCount.ToString();
            if (deathCounts != null) deathCounts.text = currentPlayer.deathCount.ToString();
            if (killsPerDeath != null)
            {
                if (currentPlayer.deathCount != 0)
                {
                    killsPerDeath.text = ((float)currentPlayer.killCount / currentPlayer.deathCount).ToString();
                }
                else
                {
                    killsPerDeath.text = 0.ToString();
                }
            }
            


            if(userID != null) userID.text = currentPlayer.eMail.ToString()+" ("+currentPlayer.userID+")";

        }
        else
        {
            Debug.Log("DBcontroller is missing!");
        }

    }

    // Update is called once per frame
    void Update()
    {

    }
}
