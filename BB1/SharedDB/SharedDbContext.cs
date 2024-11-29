using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Text;

namespace SharedDB
{
    public class SharedDbContext : DbContext
    {
        public DbSet<TokenDb> Tokens { get; set; }
        public DbSet<ServerDb> Servers { get; set; }

        // Game Server 에서 사용할 DbContext
        public SharedDbContext()
        {

        }

        // Account Server 에서 사용할 DbContext, AccountServer 는 StartUp 부분에서 여기에 정보가 알아서 세팅이 된다.
        public SharedDbContext(DbContextOptions<SharedDbContext> options) : base(options)
        {
        }


        // 아래는 게임 서버를 위한 부분
        public static string ConnectionString { get; set; } =
    "Data Source=DESKTOP-9IA022F;Initial Catalog=SharedDB;User ID=sa;Password=1234;Connect Timeout=30;Encrypt=False;TrustServerCertificate=False;ApplicationIntent=ReadWrite;MultiSubnetFailover=False";

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            // AccountServer 쪽에서 configuration 이 된 후에 아래 내용은 적용하지 않기 위한 것
            // 게임 서버에서는 OnCofniguring 을 통해 정보를 세팅하기에 이 함수가 필요.
            if (optionsBuilder.IsConfigured == false)
            {
                optionsBuilder
                .UseSqlServer(ConnectionString);
            }
        }

        protected override void OnModelCreating(ModelBuilder builder)
        {
            // Token 의 AccountDbId 에 Unique 인덱스 추가
            builder.Entity<TokenDb>()
                .HasIndex(t => t.AcountDbId)
                .IsUnique();

            // Server 의 이름에 Unique 인덱스 추가
            builder.Entity<ServerDb>()
                .HasIndex(s => s.Name)
                .IsUnique();
        }
    }
}
