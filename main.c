#include <stdio.h>
#include "DataFunctionality.h"
#include <string.h>
#include <stdlib.h>
#include "STRING.h"
#include "INT.h"
#include "FLOAT.h"

#define DELIM " "
#define MAX_TESTS 79

void creareTabel(char **arg, t_db *db, int elem,FILE *out)
{
  t_table *t = createTable(db,arg[1],out);
  if(t){
	if( !strcmp(arg[2],"FLOAT"))
		t->type = FLOAT;
	else if (!strcmp(arg[2],"INT"))
		t->type = INT;
	else if (!strcmp(arg[2],"STRING"))
		t->type = STRING;
	else
		fprintf(out,"Unknown data type.\n");
	int i;
	for(i = 3; i < elem ;i++)
	 	insertColumn(t,arg[i],out);

		
	}
}
 
void adaugaValori(t_db *db,char **arg,int elem,FILE *out)
{
	t_table *t = db->tables;
	char *tableName = arg[1];

	for(; t != NULL ; t = t->next){
	  if(!strcmp(tableName,t->name))
		break;
	}
	if(!t){
		fprintf(out,"Table \"%s\" does not exist.\n",tableName);
		return;
	}


	if (t->type == INT){
	int *valori = malloc(sizeof(int) * (elem - 2));
	if(!valori)
	{
	 fprintf(out,"nu s a alocat vectorul int de valori\n");
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
	 fprintf(out,"nu s a alocat vectorul int de valori\n");
     return;
	}
	int i;
	for(i = 0; i < elem - 2;i++)
		valori[i] = atof(arg[i+2]);

	addFLOAT(db,tableName,valori,elem - 2,out);
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
	else
		fprintf(out,"Unknown data type.\n");

}
void findLine(t_db *db,char **arg,FILE *out)
{
	t_table *t = db->tables;
	char *tableName = arg[1];

	for(; t != NULL ; t = t->next)
	  if(!strcmp(tableName,t->name))
		break;
	if(t->type == STRING)
		// 2= nume coloana 4 = valoare 3 = relatie
		findSTRING(db,tableName,arg[2],arg[4],arg[3],out); 
	else if(t->type == INT)
		findINT(db, tableName, arg[2], atoi(arg[4]), arg[3],out);
	else if(t->type == FLOAT)
		findFLOAT(db, tableName, arg[2], atof(arg[4]), arg[3],out);
	else 
		fprintf(out,"Unknown data type.\n");

}

void stergeTable(t_db **db, char *tableName,FILE *out)
{
	t_table *t = (*db)->tables,*ultim = NULL;
	//tableName[strlen(tableName) - 1 ] = '\0';
	for( ; t != NULL ; ultim = t,t = t->next)
		if (!strcmp(t->name,tableName)){
			printf("%s\n",tableName);
			break;
		}
	if(t){
	   t_table *aux = t;
	   if(t == (*db)->tables)
	   	(*db)->tables = (*db)->tables->next;
	   else
	   	ultim->next = t->next;
	   
	   deleteTable(&aux);
	}
	else{
		tableName[ strlen(tableName) - 1 ] = '\0'; 
		fprintf(out,"Table \"%s\" not found in database.\n",tableName);
	}
	
}
void stergLin(t_db *db,char *tabName,char *colName,char *rel,char *val,FILE *o)
{
	t_table *table = db->tables;

	for( ;table != NULL; table = table->next)
		if (!strcmp(tabName,table->name))
			break;
	if(table)
	{
		if(table->type == INT)
			deleteIntLine(table,colName,rel,val,o);//sa convertesc vectorul de valori in int
		else if(table->type == FLOAT)
			deleteFloatLine(table,colName,rel,val,o);//sa convertesc valorile in float 
		else if(table->type == STRING)
			deleteStringLine(table,colName,rel,val,o);
	}
	else{
		tabName[strlen(tabName) - 1 ] = '\0';
	  fprintf(o,"Table \"%s\" not found in database.\n",tabName);
	}
}


int main()
{
	char request[MAX_CMD_LEN];
	t_db *db;
	char *tok;
	char *arg[30];
	int elem = 0, i;

	FILE *in = fopen("./in/test42.in","rt");
	FILE *out = fopen("./out/test42.out","wt");
	if(!in)
	{
		printf("couldn't open file\n");
		return 1;
	}
	if(!out)
	{
		printf("couldn't open file out\n");
		return 1;
	}
	
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
	if (elem == 1)
		arg[0][strlen(arg[0])-1] = '\0';
	if (!strcmp("INIT_DB",arg[0]))
		db = initDB(arg[1],out);
	if (!strcmp("CREATE",arg[0]))
		 creareTabel(arg, db, elem, out);
	if (!strcmp("ADD",arg[0]))
		adaugaValori(db,arg,elem,out);
	if (!strcmp("PRINT",arg[0])){
		printTable(db,arg[1],out);
		fprintf(out,"\n");
	}
	if (!strcmp("SEARCH",arg[0]))
		findLine(db,arg,out);
    if (!strcmp("DELETE",arg[0]))
		{
		if (elem == 2){
		  arg[1][strlen(arg[1]) - 1 ] = '\0';
		  stergeTable(&db,arg[1],out);
		}
		
		else if (elem ==  1)
		   deleteDB(db);
		else if ( elem > 2)
		  stergLin(db,arg[1],arg[2],arg[3],arg[4],out);
		}
	if (!strcmp("PRINT_DB",arg[0]))
		printDB(db,out);
	}
	fclose(in);
	fclose(out);

		
	// la sfarsit sa adaug eu Delete DB
	for(i = 0 ; i < elem ;i++)
		free(arg[i]);


	return 0;
}