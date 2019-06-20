%{

#include <stdio.h>
#include <stdlib.h>
#include "obj_reader.h"

struct OBJECT_ATTRIBUTES* currentObj = NULL;

%}

%union{
	struct OBJECT_ATTRIBUTES *pObjectAttributes;
	struct ATTRIBUTE_F *pAttributeF;
	struct ATTRIBUTE_V *pAttributeV;
	struct ATTRIBUTE_VN *pAttributeVN;
	struct FACENUM* pFaceNum;
	struct VERTNUM* pVertNum;
	double d;
	char* s;
}

%token <d> NUMBER 
%token <s> NAME
%token <s> FILENAME

%token EOL

%token O V VT VN S F MTLLIB USEMTL ON OFF
%token SLASH

%type <pFaceNum> facenums facenum
%type <pVertNum> vertnums vertnum verts
%type <pObjectAttributes> object_attributes
%type <s> object_name
%type <pAttributeF> attribute_f
%type <pAttributeV> attribute_v
%type <pAttributeVN> attribute_vn

%start obj_defs


%%


obj_defs: objects 				{
							printf("Parsing has been completed.\n");
							printf("Start dump>\n"); 
							dump_all_objects();
 						}
;

objects: /* nothing */
| object objects 				{ }
;

object: object_name				{ printf("Object name has been reducted.\n"); }
| mtllib_filename				{ printf("mtllib_filename has been reducted.\n"); }
| usemtl_material				{ printf("usemtl_material has been reducted.\n"); }
| object_attributes				{ printf("object: object_attributes rule has been reducted.\n"); }
| EOL						{ printf("skipped EOL.\n"); }
;

object_attributes: attribute_v			{ printf("Attribute_V has been attached.\n"); }
| attribute_f					{ printf("Attribute_F has been attached.\n"); }
| attribute_vn					{ printf("Attribute_VN has been attached.\n"); }
| sonoff					{ printf("sonoff has been attached.\n"); }
;

usemtl_material: USEMTL NAME EOL 		{ printf("Do implement code here for material!\n"); }
;

mtllib_filename: MTLLIB FILENAME EOL 		{ printf("Do implement code here for mtllib!\n"); }
;

object_name: O  NAME EOL 			{ $$ = createObjectAttributes( $2 ); }
;

sonoff:     S onoff EOL				{ printf("s on/off will be ignored.\n"); }
;

attribute_vn:     VN verts EOL			{ $$ = createAttributeVN( $2 ); }
;

attribute_v:     V vertnums EOL			{ $$ = createAttributeV( $2 ); }
;

attribute_f:     F facenums EOL			{ $$ = createAttributeF( $2 ); }
;

onoff: ON
| OFF						{ }

vertnums: vertnum				{ $$ = $1; }

facenums: facenum				{ $$ = $1; }
| facenum facenums 				{ $$ = $1; $1->pNext = $2; }
;

verts: NUMBER NUMBER NUMBER 			{ $$ = createVertNum( $1, $2, $3); }

vertnum: NUMBER NUMBER NUMBER 			{ $$ = createVertNum( $1, $2, $3); }

facenum: NUMBER SLASH NUMBER SLASH NUMBER 	{ $$ = createFaceNum(enumFaceNumType1, $1, $3, $5); }
|      NUMBER SLASH NUMBER			{ $$ = createFaceNum(enumFaceNumType2, $1, $3,  0); }
|      NUMBER	SLASH SLASH NUMBER 		{ $$ = createFaceNum(enumFaceNumType3, $1,  0, $4); }
|      NUMBER					{ $$ = createFaceNum(enumFaceNumType4, $1,  0,  0); }
;

