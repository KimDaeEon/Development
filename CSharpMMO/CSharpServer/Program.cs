using System;
using System.Net;
using System.Threading;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using static Google.Protobuf.Protocol.Person.Types;

namespace CSharpServer
{

    class Program
    {
        static Listener _listener = new Listener();
        static void FlushRoom()
        {
            JobTimer.Instance.Push(FlushRoom, 250);
        }

        static void Main(string[] args)
        {
            Person person = new Person()
            {
                Name = "Daeeon",
                Id = 123,
                Email = "eo956578rr@gmail.com",
                Phones = { new PhoneNumber { Number = "555-4321", Type = PhoneType.Home } }
            };

            int size = person.CalculateSize();
            byte[] sendBuffer = person.ToByteArray();

            Person person2 = new Person();
            person2.MergeFrom(sendBuffer);



            // DNS 활용
            string host = Dns.GetHostName();
            IPHostEntry ipHost = Dns.GetHostEntry(host);
            IPAddress ipAddr = ipHost.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888); // 포트는 8888번으로 설정

            try
            {
                _listener.Init(endPoint, () => { return SessionManager.Instance.Generate(); });
                Console.WriteLine("Listening...");

                //FlushRoom(); // 아래처럼 하는 것이 더 직관적인 것 같다.
                JobTimer.Instance.Push(FlushRoom, 250);

                while (true)
                {
                    JobTimer.Instance.Flush();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }
    }
}
