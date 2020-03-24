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


void deleteColumns(t_column *columns)
{
	if(columns)
		{deleteColumns(columns->next);
		free(columns);
		}
	if(columns == NULL)
		return;
}

void deleteIntCell(t_intCell *cells)
{
	if(cells)
	{
		deleteIntCell(cells->next);
		free(cells);
	}
	else
		return;
}
void deleteInt(t_intLine *lines)
{

	if(lines)
		{deleteInt(lines->next);
		 t_intCell *celule = lines->cells;
		 deleteIntCell(celule);
		}
	else
		return;
}

void deleteStringCell(t_stringCell *cells)
{
	if(cells)
	{
		deleteStringCell(cells->next);
		free(cells);
	}
	else
		return;
}
void deleteString(t_stringLine *lines)
{
	if(lines)
		{deleteString(lines->next);
		 t_stringCell *celule = lines->cells;
		 deleteStringCell(celule);
		}
	else
		return;

}

void deleteFloatCell(t_floatCell *celule)
{
	if(celule)
		{deleteFloatCell(celule->next);
		 free(celule);
		}
	else
		return;
}

void deleteFloat(t_floatLine *lines)
{
	if(lines)
	{
		deleteFloat(lines->next);
		t_floatCell *celule = lines->cells;
		deleteFloatCell(celule);
	}
	else
		return;
}

void deleteTable(t_table *table)
{
	if(table){
		if(table->type == INT)
			deleteInt(table->lines);
		
		else if(table->type == STRING)
			deleteString(table->lines);
		//mai am pt float
		else if(table->type == FLOAT)
			deleteFloat(table->lines);
	 	
	 	deleteColumns(table->columns);
		}
}
void deleteDB(t_db *db)
{
	deleteTable(db->tables);
	free(db);
	// opreste executia programului
}

/*insereaza coloane la sfarsit */
t_column *insertColumn(t_table *table,char *colName,FILE *out)
{	if(!table->columns){
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
				fprintf(out,"%s already exists\n",tableName);
				return NULL;
			}
		for(tables = db->tables; tables->next != NULL; tables = tables->next);

		t_table *aux = initTable(out);
		if(!aux)
		{	deleteTable(db->tables);
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
	for( ;cells != NULL ; cells = cells->next)
		fprintf(out,"%f ",cells->value);
	fprintf(out,"\n");

}
void printFloatLines(t_floatLine *lines,FILE *out)
{
	for(; lines != NULL ; lines = lines->next)
		printFloatCells(lines->cells,out);
}

void printIntCells(t_intCell *cells,FILE *out)
{
	for(;cells != NULL; cells = cells->next)
		fprintf(out,"%d",cells->value);
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
	//tableName[strlen(tableName)-1] = '\0';
	for(; tb != NULL; tb = tb->next)
		if ( !strstr(tb->name,tableName))
		{	fprintf(out,"TABLE: %s",tableName);
			printColumns(tb->columns,out);
			if( tb->type == INT)		
				printIntLines(tb->lines,out);
		
			else if( tb->type == STRING)
					printStringLines(tb->lines,out);	
			else if ( tb->type == FLOAT)
				printFloatLines(tb->lines,out);
			else
				fprintf(out,"unknown data type\n");
			break;
		}
		/*
		else
		{
			printf("%s",tb->name);
		}
		*/ // nu stiu sigur de ce am pus else aici
	if(!tb)
		fprintf(out,"%s doesn't exist\n",tableName);
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


#endif