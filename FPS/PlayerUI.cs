using UnityEngine;
using UnityEngine.UI;


public class PlayerUI : MonoBehaviour
{
    [SerializeField] RectTransform thrusterFuelBarAmount;
    [SerializeField] RectTransform healthBarAmount;

    public GameObject pauseMenu;
    [SerializeField] GameObject scoreBoard;
    public GameObject crossHair;
    public GameObject scopeOverlay;

    [SerializeField] Text ammoText;

    private PlayerManager player;
    private PlayerController playerController;
    private WeaponManager playerWeaponManager;

    private Image fuelImage;
    private Color originalFuelColor; // if fuel color is yellow, player cannot fly

    
    public void SetPlayer (PlayerManager _player)
    {
        player = _player;
        playerController = player.GetComponent<PlayerController>();
        playerWeaponManager = player.GetComponent<WeaponManager>();
    }
   
    void SetThrusterFuelAmount(float _amount)
    {
        thrusterFuelBarAmount.localScale = new Vector3(1f, _amount, 1f);
        
    }

    void SetHealthAmount(float _amount)
    {
        healthBarAmount.localScale = new Vector3(1f, _amount, 1f);
    }

    void SetAmmoAmount(int _amount)
    {
        ammoText.text = _amount.ToString();
    }
    private void Start()
    {
        PauseMenu.IsPauseOn = false;
        
        fuelImage = thrusterFuelBarAmount.gameObject.GetComponent<Image>();
        originalFuelColor = fuelImage.color;
    }

    private void Update()
    {
        SetThrusterFuelAmount(playerController.GetThrusterFuelAmount());
        SetHealthAmount(player.GetHealthPercentage());
        
        if(playerWeaponManager.GetCurrentWeapon() != null)
        {
            if(!crossHair.activeSelf) crossHair.SetActive(true);
            SetAmmoAmount(playerWeaponManager.GetCurrentWeapon().currentBullets);
        }
        else
        {
            crossHair.SetActive(false);
        }

        if (Input.GetKeyDown(KeyCode.Escape) && !scopeOverlay.activeSelf)
        {
            TogglePauseMenu();
        }
        if (Input.GetKeyDown(KeyCode.Tab))
        {
            scoreBoard.SetActive(true);
        }else if (Input.GetKeyUp(KeyCode.Tab))
        {
            scoreBoard.SetActive(false);
        }
        if(playerController.canFly == false)
        {
            fuelImage.color = new Color(255, 255, 0); // if you player cannot fly, fuel color is turned to yellow
        }
        else
        {
            fuelImage.color = originalFuelColor;
        }
    }

    void TogglePauseMenu()
    {
        pauseMenu.SetActive(!pauseMenu.activeSelf);
        PauseMenu.IsPauseOn = pauseMenu.activeSelf;
    }

   
}
