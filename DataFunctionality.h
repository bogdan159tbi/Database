#include "structures.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Data definition    ->Init DB 
				      ->Delete DB -> Free memory
				      ->Create Table

   Data Query         ->Print DB
   					  ->Print table: show data in the table
   				   	  ->Search table: show lines that have value which provides with the required conditions

   Data Manipulation  ->Add table
   					  ->Delete table: free lines that consist of certain value
   					  ->Clear table: free all lines
 */

/* data definition:  ->t_db *initDB(char *name)
					 |
					 -->aloc memorie pt tabele,coloane, linii 
*/

//int query


//NOTE---> aici am functii de initializare creare tabela si afisare linii/coloane pt int si string,float nu inca
// + functie de eliberare memorie pt database ,pt linii si coloane de tip int  si string le am facut recursiv


#ifndef _DATAFUNCTIONALITY_
#define _DATAFUNCTIONALITY_


t_cellType checkType(char *type)
{
	if (!strcmp(type,"STRING"))
		return STRING;
	if (!strcmp(type,"INT"))
		return INT;
	if (!strcmp(type,"FLOAT"))
		return FLOAT;
	printf("unknown type\n");
	return 0;
}

t_db *initDB(char *name,FILE *out)
{
	t_db *db = malloc(sizeof(t_db));
	if(!db)
	{
		fprintf(out,"failed alocating db\n");
		return NULL;
	}

	strncpy(db->name,name,strlen(name));

	return db;
}

t_table *initTable(FILE *out)
{
	t_table *tables = malloc(sizeof(t_table));
	if (!tables)
	{
		fprintf(out,"failed aloc table\n");
		return NULL;
	}
	
	tables->next = NULL;
	return tables;
}


void deleteColumns(t_column **col)
{
	t_column *columns = *col;
	if(columns)
		{deleteColumns(&columns->next);
		free(columns);
		}
	if(columns == NULL)
		return;
}

void deleteIntCell(t_intCell **c)
{	t_intCell *cells = *c;
	if(cells)
	{
		deleteIntCell(&cells->next);
		free(cells);
	}
	else
		return;
}
void deleteInt(t_intLine **lin)
{
    t_intLine *lines = *lin;
	if(lines)
		{deleteInt(&lines->next);
		 t_intCell *celule = lines->cells;
		 deleteIntCell(&celule);
		 free(lines);
		}
	else
		return;
}

void deleteStringCell(t_stringCell **c)
{	
	t_stringCell *cells = *c;
	if(cells)
	{
		deleteStringCell(&cells->next);
		free(cells);
	}
	else
		return;
}
void deleteString(t_stringLine **lin)
{
	t_stringLine *lines = *lin;
	if(lines)
		{deleteString(&lines->next);
		 t_stringCell *celule = lines->cells;
		 deleteStringCell(&celule);
		 free(lines);
		}
	else
		return;

}

void deleteFloatCell(t_floatCell **cel)
{
	t_floatCell *celule = *cel;
	if(celule)
		{deleteFloatCell(&celule->next);
		 free(celule);
		
		}
	else
		return;
}

void deleteFloat(t_floatLine **lin)
{
	t_floatLine *lines = *lin;
	if(lines)
	{
		deleteFloat(&lines->next);
		t_floatCell *celule = lines->cells;
		deleteFloatCell(&celule);
		free(lines);
	}
	else
		return;
}

void deleteTable(t_table **t)
{
	t_table *table = *t;
	if(table){
		if(table->type == INT)
			{	t_intLine *line = table->lines;
				deleteInt(&line);
			}
		else if(table->type == STRING){
			t_stringLine *line = table->lines;
			deleteString(&line);
		}
		//mai am pt float
		else if(table->type == FLOAT){
			t_floatLine *line = table->lines;
			deleteFloat(&line);
		}
	 	
	 	deleteColumns(&(table->columns));
		}
}
void deleteDB(t_db *db)
{
	deleteTable(&db->tables);
	free(db);
	// opreste executia programului
}

/*insereaza coloane la sfarsit */
t_column *insertColumn(t_table *table,char *colName,FILE *out)
{	if(!table->columns){
	//colName[strlen(colName) - 1 ] = '\0';
	table->columns = malloc(sizeof(t_column));
	if(!table->columns)
	{
		fprintf(out,"didn t succeed in allocating column\n");
		/*free rest of db */
		return NULL;
	}
	strncpy(table->columns->name,colName,strlen(colName));
	table->columns->next = NULL;
				}
	else	
	{	
		t_column *aux = malloc(sizeof(t_column));
		if(!aux)
		{
			fprintf(out,"didn t succeed alocating column\n");
			return NULL;
		}
		strcpy(aux->name,colName);
		t_column *p = table->columns;
		for(;p->next != NULL; p = p->next);
		p->next = aux;
		aux->next = NULL;
	}
	if(table->columns)
	return table->columns;
	
	return NULL;
}

t_table *createTable(t_db *db,char *tableName,FILE *out/*tipul pe care l las int prima oara*/)
{	
	if(!db->tables)
	{db->tables = initTable(out);
	 if(!db->tables)
	 {
	 	fprintf(out,"didn t alocate table\n");
	 	free(db);
	 	return NULL;
	 }
	strncpy(db->tables->name,tableName,strlen(tableName));
		return db->tables;
	}
	else
	{	
		t_table *tables = db->tables;
		for(; tables != NULL ; tables = tables->next)
			if (!strcmp(tableName,tables->name))
			{
				fprintf(out,"%s already exists.\n",tableName);
				return NULL;
			}
		for(tables = db->tables; tables->next != NULL; tables = tables->next);

		t_table *aux = initTable(out);
		if(!aux)
		{	deleteTable(&db->tables);
			free(db);
			return NULL;
		}
		tables->next = aux;
		strncpy(tables->next->name,tableName,strlen(tableName));
		return aux;
	}

	//printf("inserted %s table successfully\n",tableName);

}

/*Data Query */
// print whole Database

int floatCellLen(float value)
{
	int len  = 0;
	int value2 = value;
	while(value2)
	{
		len++;
		value2 /= 10;
	}
	if(value2 == 0)
		return 1;
	return len;
}
int intCellLen(int value)
{
	int len  = 0;
	while(value)
	{
		len++;
		value /= 10;
	}
	return len;
	
}

void printStringCells(t_stringCell *cells,FILE *out)
{	
	int j;
	for( ;cells != NULL ; cells = cells->next)
		{fprintf(out,"%s",cells->value);
		 for(j = 0; j < MAX_COLUMN_NAME_LEN - strlen(cells->value);j++)
		 	 fprintf(out," ");
		}
	fprintf(out,"\n");
}

void printStringLines(t_stringLine *lines,FILE *out)
{
	for(; lines != NULL ; lines = lines->next)
		printStringCells(lines->cells,out);	
}
void printFloatCells(t_floatCell *cells,FILE *out)
{
	for( ;cells != NULL ; cells = cells->next){
		fprintf(out,"%f ",cells->value);
		for(int j = 0;j < MAX_COLUMN_NAME_LEN  - floatCellLen(cells->value) - 7 ; j++)
			fprintf(out," ");
	}
	fprintf(out,"\n");

}
void printFloatLines(t_floatLine *lines,FILE *out)
{
	for(; lines != NULL ; lines = lines->next)
		printFloatCells(lines->cells,out);
}

void printIntCells(t_intCell *cells,FILE *out)
{
	for(;cells != NULL; cells = cells->next){
		fprintf(out,"%d",cells->value);
		for(int j = 0;j < MAX_COLUMN_NAME_LEN  - intCellLen(cells->value) ; j++)
			fprintf(out," ");
	}
	fprintf(out,"\n");
}

void printIntLines(t_intLine *lines,FILE *out)
{
	for(; lines != NULL ; lines = lines->next)
		printIntCells(lines->cells,out);
}
void printColumns(t_column *columns,FILE *out)
{	int j,coloane = 0 , eliminat = 0;
	for(; columns != NULL; columns = columns->next,coloane++)
		{ 
		 fprintf(out,"%s",columns->name);
		 for (j = 0 ; j < MAX_COLUMN_NAME_LEN - strlen(columns->name)+1;j++)
	     fprintf(out," ");
		}
	fprintf(out,"\n");	
	int z;
	for(z = 0 ;z < coloane - 1  ; z++)	
	  {for(j = 0 ;j < MAX_COLUMN_NAME_LEN ; j++)
		 fprintf(out,"-");
	   
	   fprintf(out," ");
	  }
	  for(j = 0 ;j < MAX_COLUMN_NAME_LEN ; j++)
	  fprintf(out,"-");
    fprintf(out,"\n");
}



void printTable(t_db *db,char *tableName,FILE *out)
{
	t_table *tb = db->tables;
	tableName[strlen(tableName)-1] = '\0';
	for(; tb != NULL; tb = tb->next)
		if ( !strcmp(tb->name,tableName))
		{	fprintf(out,"TABLE: %s\n",tableName);
			printColumns(tb->columns,out);
			if( tb->type == INT)		
				printIntLines(tb->lines,out);
		
			else if( tb->type == STRING)
					printStringLines(tb->lines,out);	
			else if ( tb->type == FLOAT)
				printFloatLines(tb->lines,out);
			else
				fprintf(out,"Unknown data type.\n");
			break;
		}
		/*
		else
		{
			printf("%s",tb->name);
		}
		*/ // nu stiu sigur de ce am pus else aici(ps nu cred ca mai e nev)
	if(!tb)
		fprintf(out,"Table %s not found in database.\n",tableName);
}
void printDB(t_db *db,FILE *out)
{
	if( !db)
	{
		fprintf(out,"data base doesn't exist\n");
		return ;
	}
	fprintf(out,"DATABASE: %s\n",db->name);

	t_table *tables = db->tables;
	for( ; tables != NULL; tables = tables->next)
	{	
		fprintf(out,"TABLE: %s\n",tables->name);
		printColumns(tables->columns,out);
		if( tables->type == INT)
		printIntLines(tables->lines,out);

		if( tables->type == STRING)
			printStringLines(tables->lines,out);
		if( tables->type == FLOAT)
			printFloatLines(tables->lines,out);
	}

}


// mai am search

/*Data manipulation*/

// caut linie resp 
//verific daca indepl relatia ceruta
// eliberez pe rand memoria

// le am implementat deja pt fiecare tip in parte


#endif