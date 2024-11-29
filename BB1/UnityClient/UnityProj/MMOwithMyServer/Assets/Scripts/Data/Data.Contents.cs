using Google.Protobuf.Protocol;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Data
{
    #region Skill
    [Serializable]
	public class SkillData
	{
		public int id;
		public string name;
		public float cooldown;
		public int damage;
		public SkillType skillType;
		public ProjectileInfo projectile;
	}

	public class ProjectileInfo
	{
		public string name;
		public float speed;
		public int range;
		public string prefab;
	}

	[Serializable]
	public class SkillDataLoader : ILoader<int, SkillData>
	{
		public List<SkillData> skills = new List<SkillData>();

		public Dictionary<int, SkillData> MakeDict()
		{
			Dictionary<int, SkillData> dict = new Dictionary<int, SkillData>();
			foreach (SkillData skill in skills)
				dict.Add(skill.id, skill);
			return dict;
		}
	}
    #endregion

    #region Item
    [Serializable]
    public class ItemData
    {
        public int templateId;
        public string name; // TODO: 글로벌 서비스를 한다면 이런 이름 부분은 전부 국가별 string Id 로 빼야한다.
        public ItemType itemType;
        public string iconPath;
    }

    [Serializable]
    public class WeaponData : ItemData
    {
        public WeaponType weaponType;
        public int damage;
    }

    [Serializable]
    public class ArmorData : ItemData
    {
        public ArmorType armorType;
        public int defence;
    }

    [Serializable]
    public class ConsumableData : ItemData
    {
        public ConsumableType consumableType;
        public int maxCount;
    }

    [Serializable]
    public class ItemDataLoader : ILoader<int, ItemData>
    {
        public List<WeaponData> weapons = new List<WeaponData>();
        public List<ArmorData> armors = new List<ArmorData>();
        public List<ConsumableData> consumables = new List<ConsumableData>();

        public Dictionary<int, ItemData> MakeDict()
        {
            Dictionary<int, ItemData> dict = new Dictionary<int, ItemData>();

            foreach (ItemData item in weapons)
            {
                item.itemType = ItemType.Weapon;
                dict.Add(item.templateId, item);
            }

            foreach (ItemData item in armors)
            {
                item.itemType = ItemType.Armor;
                dict.Add(item.templateId, item);
            }

            foreach (ItemData item in consumables)
            {
                item.itemType = ItemType.Consumable;
                dict.Add(item.templateId, item);
            }

            return dict;
        }
    }
    #endregion

    #region Monster
    [Serializable]
    public class MonsterData
    {
        public int templateId;
        public string name; // TODO: 글로벌 서비스를 한다면 이런 이름 부분은 전부 국가별 string Id 로 빼야한다.
        public StatInfo stat;
        public string prefabPath;
    }

    [Serializable]
    public class MonsterDataLoader : ILoader<int, MonsterData>
    {
        public List<MonsterData> monsters = new List<MonsterData>();

        public Dictionary<int, MonsterData> MakeDict()
        {
            Dictionary<int, MonsterData> dict = new Dictionary<int, MonsterData>();
            foreach (MonsterData monster in monsters)
            {
                dict.Add(monster.templateId, monster);
            }
            return dict;
        }
    }
    #endregion
}