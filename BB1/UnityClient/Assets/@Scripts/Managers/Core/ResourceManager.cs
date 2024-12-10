using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AddressableAssets;
using Object = UnityEngine.Object;

public class ResourceManager
{
    // 실제 로드한 리소스.
    private Dictionary<string, UnityEngine.Object> _resources = new Dictionary<string, UnityEngine.Object>();

    #region 리소스 로드
    public bool CheckResource<T>(string key) where T : Object
    {
        if (_resources.TryGetValue(key, out Object resource))
        {
            return true;
        }

        if (typeof(T) == typeof(Sprite))
        {
            key = key + ".sprite";
            if (_resources.TryGetValue(key, out Object temp))
            {
                return true;
            }
        }

        return false;
    }

    public T Get<T>(string key) where T : Object
    {
        if (_resources.TryGetValue(key, out Object resource))
        {
            return resource as T;
        }

        //스프라이트 로드할때 항상 .sprite가 붙어 있어야하는데 데이터시트에 .sprite가 붙어있지 않은 데이터가 많음
        //임시로 붙임 -드래곤
        if (typeof(T) == typeof(Sprite))
        {
            key = key + ".sprite";
            if (_resources.TryGetValue(key, out Object temp))
            {
                return temp as T;
            }
        }

        return null;
    }

    public GameObject Instantiate(string key, Transform parent = null, bool pooling = false)
    {
        GameObject prefab = Get<GameObject>($"{key}");
        if (prefab == null)
        {
            Debug.LogError($"Failed to load prefab : {key}");
            return null;
        }

        if (pooling)
        {
            return Managers.Pool.Pop(prefab);
        }

        GameObject go = Object.Instantiate(prefab, parent);

        go.name = prefab.name;
        return go;
    }

    public void Destroy(GameObject go)
    {
        if (go == null)
            return;

        if (Managers.Pool.Push(go))
            return;

        Object.Destroy(go);
    }

    #endregion

    #region Addressable

    public void LoadDataAsync<T>(string key, Action<T> callback = null) where T : UnityEngine.Object
    {
        //스프라이트인 경우 하위객체의 이름으로 로드하면 스프라이트로 로딩이 됌
        string loadKey = key;
        if (key.Contains(".sprite"))
        {
            // Addressable에서 복합 리소스에 대해서, [name]을 사용하여 특정 하위 항목에 접근 가능
            // Characters.sprite (Sprite Atlas)
            // ├── Player(하위 Sprite)
            // ├── Enemy(하위 Sprite)

            // var atlasHandle = Addressables.LoadAssetAsync<SpriteAtlas>("Characters.sprite"); 인 경우 전체 Sprite Atlas 로드
            // var spriteHandle = Addressables.LoadAssetAsync<Sprite>("Characters.sprite[Player]"); 인 경우 Atlas 내부의 Player Sprite를 로드

            // 복합 리소스 종류
            // Sprite Atlas[Sprite], Prefab[Child], Animator Controller[Animation Clip], Audio Mixer[Audio Group], Text Asset or JSON[JSON key], Material[SomeTexture]
            loadKey = $"{key}[{key.Replace(".sprite", "")}]";
        }

        var asyncOperation = Addressables.LoadAssetAsync<T>(loadKey);
        asyncOperation.Completed += (op) =>
        {
            if (_resources.TryGetValue(key, out Object resource))
            {
                callback?.Invoke(op.Result);
                return;
            }

            _resources.Add(key, op.Result);
            callback?.Invoke(op.Result);
        };
    }

    public void LoadAllDataAsync<T>(string label, Action<string, int, int> callback) where T : UnityEngine.Object
    {
        var opHandle = Addressables.LoadResourceLocationsAsync(label, typeof(T));
        opHandle.Completed += (op) =>
        {
            int loadCount = 0;

            int totalCount = op.Result.Count;

            foreach (var result in op.Result)
            {
                if (result.PrimaryKey.Contains(".sprite"))
                {
                    LoadDataAsync<Sprite>(result.PrimaryKey, (obj) =>
                    {
                        loadCount++;
                        callback?.Invoke(result.PrimaryKey, loadCount, totalCount);
                    });
                }
                else
                {
                    LoadDataAsync<T>(result.PrimaryKey, (obj) =>
                    {
                        loadCount++;
                        callback?.Invoke(result.PrimaryKey, loadCount, totalCount);
                    });
                }
            }
        };
    }

    #endregion
}
