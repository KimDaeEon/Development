using DesignPattern;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 분야별로 개체를 분리하여 컴포넌트 클래스로 만들고, 개체 클래스는 이들 컴포넌트의 컨테이너 역할만 하는 패턴.
// 가능하다면 상속보다 이러한 컴포넌트 패턴을 쓰는 것이 더 좋다. 상속으로는 원하는 기능 부분만 골라서 사용하는 것이 까다롭기 때문이다.
namespace ComponentPattern
{
    // 만약 데모용 PhysicsComponent 가 따로 필요하다면, 아래 클래스를 abstract 로 바꾸고 Update 함수를 abstract 로 둔 후,
    // DemoPhysicsComponent 같은 클래스를 만들어서 실제 게임용과 시연용을 분리할 수도 있다.
    public class PhysicsComponent
    {
        public void Update(Player player)
        {
            Console.WriteLine("Do some physical jobs");
        }
    }
    
    public class GraphicsComponent
    {
        public void Update(Player player)
        {
            Console.WriteLine("Do some graphical jobs");
        }
    }
}
