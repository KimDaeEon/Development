using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace GameServer.Migrations
{
    /// <inheritdoc />
    public partial class Init : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropPrimaryKey(
                name: "PK_Character1",
                table: "Character1");

            migrationBuilder.RenameTable(
                name: "Character1",
                newName: "Character");

            migrationBuilder.RenameIndex(
                name: "IX_Character1_AccountDbId",
                table: "Character",
                newName: "IX_Character_AccountDbId");

            migrationBuilder.AddPrimaryKey(
                name: "PK_Character",
                table: "Character",
                column: "Id");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropPrimaryKey(
                name: "PK_Character",
                table: "Character");

            migrationBuilder.RenameTable(
                name: "Character",
                newName: "Character1");

            migrationBuilder.RenameIndex(
                name: "IX_Character_AccountDbId",
                table: "Character1",
                newName: "IX_Character1_AccountDbId");

            migrationBuilder.AddPrimaryKey(
                name: "PK_Character1",
                table: "Character1",
                column: "Id");
        }
    }
}
