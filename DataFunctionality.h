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

t_intCell *alocIntCell(int value)
{
	t_intCell *aux = malloc(sizeof(t_intCell));
	if(!aux)
		return NULL;
	aux->value = value;
	return aux;
}

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


void insertIntVal(t_table *table,int val[],int coloane)
{
	t_intLine *line ;
	/* tre sa stiu cate coloane sunt dinainte */
	// iau cazul cu 2
	
	line = malloc(sizeof(t_intLine));
	if (!line)
	{	printf("didn t alocate line \n");
		return;
	}

	t_intCell *ultim = NULL;
	int i;
	for( i = 0 ;i < coloane; i++)
	{
		t_intCell *aux = malloc(sizeof(t_intCell));
		if(!aux)
		{
			printf("nu s a alocat\n");
			return;
		}
		aux->value = val[i];
		if(!line->cells)
		{
			line->cells = aux;
			ultim = aux;
		}
		else if(ultim)
		{
			ultim->next = aux;
			ultim = aux;
		}
	}
	ultim->next = NULL;
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
void addInt(t_db *db,char *tableName,int val[],int coloane)
{
	t_table *tables = db->tables;

	for(; tables != NULL; tables = tables->next)
		if (!strcmp(tables->name,tableName))
			break;
	insertIntVal(tables,val,coloane);
}

//void findINT()
//string query

t_stringCell *alocStringCell(char *val)
{
	t_stringCell *aux = malloc(sizeof(t_stringCell));
	if(!aux)
		return NULL;
	aux->value = malloc(30);
	if(!aux->value)
	{
		free(aux);
		return NULL;
	}
	strcpy(aux->value,val);
	return aux;
}
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


void insertStringVal(t_table *table,char **val,int coloane)
{
	// la fel ca la int iau prima oara cazul cu 2 coloane

	//parcurg pana la ultima linie a tabelului in cazul in care exista
	t_stringLine *line ;
	line = malloc(sizeof(t_stringLine));
	if(!line)
	{
		printf("nu s a alocat linie pt string\n");
		return;
	}
	t_stringCell *ultim = NULL;
	for(int i = 0; i < coloane ;i++){
		if(!line->cells){
			line->cells = alocStringCell(val[i]);
			ultim = line->cells;
		}
		else if(ultim)
		{
			ultim->next = alocStringCell(val[i]);
			ultim = ultim->next;
		}
	}
	ultim->next = NULL;

	
	if(!table->lines)
		{table->lines = line;
		 
		}
	else
	{
		t_stringLine *p = table->lines;
		for(; p->next != NULL; p = p->next);
		p->next = line;
		line->next = NULL;
	}
	
}
void addString(t_db *db,char *tableName,char **values,int coloane)
{
	t_table *tables = db->tables;

	for(; tables != NULL; tables = tables->next)
		if (!strcmp(tables->name,tableName))
			break;
	if(tables)
	insertStringVal(tables,values,coloane);
}

//am verificat doar pentru valori egale la cautare 

int checkStringLine(t_stringLine *line,int whichColumn,char *value,char *rel)
{
	t_stringCell *cel = line->cells;
	int coloana = 0;
	for(; cel != NULL ; cel = cel->next,coloana++)
	 {if(!strcmp(rel,"<"))
		if (strcmp(value,cel->value) == 1)
		 if(coloana == whichColumn )
			return 1;
	  if(!strcmp(rel,"<="))
		if (strcmp(value,cel->value) == 1 || strcmp(value,cel->value) == 0)
		 if(coloana == whichColumn )
			return 1;
	  if(!strcmp(rel,">"))
		if (strcmp(value,cel->value) == -1)
		 if(coloana == whichColumn )
			return 1;
	  if(!strcmp(rel,">="))
		if (strcmp(value,cel->value) == -1 || strcmp(value,cel->value) == 0)
		 if(coloana == whichColumn )
			return 1;
	  if(!strcmp(rel,"!="))
		if (strcmp(value,cel->value) != 0)
		 if(coloana == whichColumn )
			return 1;
	   if(!strcmp(rel,"=="))
		if (strcmp(value,cel->value) == 0)
		 if(coloana == whichColumn )
			return 1;

	 }

	return 0;
}
void findSTRING(t_db *db,char *tableName,char *columnName,char *value,char *rel)
{
	t_table *t = db->tables;

	for( ; t != NULL; t = t->next)
		if (!strcmp(tableName,t->name))
			break;	
	int nr = 0;

	if(t){
	t_column *col = t->columns;
	for(; col != NULL ; col = col->next,nr++)
		if (!strcmp(col->name,columnName))
			break;
	if(col){
			t_stringLine *lin = t->lines;
			for(; lin != NULL ;lin = lin->next)
				if (checkStringLine(lin,nr,value,rel))
				 {
				 	printStringLines(lin);
				 	break;
				 }
			}
		 }
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
	 if(!db->tables)
	 {
	 	printf("didn t alocate table\n");
	 	free(db);
	 	return NULL;
	 }
	strncpy(db->tables->name,tableName,strlen(tableName));
	}
	else
	{
		t_table *tables = db->tables;
		for(; tables->next != NULL; tables = tables->next);

		t_table *aux = initTable();
		if(!aux)
		{	deleteTable(db->tables);
			free(db);
			return NULL;
		}
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

