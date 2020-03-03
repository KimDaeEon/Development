using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class Scope : NetworkBehaviour
{

    public Animator animator;
    public GameObject scopeOverlay;
    public GameObject weaponCamera;
    public Camera thisMainCamera;
    public GameObject playerOBJ;


    private bool isScopeAllocated = false;
    private bool isWeaponCameraAllocated = false;
    private bool isPlayerOBJAllocated = false;

    private bool isLocalPlayerScopeVersion = false;

    public bool isScoped = false;
    private float scopedFOV = 10f;
    private float previousFOV;

    // Update is called once per frame
    void Update()
    {
        
        if (!isPlayerOBJAllocated)
        {
            playerOBJ = this.transform.parent.parent.parent.gameObject; // this is playerObject

            if (playerOBJ != null)
            {
                isPlayerOBJAllocated = true;
                if(playerOBJ.layer == LayerMask.NameToLayer("LocalPlayer"))
                {
                    isLocalPlayerScopeVersion = true;
                }
            }
            else
            {
                Debug.Log("Cannot FInd PlayerObject!");
            }
        }

        if (!isLocalPlayerScopeVersion)
        {
            //if it is not local player, scope doesn't activated
            return;
        }


        if (!isScopeAllocated)
        {
            GameObject UIObject = GameObject.FindGameObjectWithTag("PlayerUI");
            scopeOverlay = UIObject.GetComponent<PlayerUI>().scopeOverlay;
            

            if (scopeOverlay != null)
            {
                isScopeAllocated = true;
            }
        }

        if (!isWeaponCameraAllocated)
        {
            weaponCamera = this.transform.parent.parent.GetComponentsInChildren<Camera>()[1].gameObject;
            if (weaponCamera != null)
            {
                thisMainCamera = this.transform.parent.parent.GetComponent<Camera>();

                previousFOV = thisMainCamera.fieldOfView;
                isWeaponCameraAllocated = true;
            }
        }

        



        if (scopeOverlay == null)
        {
            return;
        }
        if (Input.GetButtonDown("Fire2") && !PauseMenu.IsPauseOn)
        {
            //Debug.Log("GetButtonDown is true");
            isScoped = !isScoped;
            animator.SetBool("Scoped", isScoped); // scope animation activated
            if (isScoped)
            {
                StartCoroutine(onScoped());
            }
            else
            {
                onUnScoped();
            }
        }








    }
    IEnumerator onScoped()
    {
        yield return new WaitForSeconds(0.2f);
        scopeOverlay.SetActive(isScoped); // scopeOverLay appears
        weaponCamera.SetActive(!isScoped); // Gun disappears
        thisMainCamera.fieldOfView = scopedFOV; // Main Camera zooms in
    }

    public void onUnScoped()
    {
        Debug.Log("OnUnScoped activated!");
        scopeOverlay.SetActive(isScoped); // scopeOverLay disappears
        weaponCamera.SetActive(!isScoped); // Gun appears
        thisMainCamera.fieldOfView = previousFOV; // Revert FOV values of main camera
    }


}
