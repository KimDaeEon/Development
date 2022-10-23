IF OBJECT_ID(N'[__EFMigrationsHistory]') IS NULL
BEGIN
    CREATE TABLE [__EFMigrationsHistory] (
        [MigrationId] nvarchar(150) NOT NULL,
        [ProductVersion] nvarchar(32) NOT NULL,
        CONSTRAINT [PK___EFMigrationsHistory] PRIMARY KEY ([MigrationId])
    );
END;
GO

BEGIN TRANSACTION;
GO

CREATE TABLE [Guilds] (
    [GuildId] int NOT NULL IDENTITY,
    [GuildName] nvarchar(max) NULL,
    CONSTRAINT [PK_Guilds] PRIMARY KEY ([GuildId])
);
GO

CREATE TABLE [Players] (
    [PlayerId] int NOT NULL IDENTITY,
    [Name] nvarchar(450) NULL,
    [Test] int NOT NULL,
    [GuildId] int NULL,
    CONSTRAINT [PK_Players] PRIMARY KEY ([PlayerId]),
    CONSTRAINT [FK_Players_Guilds_GuildId] FOREIGN KEY ([GuildId]) REFERENCES [Guilds] ([GuildId]) ON DELETE NO ACTION
);
GO

CREATE TABLE [Items] (
    [ItemId] int NOT NULL IDENTITY,
    [IsDeleted] bit NOT NULL,
    [TypeId] int NOT NULL,
    [CreatedDate] datetime2 NOT NULL DEFAULT (GETUTCDATE()),
    [OwnerId] int NOT NULL,
    CONSTRAINT [PK_Items] PRIMARY KEY ([ItemId]),
    CONSTRAINT [FK_Items_Players_OwnerId] FOREIGN KEY ([OwnerId]) REFERENCES [Players] ([PlayerId]) ON DELETE CASCADE
);
GO

CREATE UNIQUE INDEX [IX_Items_OwnerId] ON [Items] ([OwnerId]);
GO

CREATE UNIQUE INDEX [Index_Player_Name] ON [Players] ([Name]) WHERE [Name] IS NOT NULL;
GO

CREATE INDEX [IX_Players_GuildId] ON [Players] ([GuildId]);
GO

INSERT INTO [__EFMigrationsHistory] ([MigrationId], [ProductVersion])
VALUES (N'20221022230342_test_migration1', N'5.0.17');
GO

COMMIT;
GO

BEGIN TRANSACTION;
GO

ALTER TABLE [Items] ADD [ItemGrade] int NOT NULL DEFAULT 0;
GO

INSERT INTO [__EFMigrationsHistory] ([MigrationId], [ProductVersion])
VALUES (N'20221022231302_test_migration2', N'5.0.17');
GO

COMMIT;
GO

