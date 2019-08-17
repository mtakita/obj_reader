%option noyywrap nodefault yylineno case-insensitive

%x MTL

%{

#include "obj_reader_bison.tab.h"
#include "obj_reader.h"
#include <stdio.h>

struct bufstack{
	struct bufstack* prev;
	YY_BUFFER_STATE bs;
	int lineno;
	char* filename;
	FILE* f;
}*curbs = 0;

char* curfilename = NULL;

int popfile(){

	struct bufstack* bs = curbs;
	struct bufstack* prevbs;

	if(!bs) return 0;

	fclose( bs->f );

	free( bs->f );

	yy_delete_buffer( bs->bs );

	prevbs = bs->prev;
	free(bs);

	if(!prevbs) return 0;

	yy_switch_to_buffer( prevbs->bs );

	curbs = prevbs;

	yylineno = curbs->lineno;

	curfilename = curbs->filename;
	return 1;
}

int newfile( const char* fn ){

	FILE* f = fopen( fn, "r" );
	struct bufstack* bs = malloc( sizeof(struct bufstack) );
	
	if(!f){	perror(fn); return 0; }
	if(!bs){ perror("malloc"); exit(1); }

	if(curbs){ curbs->lineno = yylineno; }

	bs->prev = curbs;
	bs->bs = yy_create_buffer(f, YY_BUF_SIZE);
	bs->f = f;
	bs->filename = strdup(fn);

	yy_switch_to_buffer(bs->bs);

	curbs = bs;
	yylineno = 1;
	curfilename = bs->filename;

	return 1;
}

int startParse( const char* fileName ){
	newfile( fileName );
        yyparse();
}


%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)

%%

 /* keywords for *.mtl file */
"newmtl"		{ yylog("newmtl has found.\n"); return NEWMTL;}
"ns"			{ return NS;}
"ka"			{ return KA;}
"kd"			{ return KD;}
"ks"			{ return KS;}
"ke"			{ return KE;}
"ni"			{ return NI;}
"d"			{ return D;}
"illum"			{ return ILLUM;}
"map_kd"		{ return MAP_KD;}


 /* keywords */

"o"			{ yylog( "o has been found.\n"); return O;}
"v"			{ return V;}
"vn"			{ return VN;}
"vt"			{ return VT;}
"s"			{ return S;}
"f"			{ return F;}

"mtllib"[ \t]+		{ yylog("mtllib has been found.\n" ); BEGIN MTL; } 
<MTL>[^ \t\n]+		{
				{
					int c;
					while(( c = input() ) && c != '\n' );
				}
				yylineno++;
				if(!newfile(yytext)){
					yyterminate();
				}

				createMaterialLibrary( yytext );

				BEGIN INITIAL;
			}

"usemtl"		{ return USEMTL;}

"on"			{ return ON; }
"off"			{ return OFF; }

"/"			{ return SLASH; }


[-+]?[0-9]+"."[0-9]*{EXP}? | 
"."?[0-9]+{EXP}?		{ yylval.d = atof(yytext); return NUMBER; }

([a-zA-Z])+[a-zA-Z0-9]*		{ yylval.s = yytext; return NAME; }
([a-zA-Z0-9])+\.[mM][tT][lL] 	{ yylval.s = yytext; return FILENAME; }

 # /* ([a-zA-Z0-9:\\])+\.[jJ][pP][gG] { yylval.s = yytext; return FILEPATH; } */

([a-zA-Z]:[/\\])?(([a-zA-Z0-9_]+)?[/\\])*[a-zA-Z0-9_]+\.[a-zA-Z0-9_]{1,3} { yylval.s = yytext; return FILEPATH; }

"#".*
[ \t\n]  	/* ignore spaces */
^[ \t]*$	/* ignore blank lines */

 /* \n			{ return EOL; } */
 /* \n			{ } */
 /* <<EOF>>		{ yylog("flex has reached the EOF.\n"); if(!popfile()) yyterminate();  ?? return EOF;  } */
<<EOF>>			{ yylog("flex has reached the EOF.\n"); if(!popfile()) yyterminate(); /* return EOF; */ }

.			{ yyerror("Mystery character %c\n", *yytext); }
%%

