#include <stdio.h>
#include <ctype.h>
#include "zstring.h"

char * newString(char * s){
	char * new_string;
	asprintf(&new_string, "%s", s);
	return new_string;
}

char * newNextLine(char * s, int *index){
	char buffer[1024];
	int start = *index;
	char c = s[start];
	int skipped = 0; //counts cr's skipped
	while(c != '\n' && c != '\0'){
		if(c != '\r'){
			buffer[(*index)-start-skipped] = c;
		} else {
			skipped++;
		}
		(*index)++;
		c = s[(*index)];
	}
	buffer[(*index)-start-skipped] = '\0';
	(*index)++;
	return newString(buffer);
}

char * newIndexToChar(char *s, int index, char end){
	char buffer[1024];
	int i = 0;
	int start = index;
	while( s[index+i] != end && s[index+i] != '\0'){
		buffer[i] = s[index+i];
		i++;
	}
	buffer[i] = '\0';
	return newString(buffer);
}



char * mutateToLower(char * s){
	int i;
	for(i = 0; i < strlen(s); i++){
		s[i] = tolower(s[i]);
	}

	return s;
}
