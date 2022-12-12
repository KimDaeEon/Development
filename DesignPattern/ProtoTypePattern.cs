using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 원형이 되는 인스턴스를 기반으로 해당 객체와 일치하는 상태를 갖고 있는 객체를 쉽게 생성하게 해주는 패턴
namespace ProtoTypePattern
{
    abstract class Monster
    {
        public abstract Monster Clone();
    }

    // 이제 몬스터 종류별로 Spawner 를 만들지 않아도 된다.
    class Spawner
    {
        public Spawner(Monster prototype)
        {
            _prototype = prototype;
        }

        public Monster spawnMonster()
        {
            // 아무 몬스터나 할당을 하게 되면 그 몬스터를 만드는 spawner 가 생성된다.
            // 해당 몬스터와 같은 '상태'까지 복사하는 것이 핵심.
            return _prototype.Clone();
        }

        private Monster _prototype;
    }

    class Ghost : Monster
    {
        public int Speed {get;set;}

        public Ghost(int speed)
        {
            Speed = speed;
        }
        public override Monster Clone()
        {
            // 자신의 상태를 가진 객체를 리턴
            return new Ghost(Speed);
        }
    }


}
