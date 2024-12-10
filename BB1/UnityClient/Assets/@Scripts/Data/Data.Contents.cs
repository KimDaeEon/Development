using System;
using System.Collections.Generic;
using UnityEngine;
using static CustomEnum;

namespace Data
{
    #region ActorData
    [Serializable]
    public class ActorData
    {
        public int TemplateId;
        public string NameTextID;
        public float ColliderOffsetX;
        public float ColliderOffsetY;
        public float ColliderRadius;
        public float MaxHp;
        public float UpMaxHpBonus;
        public float Atk;
        public float MissChance;
        public float AtkBonus;
        public float MoveSpeed;
        public float CriRate;
        public float CriDamage;
        public string IconImage;
        public string SkeletonDataID;
        public int DefaultSkillId;
        public int EnvSkillId;
        public int SkillAId;
        public int SkillBId;
       
    }

    [Serializable]
    public class ActorDataLoader : ILoader<int, ActorData>
    {
        public List<ActorData> actors = new List<ActorData>();
        public Dictionary<int, ActorData> MakeDict()
        {
            Dictionary<int, ActorData> dict = new Dictionary<int, ActorData>();
            foreach (ActorData actor in actors)
                dict.Add(actor.TemplateId, actor);
            return dict;
        }

        public bool Validate()
        {
            return true;
        }
    }
    #endregion

}