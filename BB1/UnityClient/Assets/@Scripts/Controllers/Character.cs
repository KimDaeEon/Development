// 사용자 (누군가 조종하는 객체)
public class Character : Actor
{
    public override void Init(ulong dataSheetId)
    {
        DataSheetId = dataSheetId;

        // TODO: 데이터 세팅
    }
}