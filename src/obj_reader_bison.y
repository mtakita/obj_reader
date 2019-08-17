%{

#include <stdio.h>
#include <stdlib.h>
#include "obj_reader.h"

struct OBJECT_ATTRIBUTES* currentObj = NULL;
struct MATERIAL* currentMtl = NULL;
struct USE_MATERIAL* currentUseMaterial = NULL;

%}

%union{
	struct OBJECT_ATTRIBUTES *pObjectAttributes;
	struct ATTRIBUTE_F *pAttributeF;
	struct ATTRIBUTE_V *pAttributeV;
	struct ATTRIBUTE_VN *pAttributeVN;
	struct ATTRIBUTE_VT *pAttributeVT;
	struct FACENUM* pFaceNum;
	struct VERTNUM* pVertNum;
	struct VERT2NUM* pVert2Num;
	double d;
	char* s;

	struct MATERIAL *pMaterial;

	struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList;

	struct ATTRIBUTE_NS *pAttributeNs;
	struct ATTRIBUTE_KA *pAttributeKa;
	struct ATTRIBUTE_KD *pAttributeKd;
	struct ATTRIBUTE_KS *pAttributeKs;
	struct ATTRIBUTE_KE *pAttributeKe;
	struct ATTRIBUTE_NI *pAttributeNi;
	struct ATTRIBUTE_D  *pAttributeD;
	struct ATTRIBUTE_ILLUM *pAttributeIllum;
	struct ATTRIBUTE_MAPKD *pAttributeMapKd;
}

%token <d> NUMBER 
%token <s> NAME
%token <s> FILENAME FILEPATH

%token EOL

%token O V VT VN S F MTLLIB USEMTL ON OFF
%token NEWMTL NS KA KD KS KE NI D ILLUM MAP_KD 
%token SLASH

%type <pFaceNum> facenums facenum
%type <pVertNum> vertnums vertnum verts
%type <pVert2Num> verts2
%type <pObjectAttributes> object_attributes
%type <pObjectAttributes> object_name
%type <pAttributeF> attribute_f
%type <pAttributeV> attribute_v
%type <pAttributeVT> attribute_vt
%type <pAttributeVN> attribute_vn
%type <pAttributeUseMaterialList> usemtl_material 
%type <pMaterial> material 

%type <pMaterial> newmtl_name

%type <s> material_attributes 
%type <s> material_attribute 
%type <pAttributeNs> attribute_ns
%type <pAttributeKa> attribute_ka
%type <pAttributeKd> attribute_kd
%type <pAttributeKs> attribute_ks
%type <pAttributeKe> attribute_ke
%type <pAttributeNi> attribute_ni
%type <pAttributeD> attribute_d
%type <pAttributeIllum> attribute_illum
%type <pAttributeMapKd> attribute_mapKd


%start objfile


%%

objfile: obj_defs				{
						  yylog("obj_defs has been reducted.\n");
						  printf("Start dump>\n"); 
						  dump_all_objects();
						}

obj_defs: materials objects 			{
						  yylog("Parsing has been completed.\n");
 						}
;

objects: /* nothing */				{ yylog(" empty has been reducted to objects.\n"); }
| object objects 				{ yylog("object objects has been reducted.\n"); }
;

object: object_name				{ yylog("Object name has been reducted.\n"); }
| usemtl_material				{ yylog("usemtl_material has been reducted.\n"); }
| object_attributes				{ yylog("object: object_attributes rule has been reducted.\n"); }
;

object_attributes: attribute_v			{ yylog("Attribute_V has been attached.\n"); }
| attribute_f					{ yylog("Attribute_F has been attached.\n"); }
| attribute_vt					{ yylog("Attribute_VT has been attached.\n"); }
| attribute_vn					{ yylog("Attribute_VN has been attached.\n"); }
| sonoff					{ yylog("sonoff has been attached.\n"); }
;

object_name: O NAME  				{ yylog("O NAME has been found.\n"); $$ = createObjectAttributes( $2 ); }
;

usemtl_material: USEMTL NAME  			{ yylog("USEMTL NAME has been found.\n"); $$ = createUseMaterial($2); }
;

sonoff:     S onoff 				{ yylog("s on/off will be ignored.\n"); }
;

attribute_vt:     VT verts2 			{ $$ = createAttributeVT( $2 ); }
;

attribute_vn:     VN verts 			{ $$ = createAttributeVN( $2 ); }
;

attribute_v:     V vertnums 			{ $$ = createAttributeV( $2 ); }
;

attribute_f:     F facenums 			{ $$ = createAttributeF( $2 ); }
;

onoff: ON
| OFF						{ }
;

vertnums: vertnum				{ $$ = $1; }
;

facenums: facenum				{ $$ = $1; }
| facenum facenums 				{ $$ = $1; $1->pNext = $2; }
;

verts2: NUMBER NUMBER  				{ $$ = createVert2Num( $1, $2 ); }
;

verts: NUMBER NUMBER NUMBER 			{ $$ = createVertNum( $1, $2, $3); }
;

vertnum: NUMBER NUMBER NUMBER 			{ $$ = createVertNum( $1, $2, $3); }
;

facenum: NUMBER SLASH NUMBER SLASH NUMBER 	{ $$ = createFaceNum(enumFaceNumType1, $1, $3, $5); }
|      NUMBER SLASH NUMBER			{ $$ = createFaceNum(enumFaceNumType2, $1, $3,  0); }
|      NUMBER	SLASH SLASH NUMBER 		{ $$ = createFaceNum(enumFaceNumType3, $1,  0, $4); }
|      NUMBER					{ $$ = createFaceNum(enumFaceNumType4, $1,  0,  0); }
;

/* material file part */

materials: /* nothing */			{ yylog("materials has been reducted.\n"); }
| material materials 				{ yylog("material materials has been reducted.\n"); }
;

material: newmtl_name material_attributes	{ yylog("newmtl_name material_attributes has been reducted."); }
;

material_attributes: /* nothing */		{ yylog(" empty has been reducted to material_attributes.\n"); }
| material_attribute material_attributes	{ yylog("material_attribute material_attributes has been reducted."); }	
;

material_attribute: attribute_ns
| attribute_ka
| attribute_kd
| attribute_ks
| attribute_ke
| attribute_ni
| attribute_d
| attribute_illum
| attribute_mapKd				{ yylog("one attributs group has been reducted to material_attribute.\n"); $$ = $1; }
;

newmtl_name: NEWMTL NAME 			{ yylog("newmtl_name has been reducted.\n"); $$ = createNewmtl( $2 ); }
;

attribute_ns:     NS NUMBER 			{ yylog("attribute_ns has been reducted.\n"); $$ = createAttributeNs( $2 ); }
;
attribute_ka:     KA NUMBER NUMBER NUMBER 	{ yylog("attribute_ka has been reducted.\n");  $$ = createAttributeKa( $2, $3, $4 ); }
;
attribute_kd:     KD NUMBER NUMBER NUMBER 	{ yylog("attribute_kd has been reducted.\n"); $$ = createAttributeKd( $2, $3, $4 ); }
;
attribute_ks:     KS NUMBER NUMBER NUMBER 	{ yylog("attribute_ks has been reducted.\n"); $$ = createAttributeKs( $2, $3, $4 ); }
;
attribute_ke:     KE NUMBER NUMBER NUMBER 	{ yylog("attribute_ke has been reducted.\n"); $$ = createAttributeKe( $2, $3, $4 ); }
;
attribute_ni:     NI NUMBER 			{ yylog("attribute_ni has been reducted.\n"); $$ = createAttributeNi( $2 ); }
;
attribute_d:      D  NUMBER 			{ yylog("attribute_d has been reducted.\n"); $$ = createAttributeD( $2 ); }
;
attribute_illum:  ILLUM NUMBER 			{ yylog("attribute_illum has been reducted.\n"); $$ = createAttributeIllum( $2 ); }
;
attribute_mapKd:  MAP_KD FILEPATH 		{ yylog("attribute_mapKd has been reducted.\n" ); $$ = createAttributeMapKd( $2 ); }
;
