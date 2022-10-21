using Microsoft.EntityFrameworkCore;
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
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer(ConnectionString);
        }
    }
}
