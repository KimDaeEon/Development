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
    if (connection == NULL)    // ���� ���� ó��
    {
        printf("%d ���� : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
        return 1;
    }
    else
    {
        printf("���� ����\n");    // ���� ���� �޽��� ���

        if (mysql_select_db(&conn, db))    // �����ͺ��̽� ����
        {
            printf("%d ���� : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
            return 1;
        }

        const char* query = "select *from player";
        int state = 0;

        state = mysql_query(connection, query);
        if (state == 0)
        {
            sql_result = mysql_store_result(connection);            // Result Set�� ����
            while ((sql_row = mysql_fetch_row(sql_result)) != NULL)    // Result Set���� 1���� �迭�� ������
            {
                //cout << sql_row[0] << sql_row[1] << sql_row[2] <  endl;
                printf("%s %s\n", sql_row[0], sql_row[1]);    // ����� �迭�� ���
            }
            mysql_free_result(sql_result);        // Result Set �����Ѵ�
        }

        mysql_close(connection);        // db���� ����
    }

    return 0;
}