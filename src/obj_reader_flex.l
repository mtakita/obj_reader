%option noyywrap nodefault yylineno case-insensitive

%{

#include "obj_reader_bison.tab.h"
#include "obj_reader.h"

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

	fclose( fp );
}


int main( int argc, char* argv[] ){

        startParse( "file.obj" );
        return 0;
}

%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)

%%

 /* keywords */

"o"			{ return O;}
"v"			{ return V;}
"vn"			{ return VN;}
"vt"			{ return VT;}
"s"			{ return S;}
"f"			{ return F;}

"mtllib"		{ printf("mtllib identified!\n"); return MTLLIB;}
"usemtl"		{ return USEMTL;}

"on"			{ return ON; }
"off"			{ return OFF; }

"/"			{ return SLASH; }


([a-zA-Z])+[a-zA-Z0-9]*	{ yylval.s = yytext; return NAME; }
([a-zA-Z0-9])+\.[mM][tT][lL] { yylval.s = yytext; return FILENAME; }

[-+]?[0-9]+"."[0-9]*{EXP}? | 
"."?[0-9]+{EXP}?	{ yylval.d = atof(yytext); return NUMBER; }

"#".*
[ \t]  /* ignore spaces */

\n			{ return EOL; }
<<EOF>>			{ return EOF; }

.			{ yyerror("Mystery character %c\n", *yytext); }
%%

