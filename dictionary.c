#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"
int i;


void printDict(Dictionary *dict){
	for(i = 0; i < DICTIONARY_INITIAL_SIZE; i++){
		if(dict->table[i] != NULL){
			printf("%d: %p: k='%s' v='%s'\n", i, 
					dict->table[i], 
					dict->table[i]->key,
					dict->table[i]->val);
		}
	}
}

Dictionary* newDictionary(){

	Dictionary *newObject = (Dictionary *)malloc(sizeof(struct Dictionary));
	DictionaryEntry** newTable = (DictionaryEntry **)malloc(sizeof(struct DictionaryEntry*)* DICTIONARY_INITIAL_SIZE);
	memset(newTable, 0, sizeof(struct DictionaryEntry*)*DICTIONARY_INITIAL_SIZE);
	newObject->table = newTable;
	newObject->size = 0;
	newObject->capacity = DICTIONARY_INITIAL_SIZE;
	return newObject;
}

DictionaryEntry* newDictionaryEntry(char *key, char *val){
	DictionaryEntry *newObject = (DictionaryEntry *)malloc(sizeof(struct DictionaryEntry));
	newObject->key = key;
	newObject->val = val;
	newObject->next = NULL;
	return newObject;
}

void addValueForKey(Dictionary* dict, char *key, char *val){
	unsigned int hashval = hashForKey(key);

	if( dict->table[hashval] == 0) {
		dict->table[hashval] = newDictionaryEntry(key, val);
	} else {
		DictionaryEntry *current = dict->table[hashval];
		int i = 0;
		while(current->next != NULL){
			printf("%d %d : k(%s) v(%s)\n", i, hashval, current->key, current->val);
			current = current->next;
		}
		current = newDictionaryEntry(key, val);
	}
}

char* getValueForKey(Dictionary * dict, char *key){
	int hashval = hashForKey(key);
	DictionaryEntry * current = dict->table[hashval];
	while( current != NULL ){
		if( strcmp( key, current->key ) == 0){
			return current->val;
		}
		current = current->next;
	}
	return NULL;
}

unsigned int hashForKey(char *key){

    unsigned int hashval;
    char *k = key;
    for(hashval = 0; *k != '\0'; k++)
    	hashval = *k + 31 * hashval;

    return hashval % DICTIONARY_INITIAL_SIZE;
}

void freeDictionary(Dictionary * dict){
	int i;
	for(i = 0; i < DICTIONARY_INITIAL_SIZE; i++){
		if(dict->table[i] != NULL){
			freeDictEntry(dict->table[i]);
		}
	}
	free(dict);
}

void freeDictEntry(DictionaryEntry * de){
	free(de->val);
	free(de->key);
	if(de->next != NULL)
		freeDictEntry(de->next);
	free(de);
}


/*
int main( int argc, char *argv[]){

	Dictionary *dict = newDictionary();

	char *k;

	char *v;

	printf("Adding value\n");
	asprintf(&k, "key");
	asprintf(&v, "value");
	addValueForKey(dict, k, v);

	printf("Taking value\n");	
	char *c = getValueForKey(dict, "key");
	if(c != NULL)
		printf("Value: %s\n", c);
	else
		printf("get value failed\n");

	return 0;
}
*/
