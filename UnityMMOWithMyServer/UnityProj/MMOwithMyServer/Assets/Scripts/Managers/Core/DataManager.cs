﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface ILoader<Key, Value>
{
    Dictionary<Key, Value> MakeDict();
}

public class DataManager
{
    public Dictionary<int, Data.SkillData> SkillDict { get; private set; } = new Dictionary<int, Data.SkillData>();
    public Dictionary<int, Data.ItemData> ItemDict { get; private set; } = new Dictionary<int, Data.ItemData>();
    public Dictionary<int, Data.MonsterData> MonsterDict { get; private set; } = new Dictionary<int, Data.MonsterData>();

    public void Init()
    {
        SkillDict = LoadJson<Data.SkillDataLoader, int, Data.SkillData>("SkillData").MakeDict();
        ItemDict = LoadJson<Data.ItemDataLoader, int, Data.ItemData>("ItemData").MakeDict();
        MonsterDict = LoadJson<Data.MonsterDataLoader, int, Data.MonsterData>("ItemData").MakeDict();
    }

    Loader LoadJson<Loader, Key, Value>(string path) where Loader : ILoader<Key, Value>
    {
		TextAsset textAsset = Managers.Resource.Load<TextAsset>($"Data/{path}");
        return Newtonsoft.Json.JsonConvert.DeserializeObject<Loader>(textAsset.text);
	}
}
