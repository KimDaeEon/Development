
using UnityEngine;
using UnityEngine.Networking;

[RequireComponent(typeof(PlayerManager))]
[RequireComponent(typeof(PlayerController))]
public class PlayerSetup : NetworkBehaviour
{
    DBcontroller myDB;

    [SerializeField] Behaviour[] componentsToDisable;

    [SerializeField] string remoteLayerName = "RemotePlayer";

    [SerializeField] string doNotDrawLayerName = "DoNotDraw";

    [SerializeField] GameObject[] culledGraphics;

    [SerializeField] GameObject playerUIPrefab;
    [HideInInspector] public GameObject playerUIInstance;


    // Start is called before the first frame update
    void Start()
    {

        Debug.Log(this.gameObject.name + "'s PlayerSetUp Activated!");
        if (!isLocalPlayer)
        {
            DisableComponents();
            AssignRemoteLayer();

        }
        else // if it is local player
        {
            myDB = DBcontroller.instance;
            // Disable player graphics for local player(do not show unnecessary image)
            CullObject(culledGraphics, LayerMask.NameToLayer(doNotDrawLayerName));

            // Instantiate PlayerUI and allocate it
            playerUIInstance = Instantiate(playerUIPrefab); // playerUIPrefab is not class, so we have to instantiate it.
            playerUIInstance.name = playerUIPrefab.name;

            // Configure(SetUp) PlayerUI
            PlayerUI pUI = playerUIInstance.GetComponent<PlayerUI>();


            if (pUI == null)
            {
                Debug.LogError("No PlyaerUI component on PlayerUI prefab.");
            }
            pUI.SetPlayer(GetComponent<PlayerManager>());

            GetComponent<PlayerManager>().SetupPlayer();


            CmdSetUserName(transform.name, CurrentUserInfo.info.userID);
            CmdSetUserInfo(transform.name, CurrentUserInfo.info);
        }

    }// start()                  

    void CullObject(GameObject[] _culledGraphics, int _Layer)
    {
        foreach (GameObject culledGraphic in _culledGraphics)
        {
            culledGraphic.layer = _Layer;
        }

    }
    public override void OnStartClient()  // It allows clients to be set up locally, it activated before start method
    {
        Debug.Log(this.gameObject.name + "'s OnStartClient activated!");
        base.OnStartClient();

        string _netID = "Player" + GetComponent<NetworkIdentity>().netId.ToString();
        PlayerManager _player = GetComponent<PlayerManager>();

        GameManager.RegisterPlayer(_netID, _player);


    }

    [Command]
    void CmdSetUserName(string _playerID, string _userName)
    {
        PlayerManager player = GameManager.GetPlayer(_playerID);

        if (player != null)
        {
            Debug.Log(_userName + " has joined");
            player.userName = _userName;
        }
    }

    [Command]
    void CmdSetUserInfo(string _playerID, UserData _userInfo)
    {
        PlayerManager player = GameManager.GetPlayer(_playerID);

        if (player != null)
        {
            Debug.Log(this.gameObject.name + " has assigned currentUserInfo");
            player.currentUserInfo = _userInfo;
        }
    }

    void OnDisable()
    {
        Debug.Log("OnDisable activated!");
        if (isLocalPlayer)
        {
            this.GetComponent<PlayerManager>().UpdateCurrentUserInfo();
            myDB.UpdateThisUserData(CurrentUserInfo.info);

            Cursor.lockState = CursorLockMode.None;
            Cursor.visible = true;

        }

        Destroy(playerUIInstance);

        if (isLocalPlayer) GameManager.instance.SetSceneCameraActive(true);


        GameManager.DeRegisterPlayer(transform.name);
    }

    void DisableComponents()
    {
        for (int i = 0; i < componentsToDisable.Length; i++)
        {
            componentsToDisable[i].enabled = false;
        }
    }

    void AssignRemoteLayer()
    {
        gameObject.layer = LayerMask.NameToLayer(remoteLayerName);
    }



}
