using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectManager
{
    //Dictionary<int, GameObject>
    List<GameObject> _objects = new List<GameObject>();

    public void Add(GameObject obj)
    {
        _objects.Add(obj);
    }

    public void Remove(GameObject obj)
    {
        _objects.Remove(obj);
    }

    public void Clear()
    {
        _objects.Clear();
    }
    
    // 해당 위치에 객체가 있는지 확인
    public GameObject Find(Vector3Int cellPos)
    {
        foreach (GameObject obj in _objects)
        {
            ActorController actorController = obj.GetComponent<ActorController>();
            if(actorController == null)
            {
                continue;
            }

            if(actorController.CellPos == cellPos)
            {
                return obj;
            }
        }

        return null;
    }

    // 찾기 조건에 따른 Find
    public GameObject Find(Func<GameObject, bool> condition)
    {
        foreach (GameObject obj in _objects)
        {
            ActorController actorController = obj.GetComponent<ActorController>();
            if (actorController == null)
            {
                continue;
            }

            if (condition.Invoke(obj))
            {
                return obj;
            }
        }

        return null;
    }
}
