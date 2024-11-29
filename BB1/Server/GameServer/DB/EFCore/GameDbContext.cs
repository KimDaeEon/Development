using GameServer.Data;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer.DB.EFCore
{
    public class GameDbContext : DbContext
    {
        public DbSet<CharacterDb> Characters { get; set; }
        public DbSet<ItemDb> Items { get; set; }

        static readonly ILoggerFactory _logger = LoggerFactory.Create(builder => { builder.AddConsole(); });

        public GameDbContext()
        {

        }

        protected override void OnConfiguring(DbContextOptionsBuilder options)
        {
            ConfigManager.Init();

            options
                .UseLoggerFactory(_logger)
                .UseSqlServer(ConfigManager.Config.connectionString);
        }

        protected override void OnModelCreating(ModelBuilder builder)
        {
            // AccountDbId에 인덱스 걸어준다
            builder.Entity<CharacterDb>()
                .HasIndex(t => t.AccountDbId);

            builder.Entity<CharacterDb>()
                .Property(nameof(CharacterDb.CreateDate))
                .HasDefaultValueSql("CURRENT_TIMESTAMP");

            builder.Entity<ItemDb>()
                .HasIndex(i => i.AccountDbId);

            builder.Entity<ItemDb>()
                .HasIndex(t => t.OwnerDbId);
        }
    }
}
