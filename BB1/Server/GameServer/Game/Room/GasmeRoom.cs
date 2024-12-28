using Microsoft.EntityFrameworkCore.ChangeTracking;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // 맵 관련 정보 저장 필요
    // 맵 정보를 Zone 으로 쪼개서 저장 필요
    // 플레이어 이동 시에 Zone 이동하는 기능 구현 필요
    // 사용자 입장 및 퇴직 처리 관련 로직 구현 필요
    // Room 내의 존재에게 패킷을 보낼 때에 Zone을 계산해서 보내는 기능 구현 필요

    public partial class GameRoom : JobQueue
    {
        public uint GameRoomId {  get; set; }
        public uint DataSheetId { get; set; }

        Dictionary<uint, Entity> _entites = new Dictionary<uint, Entity>();


        public void Update()
        {
            Flush();
        }
    }
}
