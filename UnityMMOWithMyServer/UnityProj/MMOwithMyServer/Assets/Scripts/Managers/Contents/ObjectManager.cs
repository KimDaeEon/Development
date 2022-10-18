using Google.Protobuf.Protocol;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectManager
{
    //Dictionary<int, GameObject>
    public MyPlayerController MyPlayer { get; set; }
    Dictionary<int, GameObject> _objects = new Dictionary<int, GameObject>();

    public static ActorType GetActorTypeById(int id)
    {
        return (ActorType)(id >> 24 & 0x7F); // Ȥ�� ���� ��Ʈ ����
    }

    // TODO: �̷� ������ �� ���� ���� �߰��ϴ°� �³�??
    public void Add(ActorInfo info, bool isMyPlayer = false)
    {
        ActorType actorType = GetActorTypeById(info.ActorId);
        if (actorType == ActorType.Player)
        {
            if (isMyPlayer)
            {
                GameObject obj = Managers.Resource.Instantiate("Actor/MyPlayer");
                obj.name = info.Name;
                _objects.Add(info.ActorId, obj);

                MyPlayer = obj.GetComponent<MyPlayerController>();
                MyPlayer.Id = info.ActorId;
                MyPlayer.PosInfo = info.PosInfo;
                MyPlayer.Stat = info.StatInfo;

                MyPlayer.SyncPosInstantly();
            }
            else
            {
                GameObject obj = Managers.Resource.Instantiate("Actor/Player");
                obj.name = info.Name;
                _objects.Add(info.ActorId, obj);

                PlayerController pc = obj.GetComponent<PlayerController>();
                pc.Id = info.ActorId;
                pc.PosInfo = info.PosInfo;
                pc.Stat = info.StatInfo;

                pc.SyncPosInstantly();
            }
        }
        else if(actorType == ActorType.Monster)
        {
            GameObject obj = Managers.Resource.Instantiate("Actor/Monster");
            obj.name = info.Name;
            _objects.Add(info.ActorId, obj);

            MonsterController mc = obj.GetComponent<MonsterController>();
            mc.Id = info.ActorId;
            mc.PosInfo = info.PosInfo;
            mc.Stat = info.StatInfo;

            mc .SyncPosInstantly();
        }
        else if(actorType == ActorType.Projectile)
        {
            // TODO: ���߿��� ���������� projectile �� �����ؼ� �����ϴ� ��������..
            GameObject obj = Managers.Resource.Instantiate("Actor/Arrow");
            obj.name = "Arrow" + info.ActorId;
            _objects.Add(info.ActorId, obj);

            ArrowController ac = obj.GetComponent<ArrowController>();
            ac.PosInfo = info.PosInfo;
            ac.Stat = info.StatInfo;
            ac.SyncPosInstantly();
        }
    }
    public void Add(int id, GameObject obj)
    {
        _objects.Add(id, obj);
    }

    public bool Remove(int id)
    {
        GameObject obj = FindById(id);
        if (obj == null)
        {
            return false;
        }

        _objects.Remove(id);
        Managers.Resource.Destroy(obj);
        return true;
    }

    public void Clear()
    {
        foreach (var obj in _objects.Values)
        {
            Managers.Resource.Destroy(obj);
        }

        _objects.Clear();
        MyPlayer = null;
    }

    public GameObject FindById(int id)
    {
        GameObject obj = null;
        _objects.TryGetValue(id, out obj);

        return obj;
    }

    // �ش� ��ġ�� ���Ͱ� �ִ��� Ȯ��
    public GameObject FindActorByCellPos(Vector3Int cellPos)
    {
        foreach (GameObject obj in _objects.Values)
        {
            ActorController actorController = obj.GetComponent<ActorController>();
            if (actorController == null)
            {
                continue;
            }

            if (actorController.CellPos == cellPos)
            {
                return obj;
            }
        }

        return null;
    }

    // ã�� ���ǿ� ���� Find
    public GameObject FindActorByConditionFunc(Func<GameObject, bool> condition)
    {
        foreach (GameObject obj in _objects.Values)
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
