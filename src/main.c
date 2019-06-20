#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "obj_reader.h"

void yyerror(char *s, ...){
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "%d: error: ", yylineno );
	vfprintf(stderr, s, ap );
	fprintf(stderr, "\n");
}

/*
int main( int argc, char* argv[] ){

	return yyparse();
}
*/

/*
int startParse( const char* fileName ){

	FILE* fp = fopen( fileName, "r" );
	if( fp == NULL ){
		return 1;
	}

	YY_BUFFER_STATE bp;
	bp = yy_create_buffer( fp, YY_BUF_SIZE );
	yy_switch_to_buffer( bp );

	yyparse();

	yy_flush_buffer( bp );
	yy_delete_buffer( bp );
}


int main( int argc, char* argv[] ){

	startParse( "file.obj" );
	return 0;
}

*/

