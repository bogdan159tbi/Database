#include<stdio.h>
#include "DataFunctionality.h"
#include <string.h>
#include "STRING.h"
#include "INT.h"
#include "FLOAT.h"

#define DELIM " "


void creareTabel(char **arg, t_db *db, int elem)
{
  t_table *t = createTable(db,arg[1]);
  if(t){
	if( !strcmp(arg[2],"FLOAT"))
		t->type = FLOAT;
	else if (!strcmp(arg[2],"INT"))
		t->type = INT;
	else if (!strcmp(arg[2],"STRING"))
		t->type = STRING;
	int i;
	for(i = 3; i < elem ;i++)
	{
		insertColumn(t,arg[i]);
	}
	}
}
 
void adaugaValori(t_db *db,char **arg,int elem)
{
	t_table *t = db->tables;
	char *tableName = arg[1];

	for(; t != NULL ; t = t->next){
	  if(!strcmp(tableName,t->name))
		break;
	}
	if(!t){
		printf("Table \"%s\" does not exist \n",tableName);
		return;
	}


	if (t->type == INT){
	int *valori = malloc(sizeof(int) * (elem - 2));
	if(!valori)
	{
	 printf("nu s a alocat vectorul int de valori\n");
     return;
	}
	int i;
	for(i = 0; i < elem - 2;i++)
		valori[i] = atoi(arg[i+2]);
		addInt(db,tableName,valori,elem - 2);
	}
	else if(t->type == FLOAT)
	{
	float *valori = malloc(sizeof(float) * (elem - 2));
	if(!valori)
	{
	 printf("nu s a alocat vectorul int de valori\n");
     return;
	}
	int i;
	for(i = 0; i < elem - 2;i++)
		valori[i] = atof(arg[i+2]);

	addFLOAT(db,tableName,valori,elem - 2);
	}
	else if(t->type == STRING)
	{
		char **argString = malloc(sizeof(char*)*(elem - 2 ));
		if(!argString)
			return;
		int i;
		for(i = 2; i < elem ; i++){
			argString[i] = malloc(30);
			if(!argString[i])
			{
				int j;
				for(j = 0; j < i; j++)
					free(argString[i]);
			}
			argString[i-2] = arg[i];
		}
		addString(db,tableName,argString,elem-2);
	}

}
void findLine(t_db *db,char **arg)
{
	t_table *t = db->tables;
	char *tableName = arg[1];
	
	for(; t != NULL ; t = t->next)
	  if(!strcmp(tableName,t->name))
		break;
	if(t->type == STRING)
		findSTRING(db,tableName,arg[2],arg[4],arg[3]); // 2= nume coloana 4 = valoare 3 = relatie
	else if(t->type == INT)
		findINT(db, tableName, arg[2], atoi(arg[4]), arg[3]);
	else if(t->type == FLOAT)
		findFLOAT(db, tableName, arg[2], atof(arg[4]), arg[3]);

}

int main()
{

	FILE *in = fopen("./in/test1.in","rt");
	if(!in)
	{
		printf("couldn't open file\n");
		return 1;
	}
	char request[MAX_CMD_LEN];
	t_db *db;
	char *tok;
	char *arg[30];
	int elem = 0, i;
	while ( fgets(request,MAX_CMD_LEN,in) != NULL)
	{
		// la fgets se adauga \n la sf liniei automat
		tok = strtok(request," ");
		elem = 0;
		while (tok)
		{				
			arg[elem] = malloc(MAX_TABLE_NAME_LEN);
			 if(!arg[elem])
			 {
			 	
			 	for( i = 0;i< elem; i++)
			 		free(arg[i]);
			 	return 1;
			 }
			 strcpy(arg[elem++],tok);
			 tok = strtok(NULL," ");	
		}
	if (!strcmp("INIT_DB",arg[0]))
		db = initDB(arg[1]);
	if (!strcmp("CREATE",arg[0]))
		 creareTabel(arg,db,elem);
	if (!strcmp("ADD",arg[0]))
		adaugaValori(db,arg,elem);
	if (!strcmp("PRINT",arg[0]))
		printTable(db,arg[1]);
	if (!strcmp("SEARCH",arg[0]))
		findLine(db,arg);
		
	}
	
	// la sfarsit sa adaug eu Delete DB
	for(i = 0 ; i < elem ;i++)
		free(arg[i]);
	fclose(in);
	return 0;
}