using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer
{
    class SessionManager
    {
        static SessionManager _sessionManager = new SessionManager();
        int _lastSessionId = 0; // sessionId 발급용, 발급시마다 1씩 증가
        Dictionary<int, ClientSession> _sessions = new Dictionary<int, ClientSession>();
        object _lock = new object();

        public static SessionManager Instance
        {
            get 
            { 
                return _sessionManager; 
            }
        }

        public ClientSession Generate()
        {
            lock (_lock)
            {
                int newSessionId = ++_lastSessionId;

                ClientSession newSession = new ClientSession();
                newSession.SessionId = newSessionId;
                _sessions.Add(newSessionId, newSession);

                Console.WriteLine($"Connected: {newSessionId}");

                return newSession;
            }
        }

        public ClientSession Get(int id)
        {
            lock (_lock)
            {
                ClientSession session = null;
                _sessions.TryGetValue(id, out session);
                return session;
            }
        }

        public void Remove(ClientSession session)
        {
            lock (_lock)
            {
                Console.WriteLine($"DisConnected: {session.SessionId}");
                _sessions.Remove(session.SessionId);
            }
        }
    }
}
