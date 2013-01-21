#include <stdio.h>
#include <string.h>

/*
String functions in this file are of two types, 'new' and 'mutate'
-     'new' will create a new string, using the first string parameter as a source
-  'mutate' will use the mutate the first string parameter to some new form

*/

char * newString(char * s);
char * newNextLine(char * s, int * index);
char * newIndexToChar(char *s, int index, char end);

char * mutateToLower(char * s);