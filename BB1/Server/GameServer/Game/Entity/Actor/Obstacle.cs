using System;

namespace GameServer
{
    // 수동적인 상호 작용만 가능 (폭발, 파괴, 밀려서 이동 등등)
    // 충돌 처리
    public class Obstacle : Actor
    {
        protected Obstacle()
        {
            // Obstacle은 별다른 컴포넌트가 필요 없을 수도 있음
        }

        public override void Init(ulong dataSheetId)
        {
            base.Init(dataSheetId);

            // TODO: 데이터 세팅
        }
    }
}
