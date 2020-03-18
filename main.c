#include<stdio.h>
#include "structures.h"
#include "DataFunctionality.h"
#include <string.h>

#define DELIM " "

void execCmd(char *request,t_db *db)
{
		char *tok;
		char *tableName ,*type;
		t_table *newTable;
		char *arg[30];
		int elem = 0;
		tok = strtok(request," ");
		while (tok)
		{				
			arg[elem] = malloc(MAX_TABLE_NAME_LEN);
			 if(!arg[elem])
			 {
			 	int i;
			 	for( i = 0;i< elem; i++)
			 		free(arg[i]);
			 	deleteDB(db);
			 	return;
			 }
			 strcpy(arg[elem++],tok);
			 tok = strtok(NULL," ");	
		}//end while
		tableName = malloc(sizeof(MAX_TABLE_NAME_LEN));
		if(!tableName)
		{
			deleteDB(db);
			return;
		}
		strcpy(tableName,arg[1]);
		
		if(!type)
		{
			deleteDB(db);
			free(tableName);
			return;
			// eliberare sir arg
		}
		strcpy(type,arg[2]);
		newTable = createTable(db,tableName);
		newTable->type = (t_cellType)type;
		if(!newTable)
		{
			printf("nu s a alocat\n");
			deleteDB(db);
			return;
		}
		newTable->type = (t_cellType)type;
		int index = 3;
		for(; index < elem ;index++){
			insertColumn(newTable,arg[index]);
		}

		//if(newTable->type == STRING)
			printDB(db,"STRING");
		//if(newTable->type == INT)
		//	printDB(db,INT);
		
		// pentru adaugare in tabel a valorilor pt fiecare coloana
		if (!strcmp(arg[0],"ADD"))
		{
			int argINT[100];
			char **argSTRING;	
			char *tableName = malloc(sizeof(MAX_TABLE_NAME_LEN));
			if(!tableName)
			{
				deleteDB(db);
				return;
			}
			strcpy(tableName,arg[1]);
			int index ;
			//find table with tableName
			t_table *searchTable = db->tables;
			for(;searchTable != NULL;searchTable = searchTable->next)
				if (!strcmp(searchTable->name,tableName))
					break;

			//if(searchTable->type == STRING)
			
			for(int i = 2; i < elem ;i++)
				argSTRING[i-2] = arg[i];
			     
			
			if(searchTable->type == INT)
				for(int i = 2; i < elem ;i++)
				
			for(index = 2; index < elem ;index++)
				/*if(searchTable->type == STRING)*/addString(db,tableName,argSTRING,elem-2);
					
				//else
				//	addInt(db,tableName,argINT);
			//if(newTable->type == STRING)
				printDB(db,"STRING");
			//if(newTable->type == INT)
			//	printDB(db,INT);
		
		}
		if (!strcmp(arg[0],"SEARCH"))
		{
			char *colName = malloc(MAX_COLUMN_NAME_LEN);
			if(!colName)
			{
				free(tableName);
				deleteDB(db);
				return;
			}
			strcpy(colName,arg[2]);
			// prelucare relatie dintre coloana si valoare data
			//value arg[4];
		}

	for(int i = 0;i< elem; i++)
		 free(arg[i]);

}

int main()
{
	t_db *db = initDB("SCHOOL");

	char request[MAX_CMD_LEN];
	int size = MAX_CMD_LEN;

	char **relatii;
	int i;
	relatii = malloc(sizeof(char*)*6);
	if(!relatii)
	{
		deleteDB(db);
		return 1;
	}
	for(i = 0; i < 6; i++)
	{
		relatii[i] = malloc(3);
		if(!relatii[i])
		{
			int j;
			for(j = 0 ;j < i;j++)
				free(relatii[j]);
			free(relatii);
			return 1;
		}
	}
	strcpy(relatii[0],"<");
	strcpy(relatii[1],"<=");
	strcpy(relatii[2],">");
	strcpy(relatii[3],">=");
	strcpy(relatii[4],"!=");
	strcpy(relatii[5],"==");
	/*
	fgets(request,sizeof(request),stdin);
	execCmd(request,db);
	fgets(request,sizeof(request),stdin);
	execCmd(request,db);
	*/
	t_table *t = createTable(db,"Stud");
	insertColumn(t,"nume");
	char **val;
	val = malloc(2*sizeof(char*));
	val[0] = malloc(30);
	strcpy(val[0],"pla");
	addString(db,"Stud",val,1);
	printDB(db,"STRING");
	deleteDB(db);

	free(val[0]);
	free(val);
	free(relatii);
	return 0;
}