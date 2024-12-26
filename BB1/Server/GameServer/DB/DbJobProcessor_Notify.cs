using GameServer.DB.EFCore;
using GameServer.Data;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer.DB
{
    // 메모리 선 적용 후 DB 에 그냥 던져도 상관없는 작업들
    public partial class DbJobProcessor : JobQueue
    {
        public static void TestDB()
        {
            CharacterDb charDb = new CharacterDb();
            charDb.AccountDbId = new Random().Next(0, 100);

            using (GameDbContext db = new GameDbContext())
            {
                db.Add(charDb);
                db.SaveChangesEx();
            }
        }

        //public static void EquipItemNotify(Player player, Item item)
        //{
        //    if (player == null || item == null)
        //    {
        //        return;
        //    }

        //    ItemDb itemDb = new ItemDb()
        //    {
        //        ItemDbId = item.ItemDbId,
        //        Equipped = item.Equipped
        //    };

        //    Instance.Push(() =>
        //    {
        //        using (GameDbContext db = new GameDbContext())
        //        {
        //            db.Entry(itemDb).State = EntityState.Unchanged;
        //            db.Entry(itemDb).Property(nameof(ItemDb.Equipped)).IsModified = true;

        //            bool success = db.SaveChangesEx();

        //            if (success)
        //            {
        //            }
        //            else
        //            {
        //                // TODO: 실패했으면 Kick
        //            }
        //        }
        //    }
        //    );
        //}
    }
}
