using GameServer.DB.EFCore;
using ServerCore;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public static class Extensions
    {
        public static bool SaveChangesEx(this GameDbContext db)
        {
            try
            {
                db.SaveChanges();
                return true;
            }
            catch (Exception e)
            {
                Logger.LogDebug($"SaveChanges() exception occurred!!! {e}");
                return false;
            }
        }
    }
}
