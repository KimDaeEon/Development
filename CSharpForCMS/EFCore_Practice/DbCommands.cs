using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Storage;

namespace EFCore_Practice
{
    public class DbCommands
    {
        public static void InitializeDB(bool resetDb)
        {
            using (AppDbContext db = new AppDbContext())
            {
                if ((db.GetService<IDatabaseCreator>() as RelationalDatabaseCreator).Exists() && !resetDb)
                {
                    return;
                }

                db.Database.EnsureDeleted();
                db.Database.EnsureCreated();

                CreateTestData(db);
                Console.WriteLine("DB Initialized");
            }
        }

        public static void CreateTestData(AppDbContext db)
        {

            var Bjarne = new Player()
            {
                Name = "Bjarne"
            };

            var Jeffrey = new Player()
            {
                Name = "Jeffrey"
            };


            var daeeon = new Player()
            {
                Name = "Daeeon"
            };


            var items = new List<Item>() {
                // 여기서 Primary Key 인 ItemId 는 알아서 입력이 된다.
                new Item()
                {
                    TypeId = 101,
                    CreatedDate = DateTime.Now,
                    Owner = Bjarne
                },

                new Item()
                {
                    TypeId = 102,
                    CreatedDate = DateTime.Now,
                    Owner = Jeffrey
                },

                new Item()
                {
                    TypeId = 103,
                    CreatedDate = DateTime.Now,
                    Owner = daeeon
                }
            };

            var guild = new Guild()
            {
                GuildName = "C++",
                Members = new List<Player>() { Bjarne, Jeffrey, daeeon },
            };

            db.Items.AddRange(items);
            db.Guilds.Add(guild);
            db.SaveChanges();
        }

        public static void ReadAll()
        {
            using (var db = new AppDbContext())
            {
                // AsNoTracking : Tracking Snapshot 을 남기지 않아서 Read 시에 더 빠른 성능을 낼 수 있다.
                // 아래처럼만 하면 Foreign Key 의 요소인 Owner 에 대한 정보가 없다. 당연한 얘기지만 저 정보를 가져오려면 Join 을 해야하기 때문이다.
                // OnwerId 같은 경우는 Item 테이블 내에서 저장할 것이므로 아래 결과에서 나온다는 것도 알아두자.
                foreach (Item item in db.Items.AsNoTracking())
                {
                    Console.WriteLine(item);
                }

                // 아래처럼 하면 Join Query 가 작동이 되어 Owner(Player) 정보도 item 에 담아서가져온다.
                foreach (Item item in db.Items.AsNoTracking().Include(item => item.Owner))
                {
                    Console.WriteLine(item);
                }
            }
        }

        public static void ShowItems()
        {
            Console.WriteLine("플레이어 이름을 입력하세요.");
            Console.WriteLine(">> ");

            string name = Console.ReadLine();

            using (var db = new AppDbContext())
            {
                foreach (Player player in db.Players.AsNoTracking().Include(p => p.Items).Where(p => p.Name == name))
                {
                    foreach (Item item in player.Items)
                    {
                        Console.WriteLine(item);
                    }
                }
            }
        }

        // 장점: DB 접근 한 번으로 다 로딩
        // 단점: 컬럼이 다 필요한지 모르겠는데 다 로딩
        public static void EagerLoading()
        {
            Console.WriteLine("길드 이름을 입력하세요.");
            Console.WriteLine(">> ");
            string name = Console.ReadLine();

            using (var db = new AppDbContext())
            {
                var guildMembers = db.Guilds.AsNoTracking()
                    .Where(g => g.GuildName == name)
                    .Include(g => g.Members)
                    .ThenInclude(p => p.Items);

                foreach (var guildMember in guildMembers)
                {
                    foreach (var member in guildMember.Members)
                    {
                        foreach (var item in member.Items)
                        {
                            Console.WriteLine(item);
                        }
                    }
                }
            }
        }

        // 장점: 필요한 시점에 필요한 데이터만 로딩한다.
        // 단점: DB 접근을 여러번 한다. 
        public static void ExplicitLoading()
        {
            Console.WriteLine("길드 이름을 입력하세요.");
            Console.WriteLine(">> ");
            string name = Console.ReadLine();

            using (var db = new AppDbContext())
            {
                Guild guild = db.Guilds.Where(g => g.GuildName == name).First();

                // Guild 에 있는 Members 라는 Collection 으로 Loading 해주세요.
                db.Entry(guild).Collection(g => g.Members).Load();

                // player 에 Item 을 Collection 으로 Loading 해주세요. // 변수 1개라면 Reference 를 쓴다.
                foreach( Player player in guild.Members)
                {
                    db.Entry(player).Collection(p => p.Items).Load();
                }
            }
        }

        // 장점: 필요한 정보만 로딩 가능
        // 단점: Select 안에 필요한 정보에 대해서 타이핑 해줘야 한다. 
        public static void SeletcLoading()
        {
            Console.WriteLine("길드 이름을 입력하세요.");
            Console.WriteLine(">> ");
            string name = Console.ReadLine();

            using (var db = new AppDbContext())
            {
                var infos = db.Guilds
                    .Where(g => g.GuildName == name)
                    .Select(g => new
                    {
                        Name = g.GuildName,
                        MemberCount = g.Members.Count
                    });

                foreach(var info in infos)
                {
                    Console.WriteLine($"{info.Name}, {info.MemberCount}");
                }
            }
        }
    }
}
