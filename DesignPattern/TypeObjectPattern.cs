using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 클래스 하나를 인스터별로 다른 객체형으로 표현할 수 있게 만들어, 새로운 클래스들을 유연하게 만들 수 있게 하는 패턴
namespace TypeObjectPattern
{
    // 아래의 클래스를 데이터에 따라 여러 개 인스턴스화시켜서 Monster를 생성한다.
    // 상속 관계를 사용한 비슷한 코드가 반복될 때에 아래와 같이 만들면 코드를 여러번 치지 않고 여러 개의 Monster 를 만들 수 있다.
    public class Breed
    {
        public Breed(int health, int attack, Breed parent = null)
        {
            Health = health;
            Attack = attack;
        }

        public int Attack { 
            get 
            {
                if(_parent == null)
                {
                    return _attack;
                }

                return _parent.Attack;
            }

            private set
            {
                _health = value;
            }
        } 

        public int Health { 
            get 
            {
                if(_parent == null)
                {
                    return _health;
                }

                return _parent.Health;
            }

            private set
            {
                _health = value;
            }
        }

        // 객체의 상속관계를 아래와 같은 형식으로 대체한다.
        private Breed _parent = null;

        private int _attack = 0;
        private int _health = 0;
    }

    class Monster
    {
        private Breed breed;
        public int Health {
            get
            {
                return breed.Health;
            }
        }
        public int Attack {
            get
            {
                return breed.Attack;
            }
        }

        public Monster(Breed breed)
        {
            this.breed = breed;
        }
    }
}
