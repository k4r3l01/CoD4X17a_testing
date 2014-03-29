#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "../pinc.h"
#include "db_plugin.h"
MYSQL mysql;
MYSQL_RES *res;
MYSQL_ROW row;
void connect(){
	mysql_real_connect(&mysql,"", "", "","", 3306, NULL, 0);
}

void comm_Db(char *mysqldata,char *type,char **response)
{
    
	if (mysql_ping(&mysql) != 0) 
    {
		mysql_real_connect(&mysql,"", "", "","", 3306, NULL, 0);
		if (mysql_ping(&mysql) != 0) 
		{
			*response = "error";
			Plugin_PrintError("mysql error: %s\n",mysql_error(&mysql) );
			return;
		}
	}
    if (mysql_query(&mysql,mysqldata))
    {    
		*response = "error";
		Plugin_PrintError("mysql error: %s\n",mysql_error(&mysql) );
		return;
	}
	if(strcmp( type, "select") == 0)
    {
        if (!(res = mysql_store_result(&mysql)))
		{	
			*response = "error";
			Plugin_PrintError("mysql error: %s\n",mysql_error(&mysql) );
			return;
		}
		if (!mysql_eof(res))
		{
			*response = "error";
			Plugin_PrintError("mysql error: %s\n",mysql_error(&mysql) );
			return;
		}
		int count = mysql_num_rows(res);
        if( count <= 0 ){        
            *response = "null";
			return;
        }
        char * token = ";";
        char str[256];
		char data[256];
		while ((row = mysql_fetch_row(res)) != NULL)
		{
			strncat( str, row[1], strlen(row[1])+1 );
			strncat( str, token, strlen(token)+1 );
		}
		mysql_free_result(res);
		snprintf(data,sizeof(data),"%s",str);
		*response = str;
		return;
    }else if(strcmp( type, "select_id") == 0)
    {
        if (!(res = mysql_store_result(&mysql)))
		{
			*response = "error";
			Plugin_PrintError("mysql error: %s\n",mysql_error(&mysql) );
			return;
		}	
		if (!mysql_eof(res))
		{
			*response = "error";
			Plugin_PrintError("mysql error: %s\n",mysql_error(&mysql) );
			return;
		}	
		row = mysql_fetch_row(res);
        if(row == NULL)
        {
            mysql_free_result(res);
            *response = "null";
			return;
        }
        mysql_free_result(res);
        *response = row[0];
		return;
    } else {
        if ( mysql_affected_rows( &mysql ) < 0 )
		{
			*response = "null";
			return;
		}  
        *response = "ok";
		return;
    }
}