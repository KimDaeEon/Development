﻿// <auto-generated />
using System;
using EFCore_Practice;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Migrations;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;

namespace EFCore_Practice.Migrations
{
    [DbContext(typeof(AppDbContext))]
    [Migration("20221022230342_test_migration1")]
    partial class test_migration1
    {
        protected override void BuildTargetModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("Relational:MaxIdentifierLength", 128)
                .HasAnnotation("ProductVersion", "5.0.17")
                .HasAnnotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn);

            modelBuilder.Entity("EFCore_Practice.Guild", b =>
                {
                    b.Property<int>("GuildId")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int")
                        .HasAnnotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn);

                    b.Property<string>("GuildName")
                        .HasColumnType("nvarchar(max)");

                    b.HasKey("GuildId");

                    b.ToTable("Guilds");
                });

            modelBuilder.Entity("EFCore_Practice.Item", b =>
                {
                    b.Property<int>("ItemId")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int")
                        .HasAnnotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn);

                    b.Property<DateTime>("CreatedDate")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("datetime2")
                        .HasDefaultValueSql("GETUTCDATE()");

                    b.Property<bool>("IsDeleted")
                        .HasColumnType("bit");

                    b.Property<int>("OwnerId")
                        .HasColumnType("int");

                    b.Property<int>("TypeId")
                        .HasColumnType("int");

                    b.HasKey("ItemId");

                    b.HasIndex("OwnerId")
                        .IsUnique();

                    b.ToTable("Items");
                });

            modelBuilder.Entity("EFCore_Practice.Player", b =>
                {
                    b.Property<int>("PlayerId")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int")
                        .HasAnnotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn);

                    b.Property<int?>("GuildId")
                        .HasColumnType("int");

                    b.Property<string>("Name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<int>("Test")
                        .HasColumnType("int");

                    b.HasKey("PlayerId");

                    b.HasIndex("GuildId");

                    b.HasIndex("Name")
                        .IsUnique()
                        .HasDatabaseName("Index_Player_Name")
                        .HasFilter("[Name] IS NOT NULL");

                    b.ToTable("Players");
                });

            modelBuilder.Entity("EFCore_Practice.Item", b =>
                {
                    b.HasOne("EFCore_Practice.Player", "Owner")
                        .WithOne("Item")
                        .HasForeignKey("EFCore_Practice.Item", "OwnerId")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("Owner");
                });

            modelBuilder.Entity("EFCore_Practice.Player", b =>
                {
                    b.HasOne("EFCore_Practice.Guild", "Guild")
                        .WithMany("Members")
                        .HasForeignKey("GuildId");

                    b.Navigation("Guild");
                });

            modelBuilder.Entity("EFCore_Practice.Guild", b =>
                {
                    b.Navigation("Members");
                });

            modelBuilder.Entity("EFCore_Practice.Player", b =>
                {
                    b.Navigation("Item");
                });
#pragma warning restore 612, 618
        }
    }
}
