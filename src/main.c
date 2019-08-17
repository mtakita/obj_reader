#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "obj_reader.h"

int main( int argc, char* argv[] ){
//int readObj( int argc, char* argv[] ){

        startParse( "log.obj" );
        return 0;
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

	startParse( "untitled.obj" );
	return 0;
}

*/

