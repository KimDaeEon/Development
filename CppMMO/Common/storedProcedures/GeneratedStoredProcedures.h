#pragma once
#include "Types.h"
#include <windows.h>
#include "DBBind.h"

namespace SP
{
	
    class InsertGold : public DBBind<2,0>
    {
    public:
    	InsertGold(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spInsertGold(?,?)}") { }
    	void In_Gold(int32& v) { BindParam(0, v); };
    	void In_Gold(int32&& v) { _gold = std::move(v); BindParam(0, _gold); };
    	void In_CreateTime(TIMESTAMP_STRUCT& v) { BindParam(1, v); };
    	void In_CreateTime(TIMESTAMP_STRUCT&& v) { _createTime = std::move(v); BindParam(1, _createTime); };

    private:
    	int32 _gold = {};
    	TIMESTAMP_STRUCT _createTime = {};
    };

    class GetGold : public DBBind<1,3>
    {
    public:
    	GetGold(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spGetGold(?)}") { }
    	void In_Gold(int32& v) { BindParam(0, v); };
    	void In_Gold(int32&& v) { _gold = std::move(v); BindParam(0, _gold); };
    	void Out_Id(OUT int32& v) { BindCol(0, v); };
    	void Out_Gold(OUT int32& v) { BindCol(1, v); };
    	void Out_CreateTime(OUT TIMESTAMP_STRUCT& v) { BindCol(2, v); };

    private:
    	int32 _gold = {};
    };


     
};