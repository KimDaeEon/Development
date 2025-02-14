// 서버에 의해 통제 받는 객체
public class Monster : Actor
{
    public override void Init(ulong dataSheetId)
    {
        DataSheetId = dataSheetId;

        // TODO: 데이터 세팅
    }
}