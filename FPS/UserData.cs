using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;


[Serializable]public class UserData
{
    // Start is called before the first frame update

    
    public string userID;
    public string eMail;
    public string userPassword;
    public int killCount;
    public int deathCount;

    public UserData()
    {

    }
    
    public UserData(string _userID,string _eMail, string _userPassword, int _killCount, int _deathCount)
    {
        userID = _userID;
        eMail = _eMail;
        userPassword = _userPassword;
        killCount = _killCount;
        deathCount = _deathCount;
    }
}
