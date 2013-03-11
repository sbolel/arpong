/*=============================================================================
PROJECT CD  Text File Read/Write Program
Filename:   Bolel_ProjCD_Main.h

Northwestern University
Course:     EECS 351 - Introduction To Graphics
Assignment: Project CD

Title:      GLSL Programmable Shaders
Author:     www.lighthouse3d.com (Borrowed by S.H.B.)
                ---- Borrowed with permission as stated below ----

// START OF ORIGINAL PROGRAM:
=============================================================================*/

//////////////////////////////////////////////////////////////////////
// textfile.cpp
// simple reading and writing for text files
//
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textfileProg.h"

char *textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {

      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int textFileWrite(char *fn, char *s) {

	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {

			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}







