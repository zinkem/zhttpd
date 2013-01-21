
#define DICTIONARY_INITIAL_SIZE 41



typedef struct DictionaryEntry {
	char *val;
	char *key;
	struct DictionaryEntry* next;
} DictionaryEntry;


typedef struct Dictionary {
	int size;
	int capacity;
	struct DictionaryEntry** table;
} Dictionary;

Dictionary* newDictionary();
DictionaryEntry* newDictionaryEntry(char *key, char *val);
void addValueForKey(Dictionary *dict, char *key, char *val);
char* getValueForKey(Dictionary *dict, char *key);
unsigned int hashForKey(char *key);

void freeDictionary(Dictionary * dict);
void freeDictEntry(DictionaryEntry * de);