using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;

public class WeaponPick : NetworkBehaviour
{
    public GameObject weaponPickCanvas;
    public GameObject interactingObject;
   
    public GameObject weaponPrefab;


    public PlayerWeapon weapon;

    [SerializeField] Text weaponNameText;
    [SerializeField] Text itemPickUpText;

    public GameObject willbeRemoved;



    private void Start()
    {
        weapon = weaponPrefab.GetComponent<PlayerWeapon>();
    }

    void Update()
    {
        if (!weaponPickCanvas.activeSelf)
        { return; }

        willbeRemoved = this.transform.parent.gameObject;

        Camera cam = Camera.main;

        weaponNameText.text = weapon.name;
        itemPickUpText.text = "Press 'E' to get '" + weapon.name + "'";
        if (cam != null)
        {
            transform.LookAt(transform.position + cam.transform.rotation * Vector3.forward,
       cam.transform.rotation * Vector3.up);
        }

        if (Input.GetKeyDown(KeyCode.E))
        {
            if (interactingObject != null)
            {
               // interactingObject.GetComponent<WeaponManager>().GetWeapon(this.gameObject);
                interactingObject.GetComponent<PlayerManager>().PickWeapon(willbeRemoved);
            }
        }


    }

}
