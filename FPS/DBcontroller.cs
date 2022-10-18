using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Firebase.Auth;
using UnityEngine.UI;
using UnityEngine.Networking;
using Firebase.Database;
using Firebase;
using Firebase.Unity.Editor;
using System.Threading.Tasks;
using UnityEngine.SceneManagement;

//For AES256 using these things
using System;
using System.IO;
using System.Text;
using System.Security.Cryptography;



public class DBcontroller : MonoBehaviour
{
    public GameObject whenLobbyDisabledObject;

    public static DBcontroller instance;
    string currentScene = "Login";
    const string loginScene = "Login";
    const string lobbyScene = "Lobby";

    //bool isFirst = true;
    bool isLoggedOn;

    public Dictionary<string, UserData> users;

    int userCount;

    public Text emailInput, passwordInput, statusText;
    public InputField emailInputField, passwordInputField;
    private string tempStatusString;


    private string DATA_URL = "erased for confidentiality";

    private DatabaseReference databaseReference;

    static int loadCount = 0;
    bool isLoadData = false;
    bool isLoadDataCompleteFirst = false;
    bool isLoadDataCompleteSecond = false;

    static int getUserCountTry = 0;
    bool isGetUserCount = false;
    bool isGetUserCountCompleteFirst = false;
    bool isGetUserCountCompleteSecond = false;


    bool isRegister = false;
    static bool isTextInputClear = false;


    #region AES256
    public static string Decrypt(string textToDecrypt, string key)
    {

        RijndaelManaged rijndaelCipher = new RijndaelManaged();
        rijndaelCipher.Mode = CipherMode.CBC;
        rijndaelCipher.Padding = PaddingMode.PKCS7;



        rijndaelCipher.KeySize = 128;
        rijndaelCipher.BlockSize = 128;

        byte[] encryptedData = Convert.FromBase64String(textToDecrypt);
        byte[] pwdBytes = Encoding.UTF8.GetBytes(key);
        byte[] keyBytes = new byte[16];
        int len = pwdBytes.Length;

        if (len > keyBytes.Length)
        {
            len = keyBytes.Length;
        }

        Array.Copy(pwdBytes, keyBytes, len);
        rijndaelCipher.Key = keyBytes;
        rijndaelCipher.IV = keyBytes;

        byte[] plainText = rijndaelCipher.CreateDecryptor().TransformFinalBlock(encryptedData, 0, encryptedData.Length);

        return Encoding.UTF8.GetString(plainText);

    }



    public static string Encrypt(string textToEncrypt, string key)
    {

        RijndaelManaged rijndaelCipher = new RijndaelManaged();
        rijndaelCipher.Mode = CipherMode.CBC;
        rijndaelCipher.Padding = PaddingMode.PKCS7;



        rijndaelCipher.KeySize = 128;
        rijndaelCipher.BlockSize = 128;

        byte[] pwdBytes = Encoding.UTF8.GetBytes(key);
        byte[] keyBytes = new byte[16];
        int len = pwdBytes.Length;

        if (len > keyBytes.Length)
        {
            len = keyBytes.Length;
        }

        Array.Copy(pwdBytes, keyBytes, len);
        rijndaelCipher.Key = keyBytes;
        rijndaelCipher.IV = keyBytes;

        ICryptoTransform transform = rijndaelCipher.CreateEncryptor();
        byte[] plainText = Encoding.UTF8.GetBytes(textToEncrypt);

        return Convert.ToBase64String(transform.TransformFinalBlock(plainText, 0, plainText.Length));

    }

    #endregion

    public void Login()
    {
        string tempString = Encrypt("key", passwordInput.text);
        Debug.Log("PasswordInput is " + passwordInput.text);
        Debug.Log("Login Password is " + tempString);
        FirebaseAuth.DefaultInstance.SignInWithEmailAndPasswordAsync(emailInput.text, tempString
          ).ContinueWith((task =>
         {
             if (task.IsCanceled)
             {
                 Firebase.FirebaseException e =
                 task.Exception.Flatten().InnerExceptions[0] as Firebase.FirebaseException;

                 GetErrorMessage((AuthError)e.ErrorCode);
                 return;
             }

             if (task.IsFaulted)
             {
                 Firebase.FirebaseException e =
                 task.Exception.Flatten().InnerExceptions[0] as Firebase.FirebaseException;

                 GetErrorMessage((AuthError)e.ErrorCode);
                 return;
             }

             if (task.IsCompleted)
             {
                 isLoadData = true;
                 isLoggedOn = true;
                 print("isLoggedOn is true!");
             }

         }));

    }

    public void RegisterRequest()
    {
        isLoadData = true;

        string tempString = Encrypt("key", passwordInput.text);
        if (emailInput.text.Equals("") || passwordInput.text.Equals(""))
        {
            tempStatusString = "Please enter an e-mail and password to register";
            return;
        }
        FirebaseAuth.DefaultInstance.CreateUserWithEmailAndPasswordAsync(emailInput.text,
            tempString).ContinueWith((task =>
            {
                tempStatusString = "Registering...";
                if (task.IsCanceled)
                {
                    Firebase.FirebaseException e = task.Exception.Flatten().InnerExceptions[0] as Firebase.FirebaseException;

                    GetErrorMessage((AuthError)e.ErrorCode);
                    return;
                }

                if (task.IsFaulted) //if wrong email address or password or else problems
                {
                    Firebase.FirebaseException e =
                    task.Exception.Flatten().InnerExceptions[0] as Firebase.FirebaseException;

                    GetErrorMessage((AuthError)e.ErrorCode);
                    return;
                }

                if (task.IsCompleted)
                {
                    isRegister = true;
                }

            }));

    }

    public void LogOut()
    {
        if (FirebaseAuth.DefaultInstance.CurrentUser != null)
        {
            FirebaseAuth.DefaultInstance.SignOut();

            CurrentUserInfo.info = null;
            SceneManager.LoadScene(loginScene);
            whenLobbyDisabledObject.SetActive(true);

            isLoggedOn = false;
            currentScene = loginScene;
            isLoadData = true; // LoadData again
            isTextInputClear = true; // clear the email and password text input.
        }


    }

    void GetErrorMessage(AuthError errorCode)
    {
        tempStatusString = errorCode.ToString();
    }

    public async void GetUserCountAndPrepareRegister()
    {
        await databaseReference.Child("userCount").GetValueAsync().
            ContinueWith((task =>
            {

                if (task.IsCanceled)
                {
                    Debug.Log("task is canceled!");
                    return;
                }

                if (task.IsFaulted)
                {
                    Debug.Log("task is faulted!");
                    return;
                }

                if (task.IsCompleted)
                {
                    DataSnapshot dSnapShot = task.Result;
                    string result = dSnapShot.GetRawJsonValue();

                    userCount = int.Parse(result);
                    Debug.Log("Before Registering, userCount is " + userCount);
                    RegisterUser();
                }
            }));


    }

    public async void RegisterUser()
    {
        string tempString = Encrypt("key", passwordInput.text);

        UserData tUserData = new UserData("user" + userCount, emailInput.text.ToLower(), tempString, 0, 0);

        users[tUserData.eMail] = tUserData; // new User is added to users dictionary

        string jsonData = JsonUtility.ToJson(tUserData);
        Debug.Log("before save jsondata is " + jsonData);
        await databaseReference.Child("users").Child(tUserData.userID).SetRawJsonValueAsync(jsonData);
        userCount++;
        await databaseReference.Child("userCount").SetValueAsync(userCount);
     
        tempStatusString = "Register Complete!";
    }



    public async void GetUserCount()
    {
        isGetUserCountCompleteFirst = false;
        isGetUserCountCompleteSecond = false;
        await databaseReference.Child("userCount").GetValueAsync().
            ContinueWith((task =>
            {

                if (task.IsCanceled)
                {
                    Debug.Log("task is canceled!");
                    return;
                }

                if (task.IsFaulted)
                {
                    Debug.Log("task is faulted!");
                    return;
                }

                if (task.IsCompleted)
                {
                    DataSnapshot dSnapShot = task.Result;

                    string result = dSnapShot.GetRawJsonValue();

                    userCount = int.Parse(result);
                    Debug.Log("userCount is " + userCount);
                    getUserCountTry++;
                    if (getUserCountTry == 2)
                    {
                        getUserCountTry = 0;
                        isGetUserCountCompleteSecond = true;
                    }
                    else
                    {
                        isGetUserCountCompleteFirst = true;
                    }
                }
            }));
    }


    public async void UpdateThisUserData(UserData _currentUserData)
    {
        string jsonData = JsonUtility.ToJson(_currentUserData);
        await databaseReference.Child("users").Child(_currentUserData.userID).SetRawJsonValueAsync(jsonData);
        Debug.Log(_currentUserData.userID + "has been updated Complete!");
    }



    public async void LoadData()
    {
        users.Clear();

        isLoadDataCompleteFirst = false;
        isLoadDataCompleteSecond = false;
        tempStatusString = "Synchronizing Database...";
        await FirebaseDatabase.DefaultInstance.GetReferenceFromUrl(DATA_URL).GetValueAsync().
            ContinueWith((task =>
            {

                if (task.IsCanceled)
                {
                    Debug.Log("task is canceled!");
                    return;
                }

                if (task.IsFaulted)
                {
                    Debug.Log("task is faulted!");
                    return;
                }

                if (task.IsCompleted)
                {
                    DataSnapshot dSnapShot = task.Result;

                    foreach (var child in dSnapShot.Child("users").Children)
                    {
                        string t = child.GetRawJsonValue();
                        Debug.Log(t);
                        UserData extractedUserData = JsonUtility.FromJson<UserData>(t);
                       
                        extractedUserData.eMail = extractedUserData.eMail.ToLower();
                        extractedUserData.userPassword = extractedUserData.userPassword.ToLower();
                        extractedUserData.userID = extractedUserData.userID.ToLower();
                        //kill and deaths count are integer, so to lower is not required.

                        users[extractedUserData.eMail] = extractedUserData; //add item to dictionary, key is email
                                                                         
                    }
                    isGetUserCount = true; // GetUserCount activated!
                    loadCount++;
                    if (loadCount == 2)
                    {
                        isLoadDataCompleteSecond = true;
                        loadCount = 0;
                    }
                    else
                    {
                        isLoadDataCompleteFirst = true;
                    }
                }
            }));
        tempStatusString = "Synchronization Complete!";
    }

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
        }
        else if (instance != this)
        {
            Destroy(gameObject);
        }

        DontDestroyOnLoad(gameObject);
    }
    private void Start()
    {
        users = new Dictionary<string, UserData>();
        FirebaseApp.DefaultInstance.SetEditorDatabaseUrl(DATA_URL);
        databaseReference = FirebaseDatabase.DefaultInstance.RootReference;
        LoadData();

        isLoggedOn = false;

        emailInput = GameObject.FindGameObjectWithTag("eMail").GetComponent<Text>();
        passwordInput = GameObject.FindGameObjectWithTag("password").GetComponent<Text>();
        statusText = GameObject.FindGameObjectWithTag("status").GetComponent<Text>();
    }

    private void Update()
    {
        if (isLoadData)
        {
            Debug.Log("Load First!");
            LoadData();
            isLoadData = false;
        }

        if (isLoadDataCompleteFirst)
        {
            Debug.Log("Load Second!");
            LoadData();
        }

        if (isLoadDataCompleteSecond && isGetUserCount)
        {
            GetUserCount();
            isGetUserCount = false;
        }
        if (isGetUserCountCompleteFirst)
        {
            GetUserCount();
        }

        if (isLoadDataCompleteSecond && isGetUserCountCompleteSecond && isRegister)
        {
            GetUserCountAndPrepareRegister();
            isLoadDataCompleteSecond = false;
            isGetUserCountCompleteSecond = false;
            isRegister = false;
        }

        if (isTextInputClear)
        {
            Debug.Log("Text Clear Activated!");
            if (emailInputField != null && passwordInputField != null)
            {
                Debug.Log("Email and Password is cleared!");
                emailInputField.text = "";
                passwordInputField.text = "";
                isTextInputClear = false;
            }
            else
            {
                Debug.Log("Email or PasswordInput Field is null!!");
            }

        }

        if (statusText != null)
        {
            statusText.text = tempStatusString;
        }

        if (isLoadDataCompleteSecond &&isLoggedOn && currentScene == loginScene)
        {
            if(users[emailInput.text.ToLower()] != null)
            {
                CurrentUserInfo.info = users[emailInput.text.ToLower()]; // save current user info as static
                print("currentUser is assigned!");
                currentScene = lobbyScene;
                whenLobbyDisabledObject.SetActive(false);
                isLoadDataCompleteSecond = false;

                SceneManager.LoadScene(lobbyScene);
            }
            else
            {
                tempStatusString = "Cannot Get UserInfo!! Try Again!";
            }
        }

    }

}
