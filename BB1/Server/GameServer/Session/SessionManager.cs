using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    class SessionManager : Singleton<SessionManager>
    {
        static SessionManager _sessionManager = new SessionManager();
        int _sessionIdIssuer = 0; // sessionId 발급용, 발급시마다 1씩 증가
        Dictionary<int, ClientSession> _sessions = new Dictionary<int, ClientSession>();
        object _lock = new object();

        public ClientSession Generate()
        {
            lock (_lock)
            {
                int newSessionId = ++_sessionIdIssuer;

                ClientSession newSession = new ClientSession();
                newSession.SessionId = newSessionId;
                _sessions.Add(newSessionId, newSession);

                Console.WriteLine($"Connected Sessions Count: {_sessions.Count}");

                return newSession;
            }
        }

        public void Remove(ClientSession session)
        {
            lock (_lock)
            {
                _sessions.Remove(session.SessionId);
                Console.WriteLine($"Connected Sessions Count: {_sessions.Count}");
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

        public List<ClientSession> GetSessions()
        {
            List<ClientSession> sessions = new List<ClientSession>();

            lock (_lock)
            {
                sessions = _sessions.Values.ToList();
            }

            return sessions;
        }
    }
}
