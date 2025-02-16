using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;
using Google.Protobuf.Protocol;

public class GameObjectManager
{
    #region Roots
    public Transform GetRootTransform(string name)
    {
        GameObject root = GameObject.Find(name);
        if (root == null)
            root = new GameObject { name = name };

        return root.transform;
    }

    public Transform CharacterRoot { get { return GetRootTransform("@Characters"); } }
    public Transform MonsterRoot { get { return GetRootTransform("@Monsters"); } }
    public Transform ProjectileRoot { get { return GetRootTransform("@Projectiles"); } }
    public Transform EnvRoot { get { return GetRootTransform("@Envs"); } }
    public Transform EffectRoot { get { return GetRootTransform("@Effects"); } }
    public Transform NpcRoot { get { return GetRootTransform("@Npc"); } }
    public Transform ItemHolderRoot { get { return GetRootTransform("@ItemHolders"); } }
    #endregion

    public GameObjectManager()
    {
    }

    public MyCharacter MyCharacter { get; set; }

    private Dictionary<ulong, GameObject> _objects = new Dictionary<ulong, GameObject>();

    public GameObject FindByGameId(ulong id)
    {
        GameObject go = null;
        _objects.TryGetValue(id, out go);
        return go;
    }

    public void Clear()
    {
        foreach (GameObject obj in _objects.Values)
            Managers.Resource.Destroy(obj);

        _objects.Clear();
        MyCharacter = null;
    }

    public MyCharacter SpawnMyCharacter(Protocol.EntityInfo entityInfo)
    {
        // ���⼭ CharacterInfo �´��� üũ
        var charInfo = entityInfo?.ActorInfo.CharacterInfo;

        if (charInfo == null)
        {
            Debug.LogError("Invalid entityInfo for MyCharacter");
            return null;
        }

        // TODO: ���� ������ Sheet ������� Instanitate �ϴ� �� �߰� �ʿ�
        GameObject go = Managers.Resource.Instantiate("KnightPrefab");
        go.name = charInfo.Name;
        go.transform.parent = CharacterRoot;

        MyCharacter = Utils.GetOrAddComponent<MyCharacter>(go);
        MyCharacter.EntityInfo = entityInfo;

        _objects.Add(entityInfo.GameId, go);

        return MyCharacter;
    }

    public Entity Spawn(Protocol.EntityInfo entityInfo)
    {
        if (entityInfo == null)
        {
            Debug.LogError("Invalid entityInfo");
            return null;
        }

        // Character���� Monster���� �Ǵ�
        Entity entity = null;
        GameObject go = null;

        // TODO: �������� �����ͷ� �ٸ� �ɷ� �����ϴ� ���� �߰� �ʿ�
        if (entityInfo.ActorInfo.CharacterInfo != null)
        {
            go = Managers.Resource.Instantiate("KnightPrefab"); // ĳ���� ������
            go.transform.parent = CharacterRoot;
            go.name = entityInfo.GameId.ToString();
            entity = Utils.GetOrAddComponent<Character>(go);

        }
        else if (entityInfo.ActorInfo.MonsterInfo != null)
        {
            go = Managers.Resource.Instantiate("KnightPrefab"); // ���� ������ ���� ���� �ʿ�
            go.transform.parent = MonsterRoot;
            go.name = entityInfo.GameId.ToString();
            entity = Utils.GetOrAddComponent<Monster>(go);

        }
        else
        {
            Debug.LogError("Unknown entity type");
            return null;
        }

        _objects.Add(entityInfo.GameId, go);

        entity.EntityInfo = entityInfo;

        return entity;
    }

    public void Despawn(List<ulong> gameIds)
    {
        foreach (ulong gameId in gameIds)
        {
            if (_objects.TryGetValue(gameId, out GameObject go))
            {
                Managers.Resource.Destroy(go);

                // TODO: ���⼭ MyCharacter ������ ���� �ʿ��� ���� ����ٸ� �߰�����.
                _objects.Remove(gameId);
            }
        }
    }
}