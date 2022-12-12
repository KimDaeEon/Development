using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 서비스를 구현한 '구체' 클래스를 숨긴 채로 어디에서나 서비스에 접근할 수 있게 한다.
// 이를 통해서 Singleton 패턴과 다르게 제공하는 서비스 관련 내용을 쉽게 바꿀 수 있다.
namespace ServiceMediatorPattern
{
    public abstract class Audio
    {
        public abstract void PlaySound(int soundId);
        public abstract void StopSound(int soundId);
        public abstract void StopAllSounds(int soundId);

        // 기타 음악을 위한 데이터..
    };

    // Null 상태용 Audio 클랫스로 가정, 아직 Audio 구현이 안되었을 때 용도
    public class NullAudio : Audio
    {
        public override void PlaySound(int soundId)
        {
            // 구현
        }

        public override void StopSound(int soundId)
        {
            // 구현
        }

        public override void StopAllSounds(int soundId)
        {
            // 구현
        }
    }

    // 일반 Audio 용 '구체' 클래스로 가정
    public class ConsoleAudio : Audio
    {
        public static void Log(string logString)
        {
            // 로깅하는 코드가 있다고 가정
        }

        // 이렇게 NullAudio 와 다르게 Log 기능이 달린 Audio 서비스가 생겼다.
        // 원할 때에 이것을 바꿔서 로깅 기능을 On/Off 할 수 있는 것이다.
        public override void PlaySound(int soundId)
        {
            // 구현
            Log("log"); 
        }
        
        public override void StopSound(int soundId)
        {
            // 구현
            Log("log");
        }

        public override void StopAllSounds(int soundId)
        {
            // 구현
            Log("log");
        }
    }

    // 서비스 제공자가 Audio 를 주기에 이 Audio가 ConsoleAudio 인지 NullAudio 인지 서비스 받는 쪽에선 알지 못한다. 즉 '느슨한 결합'이 되는 것이다.
    public class ServiceMediator
    {
        public static Audio Audio
        {
            get
            {
                return _audioService;
            }

            // 아래 함수를 Provide 같은 것으로 바꿀 수도 있다.
            set
            {
                if(value == null)
                {
                    _audioService = _nullAudioService;
                }
                else
                {
                    _audioService = value;
                }

            }
        }


        private static Audio _audioService;
        private static NullAudio _nullAudioService = new NullAudio();
    }
}
