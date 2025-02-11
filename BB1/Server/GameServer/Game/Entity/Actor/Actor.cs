using System;

namespace GameServer
{
    // 주체적으로 상호 작용
    // 충돌 처리
    public class Actor : Entity
    {
        protected Actor()
        {
            AddComponent(new SkillComponent()); // ✅ Entity의 _entityComponents 활용
        }

        public override void Init(ulong dataSheetId)
        {
            DataSheetId = dataSheetId;

            // TODO: 데이터 세팅
        }
    }
}
