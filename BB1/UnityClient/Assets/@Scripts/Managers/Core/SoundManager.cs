using System;
using System.Collections.Generic;
using UnityEngine;

// TODO: 얘만 지연 로딩하게 구현해두셨는데, 이게 클라쪽에서 범용적인 코드인가? 나중에 클라분들 만나면 질문드리자.
public class SoundManager
{
    private AudioSource[] _audioSources = new AudioSource[(int)CustomEnum.ESound.Max];
    private Dictionary<string, AudioClip> _audioClips = new Dictionary<string, AudioClip>();

    private GameObject _soundRoot = null;

    public void Init()
    {
        if (_soundRoot == null)
        {
            // 폴더 이름에서 찾는게 아니라 활성화된 Scene에서 이름이 @SoundRoot인 애를 찾는 것이다.
            _soundRoot = GameObject.Find("@SoundRoot");
            if (_soundRoot == null)
            {
                _soundRoot = new GameObject { name = "@SoundRoot" };
                UnityEngine.Object.DontDestroyOnLoad(_soundRoot);

                string[] soundTypeNames = System.Enum.GetNames(typeof(CustomEnum.ESound));
                for (int count = 0; count < soundTypeNames.Length - 1; count++)
                {
                    GameObject go = new GameObject { name = soundTypeNames[count] };
                    _audioSources[count] = go.AddComponent<AudioSource>();
                    go.transform.parent = _soundRoot.transform;
                }

                _audioSources[(int)CustomEnum.ESound.Bgm].loop = true;
                _audioSources[(int)CustomEnum.ESound.SubBgm].loop = true;
            }
        }
    }

    public void Clear()
    {
        foreach (AudioSource audioSource in _audioSources)
        {
            audioSource.Stop();
        }
        _audioClips.Clear();
    }

    public void Play(CustomEnum.ESound type)
    {
        AudioSource audioSource = _audioSources[(int)type];
        audioSource.Play();
    }

    public void Play(CustomEnum.ESound type, string key, float pitch = 1.0f)
    {
        AudioSource audioSource = _audioSources[(int)type];

        if (type == CustomEnum.ESound.Bgm)
        {
            PlayOrLoadIfNeeded(key, (audioClip) =>
            {
                if (audioSource.isPlaying)
                {
                    audioSource.Stop();
                }

                audioSource.clip = audioClip;
                audioSource.pitch = pitch;
                audioSource.Play();
            });
        }
        else if (type == CustomEnum.ESound.SubBgm)
        {
            PlayOrLoadIfNeeded(key, (audioClip) =>
            {
                if (audioSource.isPlaying)
                {
                    audioSource.Stop();
                }

                audioSource.clip = audioClip;
                audioSource.pitch = pitch;
                audioSource.Play();
            });
        }
        else
        {
            PlayOrLoadIfNeeded(key, (audioClip) =>
            {
                audioSource.pitch = pitch;
                audioSource.PlayOneShot(audioClip);
            });
        }
    }

    public void Play(CustomEnum.ESound type, AudioClip audioClip, float pitch = 1.0f)
    {
        AudioSource audioSource = _audioSources[(int)type];

        if (type == CustomEnum.ESound.Bgm)
        {
            if (audioSource.isPlaying)
                audioSource.Stop();

            audioSource.clip = audioClip;
            audioSource.pitch = pitch;
            //if (Managers.Game.BGMOn)
            audioSource.Play();
        }
        else if (type == CustomEnum.ESound.SubBgm)
        {
            if (audioSource.isPlaying)
                audioSource.Stop();

            audioSource.clip = audioClip;
            //if (Managers.Game.EffectSoundOn)
            audioSource.Play();
        }
        else
        {
            audioSource.pitch = pitch;
            //if (Managers.Game.EffectSoundOn)
            audioSource.PlayOneShot(audioClip);
        }
    }

    public void Stop(CustomEnum.ESound type)
    {
        AudioSource audioSource = _audioSources[(int)type];
        audioSource.Stop();
    }

    public void PlayButtonClick()
    {
        Play(CustomEnum.ESound.Effect, "Click_CommonButton");
    }

    public void PlayPopupClose()
    {
        Play(CustomEnum.ESound.Effect, "PopupClose_Common");
    }

    private void PlayOrLoadIfNeeded(string key, Action<AudioClip> callback)
    {
        AudioClip audioClip = null;
        if (_audioClips.TryGetValue(key, out audioClip))
        {
            callback?.Invoke(audioClip);
            return;
        }

        audioClip = Managers.Resource.Get<AudioClip>(key);

        if (!_audioClips.ContainsKey(key))
        {
            _audioClips.Add(key, audioClip);
        }

        callback?.Invoke(audioClip);
    }
}
