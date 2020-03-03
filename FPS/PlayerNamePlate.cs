using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerNamePlate : MonoBehaviour
{
    [SerializeField] Text usernameText;

    [SerializeField] PlayerManager player;

    [SerializeField] private RectTransform healthBarFill;

    // Update is called once per frame
    void Update()
    {
        Camera cam = Camera.main;

        usernameText.text = player.userName;
        if (cam != null)
        {
            transform.LookAt(transform.position + cam.transform.rotation * Vector3.forward,
       cam.transform.rotation * Vector3.up);
        }
        healthBarFill.localScale = new Vector3(1f, player.GetHealthPercentage(), 1f);



        //Debug.Log(this.gameObject.transform.rotation);

    }
}
