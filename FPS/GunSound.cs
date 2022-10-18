using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GunSound : MonoBehaviour
{
    public AudioClip reloaindgSound;

    public AudioSource audioSource;
   
    void ReloadingSound()
    {
        audioSource.PlayOneShot(reloaindgSound);
    }
}
