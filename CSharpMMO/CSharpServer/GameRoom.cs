using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer
{
    class GameRoom
    {
        List<ClientSession> _sessions = new List<ClientSession>();
        object _lock = new object();

        public void Enter(ClientSession session)
        {
            lock (_lock)
            {
                _sessions.Add(session);
                session.Room = this;
            }
        }

        public void Leave(ClientSession session)
        {
            lock (_lock)
            {
                _sessions.Remove(session);
                Console.WriteLine($"_lock ={_lock.GetHashCode()} room = {this.GetHashCode()}, leave session id : {session.SessionId}, sessions count : {_sessions.Count}");
            }
        }

        public void Broadcast(ClientSession session, string chatMessage)
        {
            S_Chat packet = new S_Chat();
            packet.playerId = session.SessionId;
            packet.chatMessage = chatMessage;
            ArraySegment<byte> segment = packet.Write();

            lock (_lock)
            {
                Console.WriteLine($"_lock ={_lock.GetHashCode()}, room = {this.GetHashCode()}, Broadcast start, sessions count : {_sessions.Count}");

                foreach (ClientSession tempSession in _sessions)
                {
                    tempSession.Send(segment);
                    Console.WriteLine($"Sent message to {tempSession.SessionId}, sessions count : {_sessions.Count}");
                }

                Console.WriteLine($"room = {this.GetHashCode()}, Broadcast end, sessions count : {_sessions.Count}");
            }
        }
    }
}
