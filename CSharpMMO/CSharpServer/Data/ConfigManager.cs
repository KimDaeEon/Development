using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Data
{
    [Serializable]
    public class ServerConfig
    {
        public string dataPath;
    }
    public class ConfigManager
    {
        public static ServerConfig Config { get; private set; }
        public static void Init()
        {
            string text = File.ReadAllText("config.json"); // 보통 config 파일은 실행 파일과 같은 경로에 넣는다.
            // DeserializeObject 를 할 때에 변수명은 대소문자는 일치 안해도 된다. 하지만 이 외에는 일치해야한다.
            Config = Newtonsoft.Json.JsonConvert.DeserializeObject<ServerConfig>(text);
        }
    }
}
