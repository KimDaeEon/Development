using DesignPattern;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 객체의 상태가 변할 때에 그 객체에 의존성을 가진 객체들이 변화를 통지받고 자동으로 업데이트 될 수 있도록 해주는 패턴
namespace ObserverPattern
{
    public enum EventType
    {
        KILL_MONSTER,
        GET_ITEM,
    }

    public class Event
    {
        public EventType Type { get; set; }
        public object Info { get; set; }
    }

    public interface Observer
    {
        void OnNotify(Player player, Event evt);
    }

    public class Achievements : Observer
    {
        public void OnNotify(Player player, Event evt)
        {
            Console.WriteLine("Achievements Event Processed");
        }
    }
}
