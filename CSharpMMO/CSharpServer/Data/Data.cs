using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// TODO: 아래 데이터 파일들 지금 Json 에서 읽어오는데, Excel 에서 읽어와서 json 으로 바꾸도록 만들자. EPPlus 가 좋다고 하니 추후 적용.
// 파일에서 읽어온 데이터를 갖고 있는 구조체
// Game 내부 폴더에 있는 데이터가 게임에서 사용되기 위한 데이터라는 면에서 구별된다.
// [Serializable] 이 없어도 서버에서 데이터 파싱이 되지만, Unity 의 JsonParser 는 해당 attribute 가 없으면 파싱이 안된다.
namespace CSharpServer.Data
{
    #region StatInfo
    [Serializable]
    public class StatDataLoader : ILoader<int, StatInfo>
    {
        public List<StatInfo> stats = new List<StatInfo>();

        public Dictionary<int, StatInfo> MakeDict()
        {
            Dictionary<int, StatInfo> dict = new Dictionary<int, StatInfo>();
            foreach (StatInfo stat in stats)
            {
                stat.Hp = stat.MaxHp;
                dict.Add(stat.Level, stat);
            }
            return dict;
        }
    }
    #endregion

    #region Skill
    [Serializable]
    public class SkillData
    {
        public int id;
        public string name;
        public float cooltime;
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
    }

    public class WeaponData : ItemData
    {
        public WeaponType weaponType;
        public int damage;
    }

    public class ArmorData : ItemData
    {
        public ArmorType armorType;
        public int defence;
    }

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
    public class RewardData
    {
        public int itemTemplateId; // 아이템 데이터 ID
        public int probability; // 1000 분율
        public int count; 
    }

    [Serializable]
    public class MonsterData
    {
        public int templateId;
        public string name; // TODO: 글로벌 서비스를 한다면 이런 이름 부분은 전부 국가별 string Id 로 빼야한다.
        public StatInfo stat;
        public List<RewardData> rewards;
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
