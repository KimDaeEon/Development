using ComponentPattern;
using ObserverPattern;
using SandBoxPattern;
using StatePattern;
using System;
using System.Collections.Generic;

namespace DesignPattern
{
    public class Player
    {
        public static IdleState IdleState { get; private set; } = new IdleState();
        public static StunState StunState { get; private set; } = new StunState();

        public Player()
        {
            addObserver(new Achievements());
            _state = IdleState;
        }

        public void addObserver(Observer observer)
        {
            _evtObserver.Add(observer);
        }

        public void removeAllObservers()
        {
            _evtObserver.Clear();
        }

        public void KillMonster()
        {
            Console.WriteLine("Monster Killed");
            Event monsterKillEvent = new Event() { Type = EventType.KILL_MONSTER };
            Notify(monsterKillEvent);
        }

        // 아래를 통해 이벤트를 처리하는 모든 Observer에 작업이 전달되어서 처리
        protected void Notify(Event evt)
        {
            foreach (Observer observer in _evtObserver)
            {
                observer.OnNotify(this, evt);
            }
        }


        public void Update()
        {
            Console.WriteLine($"{this} Update");
            KillMonster();
            _state.Update(this);

            _physics.Update(this);
            _graphics.Update(this);
        }

        public void HandleInput(object input)
        {
            _state.HandleInput(this, input);
        }

        State _state; // 게임에 적용할 때에는 Stack 으로 만들어서 상태를 바꿀 수 있도록 하자.
        List<Observer> _evtObserver = new List<Observer>();

        // 컴포넌트를 자주 바꿔야 한다면, 인터 페이스나 abstract class를 통해서 구현한 컴포넌트를 주입받도록 해야한다.
        GraphicsComponent _graphics = new GraphicsComponent();
        GraphicsComponent _physics = new GraphicsComponent();
    }

    class Program
    {
        static void Main(string[] args)
        {
            Player player = new Player();

            while (true)
            {
                player.Update();
            }

        }
    }
}
