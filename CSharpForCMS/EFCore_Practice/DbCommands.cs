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

            //Console.WriteLine(daeeon.PlayerId); 
            //Console.WriteLine(db.Entry(daeeon).State); // State = Detached

            var items = new List<Item>() {
                // 여기서 Primary Key 인 ItemId 는 알아서 입력이 된다.
                new Item()
                {
                    TypeId = 101,
                    Owner = Bjarne
                },

                new Item()
                {
                    TypeId = 102,
                    Owner = Jeffrey
                },

                new Item()
                {
                    TypeId = 103,
                    Owner = daeeon
                }
            };

            var guild = new Guild()
            {
                GuildName = "C++",
                Members = new List<Player>() { Bjarne, Jeffrey, daeeon },
            };
            Console.WriteLine($"0번 상태 {db.Entry(guild).State}"); // Detached 아예 DB 와 연계되어서 존재를 모르는 객체이다.
            db.Guilds.Attach(guild);
            Console.WriteLine($"0번 상태 {db.Entry(guild).State}"); // Added, 해당 객체가 추적되고 있다고 강제로 설정해주는 것이다. 웬만하면 이렇게 쓰면 안된다.

            db.Items.AddRange(items);
            db.Guilds.Add(guild);
            Console.WriteLine($"1번 상태 {db.Entry(daeeon).State}"); // Unchanged 다. Player 테이블은 바뀌는 것이 없기 때문이다.
            //Console.WriteLine(db.Entry(daeeon).State); // State = Added, Item 만 추가했는데도 State 바뀌는 것에 유의

            db.SaveChanges();
            //Console.WriteLine(daeeon.PlayerId); 
            //Console.WriteLine(db.Entry(daeeon).State); // Stat = Unchanged

            // Add Test
            {
                Item item = new Item()
                {
                    TypeId = 500,
                    Owner = daeeon,
                };

                db.Items.Add(item);
                Console.WriteLine($"2번 상태{db.Entry(daeeon).State}");
            }

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
                    .ThenInclude(p => p.Item);

                foreach (var guildMember in guildMembers)
                {
                    foreach (var member in guildMember.Members)
                    {
                        Console.WriteLine(member.Item);
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

                // player 에 Item 을 Reference 로 Loading 해주세요. // 변수 1개라면 Reference 를 쓴다.
                foreach( Player player in guild.Members)
                {
                    db.Entry(player).Reference(p => p.Item).Load();
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

        public static void UpdateTest()
        {
            using (AppDbContext db = new AppDbContext())
            {
                var guild = db.Guilds.Single(g => g.GuildName == "C++");

                guild.GuildName = "C#";

                db.SaveChanges();
            }
        }

        public static void ShowItems()
        {
            using (AppDbContext db = new AppDbContext())
            {
                foreach (var item in db.Items.Include(i => i.Owner).ToList())
                {
                    Console.WriteLine(item);
                }
            }
        }
        public static void ShowGuild()
        {
            using (AppDbContext db = new AppDbContext())
            {
                foreach (var guild in db.Guilds.Include(g => g.Members).ToList())
                {
                    Console.WriteLine(guild);
                }
            }
        }

        public static void FkeyNullableTest()
        {
            ShowItems();

            Console.WriteLine("삭제할 Id를 입력해주세요.");
            Console.WriteLine(" >>");
            int id = int.Parse(Console.ReadLine());

            using (AppDbContext db = new AppDbContext())
            {
                var player = db.Players
                    .Include(p => p.Item)
                    .Single(p => p.PlayerId == id);

                db.Players.Remove(player);
                db.SaveChanges();
            }

            Console.WriteLine("Complete!");
            ShowItems();
        }

        // 1:1 관계 업데이트 테스트
        public static void RelationshipUpdateTest1()
        {
            ShowItems();

            Console.WriteLine("아이템을 바꿀 PlayerId 를 입력해주세요.");
            Console.WriteLine(" >>");
            int id = int.Parse(Console.ReadLine());

            using (AppDbContext db = new AppDbContext())
            {
                var player = db.Players
                    .Include(p => p.Item)
                    .Single(p => p.PlayerId == id);

                player.Item = new Item()
                {
                    TypeId = 777,
                };
                db.SaveChanges();
            }
        }

        // 1:N 관계 업데이트 테스트
        public static void RelationshipUpdateTest2()
        {
            ShowGuild();

            Console.WriteLine("업데이트할 길드 ID 를 입력해주세요.");
            Console.WriteLine(" >>");
            int id = int.Parse(Console.ReadLine());

            using (AppDbContext db = new AppDbContext())
            {
                Guild guild = db.Guilds
                    .Include(g => g.Members)
                    .Single(g => g.GuildId == id);

                // 이렇게 new 로 새로 할당하면, 기존 길드에 소속된 Player 들의 GuildId 가 다 Null 이 된다!
                // 그런데 이 앞단에서 Include 를 안해서 Members 데이터를 가져오지 않으면, 기존 길드원들이 유지되면서 새로운 멤버가 추가되게 된다.
                // 원치 않는 상황이 발생하므로 Include 꼭 하는 것을 명심하자.
                guild.Members = new List<Player>(); 
                guild.Members.Add(new Player()
                {
                    Name = "Toby"
                });

                db.SaveChanges();
            }

            ShowGuild();
        }

        public static void SoftDeleteTest()
        {
            ShowItems();

            Console.WriteLine("지울 아이템 PlayerId를 입력해주세요.");
            Console.WriteLine(" >>");
            int id = int.Parse(Console.ReadLine());

            using (AppDbContext db = new AppDbContext())
            {
                var player = db.Players
                    .Include(p => p.Item)
                    .Single(p => p.PlayerId == id);

                player.Item.IsDeleted = true;
                db.SaveChanges();
            }
        }

        public static void DirectSqlTest()
        {
            using (AppDbContext db = new AppDbContext())
            {
                // FromSql
                // Query SQL 용이다. (return 값이 있는 SQL, select)
                {
                    string name = "Daeeon";

                    // 아래처럼 하면 쿼리 내용으로 테이블 내용 해킹당할 수 있다.
                    // string nameForHacking = "'Anything' OR 1=1"; 
                    // 이러한 시도를 막기 위해서 FromSqlRaw, FromSqlInterpolated 와 같은 함수가 존재하는 것이다.
                    // 추가로 FromSql 방식으로는 Update 같은 것은 안된다는 것도 알아두자.
                    var list = db.Players
                        .FromSqlRaw("SELECT * from players where name = {0}", name).ToList();

                    // 아래처럼 String Interpolation 을 쓸 거면 FromSqlInterpolated 로 호출해야 한다.
                    //var list2 = db.Players
                    //    .FromSqlInterpolated($"SELECT * from players where name = {name}").ToList();

                    foreach (var p in list)
                    {
                        Console.WriteLine(p);
                    }
                }

                // ExecuteSqlCommand
                // Non-Query Sql(return 값이 없는 것으로 insert, update, delete 를 의미)용 함수
                {
                    db.Database.ExecuteSqlInterpolated($"update Players set Test = 332 where Name = 'daeeon'");
                }

                // Reload
                // Tracked Entity 가 있는데, 2번에 의해 DB 정보가 변경되면 DB와 메모리 상의 Entity 가 상태가 불일치하게 된다.
                // 이럴 때에 쓰는 것이 Reload 이다.
                {
                    Player p = db.Players.Single(p => p.Name == "Daeeon");

                    Console.WriteLine(p);
                    db.Database.ExecuteSqlInterpolated($"update Players set Test = 331 where Name = 'daeeon'");
                    Console.WriteLine(p);

                    db.Entry(p).Reload();
                    Console.WriteLine(p); // 이 시점에 DB 데이터와 p 데이터가 일치하게 된다.
                }
            }
        }
    }
}
