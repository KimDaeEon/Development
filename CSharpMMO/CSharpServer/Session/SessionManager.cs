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

        // TODO: 아래도 데이터를 통해 처리해야한다.
        public int GetCrowdedLevel()
        {
            int count = 0;
            lock (_lock)
            {
                count = _sessions.Count;
            }

            return count / 100;
        }

        public ClientSession Generate()
        {
            lock (_lock)
            {
                int newSessionId = ++_lastSessionId;

                ClientSession newSession = new ClientSession();
                newSession.SessionId = newSessionId;
                _sessions.Add(newSessionId, newSession);

                Console.WriteLine($"Connected Sessions Count: {_sessions.Count}");

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
                _sessions.Remove(session.SessionId);
                Console.WriteLine($"Connected Sessions Count: {_sessions.Count}");
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
