using System;

namespace GameServer
{
    // 습득 외의 상호 작용 불가
    // 충돌 처리 없음
    public class Item : Entity
    {
        protected Item()
        {
            // 필요하면 Component 추가
            // AddComponent(new SkillComponent()); // Item도 SkillComponent를 포함할 수 있음
        }

        public override void Init(ulong dataSheetId)
        {
            DataSheetId = dataSheetId;

            // TODO: 데이터 세팅
        }
    }
}
