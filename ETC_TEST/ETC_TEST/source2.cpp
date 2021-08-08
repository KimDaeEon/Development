//#include <stdio.h>
//#include <Windows.h>
#include <iostream>
#include <string>
#include<my_global.h>
#include <mysql.h>

#pragma comment(lib, "libmySQL.lib")

using namespace std;

const char* host = "localhost";
const char* user = "root";
const char* pw = "1234";
const char* db = "sys";

int main() {
    MYSQL* connection = NULL;
    MYSQL conn;
    MYSQL_RES* sql_result;
    MYSQL_ROW sql_row;

    if (mysql_init(&conn) == NULL)
    {
        printf("mysql_init() error!");
    }

    connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
    if (connection == NULL)    // 연결 에러 처리
    {
        printf("%d 에러 : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
        return 1;
    }
    else
    {
        printf("연결 성공\n");    // 연결 성공 메시지 출력

        if (mysql_select_db(&conn, db))    // 데이터베이스 선택
        {
            printf("%d 에러 : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
            return 1;
        }

        const char* query = "select *from player";
        int state = 0;

        state = mysql_query(connection, query);
        if (state == 0)
        {
            sql_result = mysql_store_result(connection);            // Result Set에 저장
            while ((sql_row = mysql_fetch_row(sql_result)) != NULL)    // Result Set에서 1개씩 배열에 가져옴
            {
                //cout << sql_row[0] << sql_row[1] << sql_row[2] <  endl;
                printf("%s %s\n", sql_row[0], sql_row[1]);    // 저장된 배열을 출력
            }
            mysql_free_result(sql_result);        // Result Set 해제한다
        }

        mysql_close(connection);        // db서버 종료
    }

    return 0;
}