using DesignPattern;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 상태에 기능들을 집중하여, 객체 내부 상태에 따라 행동 방식을 변경하도록 해주는 패턴
namespace StatePattern
{
    public abstract class State
    {
        public abstract void HandleInput(Player player, object input);
        public abstract void Update(Player player);
    }
    // 계층화된 상태
    public class NormalState: State
    {
        public override void HandleInput(Player player, object input)
        {
            Console.Write($"{this} HandleInput");
        }

        public override void Update(Player player)
        {
            Console.WriteLine($"{this} Update");
        }
    }

    // 상태별로 해당 상태에 해야할 행동들이 집약되어 있다.
    public class IdleState : NormalState
    {
        public override void HandleInput(Player player, object input)
        {
            Console.Write($"{this} HandleInput");
        }

        public override void Update(Player player)
        {
            Console.WriteLine($"{this} Update");
        }
    }

    public class CrowdControlState : State
    {
        public override void HandleInput(Player player, object input)
        {
            Console.Write($"{this} HandleInput");
        }

        public override void Update(Player player)
        {
            Console.WriteLine($"{this} Update");
        }
    }

    // 계층화된 상태
    public class StunState : CrowdControlState
    {
        public override void HandleInput(Player player, object input)
        {
            base.HandleInput(player, input);
            Console.Write($"{this} HandleInput");
        }

        public override void Update(Player player)
        {
            base.Update(player);
            Console.WriteLine($"{this} Update");
        }
    }
}
