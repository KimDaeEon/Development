using System;

namespace GameServer
{
    // 일회성 상호 작용
    // 충돌 처리 X (Actor에 충돌 시 사라짐)
    public class Projectile : Entity
    {
        protected Projectile()
        {
            // 필요하면 Component 추가
            // AddComponent(new AIComponent()); // Projectile도 AIComponent를 포함 가능
        }

        public override void Init(ulong dataSheetId)
        {
            DataSheetId = dataSheetId;

            // TODO: 데이터 세팅
        }
    }
}
