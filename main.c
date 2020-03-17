#include<stdio.h>
#include "structures.h"
#include "DataFunctionality.h"
#include <string.h>


#define DELIM " "

void execCmd(char *request,t_db *db)
{
	char *tok;
	tok = strtok(request,DELIM);

		char *tableName;
		strcpy(tableName,"Students");
		db->tables = createTable(db,tableName);	
		insertColumn(db->tables,"First_Name");
		insertColumn(db->tables,"Last_Name");
}

int main()
{
	t_db *db = initDB("SCHOOL");

	char request[100];
	strcpy(request,"CREATE Students First_Name LastName");
	
	execCmd(request,db);
	int val[3];
	val[0] = 1;
	val[1] = 2;
	addInt(db,"Students",val);

	val[0] = 3;
	val[1] = 4;
	addInt(db,"Students",val);

	printDB(db,"INT");
	deleteDB(db);

	return 0;
}