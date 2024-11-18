using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 부모클래스에서 제공해주는 protected 함수(기능들)로 하위 클래스의 기능을 만드는 패턴.
// 하위 클래스 기능이 부모 클래스의 기능들을 조합적으로 사용하고 많은 클래스가 만들어질 것으로 예상이 될 때에 사용하면 좋다.
namespace SandBoxPattern
{
    // 안에서 소리를 실행하고, 이펙트를 실행하는 등의 대규모 작업을 하는 클래스
    public abstract class SuperPower
    {
        // 아래의 반드시 구현해야 하는 메서드를 'SandBox Method' 라고 한다.
        protected abstract void Activate();

        // 아래는 상위 클래스에서 제공되는 기능들
        protected void Move(double x, double y, double z)
        {
            Console.WriteLine($"Move to {x}, {y}, {z}");
        }

        protected void PlaySound(int soundId, int volume)
        {
            _soundPlayer.PlaySound(soundId, volume);
        }
        
        protected void PlayEffect(object effect)
        {
            _effectSpanwer.PlayEffect(effect);
        }

        private SoundPlayer _soundPlayer = new SoundPlayer();
        private EffectSpanwer _effectSpanwer = new EffectSpanwer();


        // 부모 클래스의 함수가 많아지고 기능이 분리되어 있다면 이렇게 보조 클래스를 만드는 것이 좋다.
        // 이를 통해 보조 시스템과 상위 시스템의 결합도를 낮출 수 있다.
        private class SoundPlayer
        {
            public void PlaySound(int soundId, int volume)
            {
                Console.WriteLine($"Sound: {soundId} on {volume}");
            }
        }

        private class EffectSpanwer
        {
            public void PlayEffect(object effect)
            {
                Console.WriteLine($"Sound: {effect}");
            }
        }
    }



    public class Lazer : SuperPower
    {
        protected override void Activate()
        {
            object effect = new object();
            PlaySound(1, 1);
            PlayEffect(effect);
        }
    }
}
