﻿BEGIN TRANSACTION;
GO

DECLARE @var0 sysname;
SELECT @var0 = [d].[name]
FROM [sys].[default_constraints] [d]
INNER JOIN [sys].[columns] [c] ON [d].[parent_column_id] = [c].[column_id] AND [d].[parent_object_id] = [c].[object_id]
WHERE ([d].[parent_object_id] = OBJECT_ID(N'[Items]') AND [c].[name] = N'ItemGrade');
IF @var0 IS NOT NULL EXEC(N'ALTER TABLE [Items] DROP CONSTRAINT [' + @var0 + '];');
ALTER TABLE [Items] DROP COLUMN [ItemGrade];
GO

DELETE FROM [__EFMigrationsHistory]
WHERE [MigrationId] = N'20221022231302_test_migration2';
GO

COMMIT;
GO

