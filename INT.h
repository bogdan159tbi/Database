#include "DataFunctionality.h"

#ifndef _INT_
#define _INT_


//aici sunt functii de alocare celula , inserare celula cu anumita valoare si urmeaza delete la o anumita celula
// ---- SEARCH ,DELETE 

t_intCell *alocIntCell(int value)
{
	t_intCell *aux = malloc(sizeof(t_intCell));
	if(!aux)
		return NULL;
	aux->value = value;
	return aux;
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
	if(tables == NULL)
	{
		printf("couldn't find table\n");
		return;
	}
	insertIntVal(tables,val,coloane);
}


int checkIntLine(t_intLine *line,int whichColumn,int val,char *rel)
{

	t_intCell *cell = line->cells;
	for(int column = 0; cell != NULL,column <= whichColumn; cell = cell->next,column++)
	{
	  if(!strcmp(rel,"<"))
	  	if(val < cell->value)
	  		return 1;
	  if(!strcmp(rel,"<="))
	  	if(val < cell->value || val == cell->value)
	  		return 1;
   	  if(!strcmp(rel,">"))
   	  	if(val > cell->value)
   	  		return 1;
   	  if(!strcmp(rel,">="))
   	  	if(val >= cell->value)
   	  		return 1;
   	  if(!strcmp(rel,"!="))
   	  	if(val != cell->value)
   	  		return 1;
   	  if(!strcmp(rel,"=="))
   	  	if(val == cell->value)
   	  		return 1;
	}

	return 0;
}

void findINT(t_db *db,char *tableName,char *columnName,int columnValue,char *rel)
{
	int nrColumn = 0;
	t_table *t;
	for (t = db->tables ; t != NULL ; t = t->next)
		if (!strcmp(tableName,t->name))
			break;
	if(t)
	{
		t_column *c;
		for(c = t->columns ; c != NULL; c = c->next,nrColumn++)
			if (!strcmp(c->name,columnName))
				break;
		if(c)
		{
			t_intLine *l = t->lines;
			for(; l != NULL; l = l->next)
				if(checkIntLine(l,nrColumn,columnValue,rel))
				{
					printf("TABLE: %s\n",tableName);
					printColumns(c);
					printIntLines(l);
					break;
				}
		}
		else
		printf("Table \"%s\" does not contain column \"%s\" \n",tableName,columnName);

	}
	
}

void deleteIntLine()
{
	
}

#endif