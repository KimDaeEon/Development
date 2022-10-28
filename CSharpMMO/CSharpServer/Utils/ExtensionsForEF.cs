using CSharpServer.DB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Utils
{
    public static class ExtensionsForEF
    {
        public static bool SaveChangesEx(this AppDbContext db)
        {
            try
            {
                db.SaveChanges();
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine($"SaveChanges() exception occurred!!! {e}");
                return false;
            }
        }
    }
}
