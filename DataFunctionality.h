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

#include "structures.h"
#include <string.h>
#include<stdlib.h>
#include<stdio.h>

/* data definition:  ->t_db *initDB(char *name)
					 |
					 -->aloc memorie pt tabele,coloane, linii 
*/

//int query
void printIntCells(t_intCell *cells)
{
	for(;cells != NULL; cells = cells->next)
		printf("%d          ",cells->value);
	printf("\n");
}

void printIntLines(t_intLine *lines)
{
	for(; lines != NULL ; lines = lines->next)
		printIntCells(lines->cells);
}


void insertIntVal(t_table *table,int val[])
{
	t_intLine *line ;
	
	/* tre sa stiu cate coloane sunt dinainte */
	// iau cazul cu 2
	t_intCell *aux = malloc(sizeof(t_intCell));
	if(!aux)
	{
		printf("nu s a alocat\n");
		return;
	}
	aux->value = val[0];
	line = malloc(sizeof(t_intLine));
	if (!line)
	{	printf("didn t alocate line \n");
		free(aux);
		return;
	}

	line->cells = aux;
	aux = malloc(sizeof(t_intCell));
	aux->value = val[1];
	line->cells->next = aux;
	// verific daca exista alta linie inainte in tabel
	if(!table->lines)
	table->lines = line;
	else
	{
		t_intLine *p = table->lines;
		for( ; p->next != NULL ; p = p->next);

		p->next = line;
		line->next = NULL;
	}

}
void addInt(t_db *db,char *tableName,int val[])
{
	t_table *tables = db->tables;

	for(; tables != NULL; tables = tables->next)
		if (!strcmp(tables->name,tableName))
			break;
	insertIntVal(tables,val);
}

//string query
void printStringCells(t_stringCell *cells)
{
	for( ;cells != NULL ; cells = cells->next)
		printf("%s ",cells->value);
	printf("\n");
}

void printStringLines(t_stringLine *lines)
{
	for(; lines != NULL ; lines = lines->next)
		printStringCells(lines->cells);	
}


void insertStringVal(t_table *table,char **values)
{

}
void addString(t_db *db,char *tableName,char **values)
{
	t_table *tables = db->tables;

	for(; tables != NULL; tables = tables->next)
		if (!strcmp(tables->name,tableName))
			break;
	insertStringVal(tables,values);
}



t_db *initDB(char *name)
{
	t_db *db = malloc(sizeof(t_db));
	if(!db)
	{
		printf("failed alocating db\n");
		return NULL;
	}

	strncpy(db->name,name,strlen(name));

	return db;
}

t_table *initTable()
{
	t_table *tables = malloc(sizeof(t_table));
	if (!tables)
	{
		printf("failed aloc table\n");
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

void deleteCell(t_intCell *cells)
{
	if(cells)
	{
		deleteCell(cells->next);
		free(cells);
	}
	else
		return;
}
void deleteLines(t_intLine *lines)
{

	if(lines)
		{deleteLines(lines->next);
		 t_intCell *celule = lines->cells;
		 deleteCell(celule);
		}
	else
		return;
}

void deleteTable(t_table *table)
{
	if(table){
	deleteLines(table->lines);
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
t_column *insertColumn(t_table *table,char *colName)
{	if(!table->columns){
	table->columns = malloc(sizeof(t_column));
	if(!table->columns)
	{
		printf("didn t succeed in allocating column\n");
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
			printf("didn t succeed alocating column\n");
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

t_table *createTable(t_db *db,char *tableName/*tipul pe care l las int prima oara*/)
{	
	if(!db->tables)
	{db->tables = initTable();
	strncpy(db->tables->name,tableName,strlen(tableName));
	}
	else
	{
		t_table *tables = db->tables;
		for(; tables->next != NULL; tables = tables->next);

		t_table *aux = initTable();
		tables->next = aux;
		aux->next = NULL;
	}

	return db->tables;
}

/*Data Query */
// print whole Database



void printColumns(t_column *columns)
{
	for(; columns != NULL; columns = columns->next)
			printf("%s ",columns->name);
	printf("\n");
}



void printTable(t_db *db,char *tableName,char *type)
{
	t_table *tb = db->tables;
	for(; tb != NULL; tb = tb->next)
		if ( !strcmp(tb->name,tableName))
		{	printColumns(tb->columns);
			
			if( ! strcmp(type,"INT"))
				printIntLines(tb->lines);

			if( !strcmp(type,"STRING"))
				printStringLines(tb->lines);	

			break;
		}
}
void printDB(t_db *db,char *type)
{
	printf("DATABASE: %s\n",db->name);

	t_table *tables = db->tables;
	for( ; tables != NULL; tables = tables->next)
	{
		printColumns(tables->columns);
		if( ! strcmp(type,"INT"))
		printIntLines(tables->lines);

		if( !strcmp(type,"STRING"))
			printStringLines(tables->lines);
	}

}


// mai am search

/*Data manipulation*/

