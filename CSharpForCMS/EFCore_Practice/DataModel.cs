using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

// Migration
// EntityFrameworkCore.Tools 설치 필요
// 도구 -> Nuget 패키지 관리자 -> 패키지 관리자 콘솔 창으로 들어간 후 아래 내용 실행

// 1. Code-First Migration
// 1) Add-Migration [Name] 를 command 에 입력, Migration 파일을 생성
//  (1) DbContext 를 찾아서 DB 모델 분석 (현재 최신 DB 구조)
//  (2) ModelSnapshot.cs 를 이용해서 가장 마지막 Migration 상태 분석 (이전 DB 구조)
//  (3) (1) 과 (2) 를 비교해서 관련 변경 사항을 추출 (Migrate.Designer.cs 와 Migrate.cs 와 ModelSnapShot.cs 를 생성)

// 2) 위에서 만든 Migration 파일을 적용, 아래의 3가지 방식이 존재
//  (1) SQL change script 생성
//  -- Script-Migration [From] [To] [Options] 를 command 에 입력.
//  기본은 최초[From] 현재[To] 로 되어 있고, 현재에서 과거로 Migration 할 때에 정말 유용하다.
//  DBA 와 함께 일할 때에 가장 권장되는 방법
//  (2) Database.Migrate 함수 호출 (코드 상에서 하는 것이라 잘 안쓴다.)
//  (3) Command Line 방식 
// -- Update-Database [options] <- 최신 DB 상태로 만들어준다.
// -- Update-Database [Name] 을 하면 [Name]에 해당하는 Migration 으로 변경해준다.
// 이와 더불어 Migration 이 적용되면 __EFMigrationsHistory 테이블 내용 또한 적용된 Migration 만 남도록 변경된다는 것도 알아두자.
// 물론 이렇게 Migration 한다고 해서 DataModel 쪽의 C#코드는 바뀌지 않으므로 git 같은 형상관리 툴로 DataModel 관련 코드를 되돌려야 한다는 것도 잊지 말자.

// 3) 가장 최근 Migration 제거
// Remove-Migration 명령어를 통해 가능하다. 이 때에 과거 migration 으로 되돌려져 있지 않으면 제거가 불가능하다. 


// 2. Database-First Migration
// 앞의 Code First Migration 이 코드를 바탕으로 DB를 바꾸는 것이었다면, 이 방법은 DB를 바탕으로 관련 코드를 생성한다고 보면 된다.
// EF-Core Power Tools 를 깔고, Reverse Engineer 메뉴를 선택하고 대상 DB 연결하면 해당 DB를 기반으로 C# 코드를 생성해준다. Database_First_Migration 폴더 참고


// 3. SQL-First Migration
// SQL script 정보를 통해서 migration 적용, DB 간에 차이를 나타내는 script 는 어떻게 해서든 추출할 수 있다고 한다. 그것을 바탕으로 한 작업

// 데이터 모델 State
// 1) Detached (No Tracking 추적되지 않는 상태, 메모리에 DB 데이터가 올라오지 않았음. SaveChanges를 해도 변하지 않는다.)
// 2) Unchanged (DB 에 데이터가 있는 상태이고, 메모리 데이터 수정 사항은 없었음, SaveChanges를 해도 변하지 않는다.)
// 3) Deleted (DB에는 아직 있지만, 삭제되어야 함. SaveChanges를 하면 DB 데이터 삭제된다.)
// 4) Modified (DB에는 있고, 변경은 완료되지 않음. SaveChanges를 하면 DB 데이터 변경된다.)
// 5) Added (DB에는 해당 데이터가 없고, 적용되지 않음. SaveChanges를 하면 DB 에 데이터 추가된다.)




// SaveChanges 호출하면 일어나는 것
// 1) 객체들의 상태가 Unchanged 로 바뀐다. (변경 사항이 DB에 적용됨, 변경 사항은 Column 별로 추적되어서 변경되는 사항만 반영)
// 2) Primary Key 는 SQL Identity 를 써서 만든다.
// 3) 데이터 추가 후 ID 받아와서 객체의 ID property 를 채워준다. Foreign Key 참조로 존재하는 데이터들도 연결해준다.
// 이미 존재하는 Foreign Key 연결 안된 객체는, 연결하려는 객체를 Tracking 상태에 둔 후에 참조를 할당한 후 SaveChanges를 하면 나중에라도 연결이 가능하다.
// 1:1 관계인 경우 기존 Foreign key 할당 레코드는 Foreign key 가 사라진다. 이런 것까지 알아서 다 해준다는 것이 핵심
namespace EFCore_Practice
{
    // 아래처럼 데이터를 모델링한 클래스를 Entity 클래스 라고 한다.
    // Dependent entity: This is the entity that contains the foreign key properties. Sometimes referred to as the 'child' of the relationship.
    // Principal entity: This is the entity that contains the primary/alternate key properties.Sometimes referred to as the 'parent' of the relationship.
    // 아래에서 Item 이 OnwerId 를 통해 foreign key 를 갖는 테이블이 되어서 Dependent Entity 이고, Player 는 Principal Entity 가 된다.
    public class Item
    {
        // 아래처럼 Soft Delete 를 위해서 플래그를 설정해두고, AppDbContext 에 필터를 추가하면 쿼리 검색 시에 아래 플래그가 설정된 레코드는 제외된다.
        public bool IsDeleted { get; set; } = false;
        public int ItemId { get; set; }
        public int TypeId { get; set; }
        public int ItemGrade { get; set; }
        public DateTime CreatedDate { get; private set; }

        // 다른 클래스를 참조하고 있다면 Foreign key 로 참조한다. (Navigational Property)
        // 여기서 OwnerId 가 Nullable 이 아니면 Player 가 삭제되면 해당 아이템도 같이 삭제된다.
        // OwnerId 가 Nullable 이면 Player 가 삭제될 때에 OnwerId 에는 Null 이 들어가고, Player 데이터만 삭제된다.
        public int OwnerId { get; set; }
        public Player Owner { get; set; }

        public override string ToString()
        {
            string result = $"ItemId : {ItemId}, TypeId: {TypeId}, CreateDate: {CreatedDate}, Owner: {Owner}";
            return result;
        }
    }

    //[Table("MyPlayer")] // 이렇게 해서 테이블 이름 명시적으로 설정 가능
    public class Player
    {
        public int PlayerId { get; set; } // 클래스이름+Id 로 해두면 EFCore 에서 자동으로 Primary key 로 설정해준다.
        public string Name { get; set; }
        public int Test { get; set; } = 111;
        public Item Item { get; set; }
        public Guild Guild { get; set; }
        public override string ToString()
        {
            string result = $"Player : {PlayerId}, Name: {Name}, Guild: {Guild}";

            return result;
        }
    }

    public class Guild
    {
        public int GuildId { get; set; }
        public string GuildName { get; set; }
        public ICollection<Player> Members { get; set; }

        public override string ToString()
        {
            string result = $"Guild: {GuildId}, GuildName: {GuildName}";
            return result;
        }
    }
}
