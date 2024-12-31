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
        public int GameRoomId { get; set; }
        public int DataSheetId { get; set; }

        Random _rand = new Random(); // 현재는 몬스터나 플레이어를 랜덤한 위치에 뿌리려고 쓰는데, TODO: 추후에 데이터로 다 빠지면 제거

        public MapComponent Map { get; private set; } = new MapComponent();
        public Zone[,] Zones { get; private set; } // 인근 오브젝트를 빠르게 찾기 위한 일종의 캐시이다

        public const int VisionCells = 10; // 플레이어가 자신을 중심으로 한 번에 볼 수 있는 셀 개수
        public int ZoneCells { get; private set; } // 하나의 존을 구성하는 셀 개수


        public void Init(int dataSheetId, int zoneCells)
        {
            DataSheetId = dataSheetId;
            ZoneCells = zoneCells; // 이게 10이면 X, Y 각각 10칸씩 갖는 Zone으로 맵이 쪼개진다.

            Map.LoadMap();

            // 맵 사이즈가 Cell이 1개라면 올림해서 Zone이 1개는 나와야 하므로 올림 연산이 되도록 이렇게 계산
            int zoneCountX = (Map.SizeX + ZoneCells - 1) / ZoneCells; 
            int zoneCountY = (Map.SizeY + ZoneCells - 1) / ZoneCells;

            Zones = new Zone[zoneCountX, zoneCountY];

            for (int x = 0; x < zoneCountX; x++)
            {
                for (int y = 0; y < zoneCountY; y++)
                {
                    Zones[x, y] = new Zone(x, y);
                }
            }
        }


        public void Update()
        {
            Flush();
        }
    }
}
