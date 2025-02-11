using System;

namespace GameServer
{
    public class Monster : Actor
    {
        protected Monster()
        {
            AddComponent(new AIComponent()); // ✅ 필요할 때만 추가
        }

        public override void Init(ulong dataSheetId)
        {
            base.Init(dataSheetId);
            DataSheetId = dataSheetId;

            // TODO: 데이터 세팅
        }
    }
}
