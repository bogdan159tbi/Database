#include "DataFunctionality.h"

#ifndef _STRING_
#define _STRING_

// NOTE: aici am functii de adaugare celula in coloana pt string si cautare celula cu anumita proprietate
//------ urmeaza sa pun si functia de delete celula de tip char

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


void insertStringVal(t_table *table,char **val,int coloane)
{
	// la fel ca la int iau prima oara cazul cu 2 coloane

	//parcurg pana la ultima linie a tabelului in cazul in care exista
	// adauga o noua linie cu valorile val pt fiecare coloane S si nr de coloane = coloane

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
	// adauga o noua line in tabelul cu numele tableName cu valorile values pt coloane (ex nume prenume) 
	// check if the table already exists (reminder)
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
	for(; cel != NULL ,coloana <= whichColumn; cel = cel->next,coloana++)
	 {if(!strcmp(rel,"<"))
		if (strcmp(value,cel->value) == -1)
			return 1;
	  if(!strcmp(rel,"<="))
		if (strcmp(value,cel->value) == 1 || strcmp(value,cel->value) == 0)
			return 1;
	  if(!strcmp(rel,">"))
		if (strcmp(value,cel->value) == 11)
			return 1;
	  if(!strcmp(rel,">="))
		if (strcmp(value,cel->value) == 1 || strcmp(value,cel->value) == 0)
			return 1;
	  if(!strcmp(rel,"!="))
		if (strcmp(value,cel->value) != 0)
			return 1;
	   if(!strcmp(rel,"=="))
		if (strcmp(value,cel->value) == 0)
			return 1;

	 }

	return 0;
}

//find  if there is any line which has a column with value = value | 
// and matches the relation found in rel

void findSTRING(t_db *db,char *tableName,char *columnName,char *value,char *rel,FILE *out)
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
				 {	fprintf(out,"TABLE: %s\n",tableName);
					printColumns(col,out);
				 	printStringLines(lin,out);
				 }
			}
	else
		fprintf(out,"Table \"%s\" does not contain column \"%s\".\n",tableName,columnName);
	}
}


// free line that respects the condition found in rel
/*
void deleteStringLine(t_db *db,char *tableName,char *columnName,char *val,char *rel)
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
				if (checkStringLine(lin,nr,val,rel))
				 	deleteString(lin);
			}
		 }	

}
*/

void deleteStringLine(t_table *table,char *colName,char *rel,char *val,FILE *o)
{
int nrColumn = 0;

//verifica existenta coloanei si indicele sau in lista de coloane
t_column *c = table->columns;
for(; c != NULL; c = c->next,nrColumn++)
	if (!strcmp(c->name,colName))
		break;
if( !c){
	fprintf(o,"Table \"%s\" does not contain column \"%s\".\n",table->name,colName);
	return;
}
//gaseste linie
t_stringLine *lin = table->lines ,*ultim = NULL;
for( ; lin != NULL; lin = lin->next)
	if(checkStringLine(lin,nrColumn,val,rel))
		break;
if(lin == NULL)
	return;//daca nu exista vreo linie cu valorea ceruta

//refa legaturile in functie de pozitia liniei in lista de linii(urm)
if(!ultim){
	t_stringLine *firstLine = table->lines;
	firstLine = firstLine->next;
	table->lines = firstLine;
}
else
	ultim->next = lin->next;
//delete celule de tip float
deleteStringCell(&lin->cells);
//elibereaza linie
free(lin);
}

#endif