using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Text;

namespace EFCore_Practice
{
    // 이 클래스 자체가 Db 를 관리하는 클래스라고 보면 된다.
    // EF Core 작동 과정
    // 1) DbContext 생성
    // 2) DbSet<T> 를 찾는다.
    // 3) class 구조를 분석
    // 4) class 에서 참조하는 다른 class 분석
    // 5) OnModelCreating 함수 호출 (overriding 을 통해 추가 설정 가능)
    // 6) 데이터베이스 전체 모델링 구조를 내부 메모리에 들고 있는다.
    public class AppDbContext : DbContext
    {
        public DbSet<Item> Items { get; set; } // DbSet<T> 이 T 라는 객체와 대응되는 테이블이라고 보면 된다.
        public DbSet<Player> Players { get; set; } // 이 부분 없어도 Navigational Property 로 Player 테이블이 알아서 생성된다.
        public DbSet<Guild> Guilds { get; set; } 

        public const string ConnectionString = 
            "Data Source=DESKTOP-9IA022F;Initial Catalog=test;User ID=sa;Password=1234;Connect Timeout=30;Encrypt=False;TrustServerCertificate=False;ApplicationIntent=ReadWrite;MultiSubnetFailover=False"; // DB 에 어떻게 연결할지 설정을 저장하는 문자열

        // microsoft.Extensions.logging.console <- 요거 깔아야지 AddConsole() 이 가능하다.
        public static readonly ILoggerFactory MyloggerFactory = LoggerFactory.Create(builder =>
        {
            builder.AddConsole();
        });
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder
                .UseLoggerFactory(MyloggerFactory) // 이를 통해 EF Core 돌아가는 동안 생성되는 sql 을 볼 수 있다.
                .UseSqlServer(ConnectionString);
        }

        // Fluent API 방식으로 모델 관련 설정을 하게해주는 함수이다.
        // Convention 이나 Data Annotation 보다 자세하게 설정이 가능하지만, 그만큼 손이 많이 간다.
        // Convention => Data Annotation => Fluent API 순으로 모델 관련 설정이 적용되기에, 다음 단계에서 앞단에서 한 설정을 덮어쓴다는 것 또한 알아두자.
        protected override void OnModelCreating(ModelBuilder builder)
        {
            // 모델 차원에서 해당 Column 이 false 인 것만 가져온다.
            // 아래 필터를 무시하려면 나중에 질의 C# 코드를 작성할 때에 IgnoreQueryFilters() 를 붙이면 된다.
            builder.Entity<Item>().HasQueryFilter(i => i.IsDeleted == false);
            builder.Entity<Player>().HasIndex(p => p.Name).IsUnique().HasDatabaseName("Index_Player_Name");

            // 아래처럼 해야지 DB 단에서 Default 값이 세팅이 된다. Property 의 기본값만 입력해두면 DB 단의 기본값으로는 반영되지 않는다.
            builder.Entity<Item>()
                .Property("CreatedDate")
                // 아래 같이 HasDefaultValueSql 를 사용하는 방식을 SQL Fragment 라고 한다.
                .HasDefaultValueSql("GETUTCDATE()");
        }
    }
}
