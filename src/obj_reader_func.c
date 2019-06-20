#include <stdio.h>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "obj_reader.h"

extern struct OBJECT_ATTRIBUTES* currentObj;

static unsigned symhash( const char* sym){

	unsigned int hash = 0;
	unsigned c;

	while( c = *sym++ ) hash = hash*9^c;

	return hash;
}


struct OBJECT_ATTRIBUTES* lookup( const char* sym){

	struct OBJECT_ATTRIBUTES* sp = &symtab[symhash(sym)%NHASH];

	int scount = NHASH;

	while(--scount >= 0 ){
		if(sp->pObjectName && !strcmp(sp->pObjectName, sym)) {
			return sp;
		}

		if(!sp->pObjectName){
			sp->pObjectName = strdup(sym);
			return sp;
		}

		if(++sp >= symtab+NHASH) sp = symtab;
	}

	yyerror("symbol table overflow\n");
	abort();
}


struct VERTNUM* createVertNum( double x, double y, double z ){
	struct VERTNUM* pVertNum = malloc( sizeof(struct VERTNUM) );
	(*pVertNum).x = x;
	(*pVertNum).y = y;
	(*pVertNum).z = z;

	printf( "createVertNum( %#6.6g, %#6.6g, %#6.6g );\n", x, y, z );

	return pVertNum;
}

struct FACENUM* createFaceNum( enum FaceNumType type, double vertnum, double texnum, double normnum ){
	struct FACENUM* pFaceNum = malloc( sizeof(struct FACENUM) );
	(*pFaceNum).type = type;
	(*pFaceNum).vertnum = vertnum;
	(*pFaceNum).texnum = texnum;
	(*pFaceNum).normnum = normnum;
	(*pFaceNum).pNext = NULL;

	printf( "createFaceNum( %d, %#6.6g, %#6.6g, %#6.6g );\n", type, vertnum, texnum, normnum );

	return pFaceNum;
}

void setAttribute( enum TYPE type, struct FACENUM* pFaceNum ){
}


void setCurrentObj( const char* name ){
}

struct ATTRIBUTE_VN_LIST* getLastAttributeVN( struct OBJECT_ATTRIBUTES* pObject ){

	struct ATTRIBUTE_VN_LIST* pAttributeVNList = (*pObject).pAttributeVNList;
	if( pAttributeVNList == NULL ){
		return pAttributeVNList;
	}

	while( (*pAttributeVNList).pNext != NULL ){
		pAttributeVNList = (*pAttributeVNList).pNext;
	}

	return pAttributeVNList;
}

struct ATTRIBUTE_V_LIST* getLastAttributeV( struct OBJECT_ATTRIBUTES* pObject ){

	struct ATTRIBUTE_V_LIST* pAttributeVList = (*pObject).pAttributeVList;
	if( pAttributeVList == NULL ){
		return pAttributeVList;
	}

	while( (*pAttributeVList).pNext != NULL ){
		pAttributeVList = (*pAttributeVList).pNext;
	}

	return pAttributeVList;
}

struct ATTRIBUTE_V* createAttributeV( struct VERTNUM* pVertNum ){

	struct ATTRIBUTE_V* pAttributeV = malloc( sizeof(struct ATTRIBUTE_V) );
	memset( pAttributeV, 0, sizeof(struct ATTRIBUTE_V) );

	(*pAttributeV).pVertNum = pVertNum;

	struct ATTRIBUTE_V_LIST* pAttributeVL = malloc( sizeof(struct ATTRIBUTE_V_LIST) );
	memset( pAttributeVL, 0, sizeof(struct ATTRIBUTE_V_LIST) );

	(*pAttributeVL).pAttributeV = pAttributeV;
	(*pAttributeVL).pNext = NULL;

	if( currentObj != NULL ){
		struct ATTRIBUTE_V_LIST* pAttributeVList = getLastAttributeV( currentObj );
		if( pAttributeVList == NULL ){
			(*currentObj).pAttributeVList = pAttributeVL;
		}
		else{
			(*pAttributeVList).pNext = pAttributeVL;
		}
	}
	else{
		abort();
	}

	printf( "createObjectAttributeV();\n" );  

	return pAttributeV;

}

struct ATTRIBUTE_VN* createAttributeVN( struct VERTNUM* pVertNum ){

	struct ATTRIBUTE_VN* pAttributeVN = malloc( sizeof(struct ATTRIBUTE_VN) );
	memset( pAttributeVN, 0, sizeof(struct ATTRIBUTE_VN) );

	(*pAttributeVN).pVertNum = pVertNum;

	struct ATTRIBUTE_VN_LIST* pAttributeVNL = malloc( sizeof(struct ATTRIBUTE_VN_LIST) );
	memset( pAttributeVNL, 0, sizeof(struct ATTRIBUTE_VN_LIST) );

	(*pAttributeVNL).pAttributeVN = pAttributeVN;
	(*pAttributeVNL).pNext = NULL;

	if( currentObj != NULL ){
		struct ATTRIBUTE_VN_LIST* pAttributeVNList = getLastAttributeVN( currentObj );
		if( pAttributeVNList == NULL ){
			(*currentObj).pAttributeVNList = pAttributeVNL;
		}
		else{
			(*pAttributeVNList).pNext = pAttributeVNL;
		}
	}
	else{
		abort();
	}

	printf( "createObjectAttributeVN();\n" );  

	return pAttributeVN;

}

struct ATTRIBUTE_F_LIST* getLastAttributeF( struct OBJECT_ATTRIBUTES* pObject ){

	struct ATTRIBUTE_F_LIST* pAttributeFList = (*pObject).pAttributeFList;
	if( pAttributeFList == NULL ){
		return pAttributeFList;
	}

	while( (*pAttributeFList).pNext != NULL ){
		pAttributeFList = (*pAttributeFList).pNext;
	}

	return pAttributeFList;
}

struct ATTRIBUTE_F* createAttributeF( struct FACENUM* pFaceNum ){

	struct ATTRIBUTE_F* pAttributeF = malloc( sizeof(struct ATTRIBUTE_F) );
	memset( pAttributeF, 0, sizeof(struct ATTRIBUTE_F) );

	(*pAttributeF).type = enumF;
	(*pAttributeF).pFaceNum = pFaceNum;

	struct ATTRIBUTE_F_LIST* pAttributeFL = malloc( sizeof(struct ATTRIBUTE_F_LIST) );
	memset( pAttributeFL, 0, sizeof(struct ATTRIBUTE_F_LIST) );

	(*pAttributeFL).pAttributeF = pAttributeF;

	if( currentObj != NULL ){
		struct ATTRIBUTE_F_LIST* pAttributeFList = getLastAttributeF( currentObj );
		if( pAttributeFList == NULL ){
			(*currentObj).pAttributeFList = pAttributeFL;
		}
		else{
			(*pAttributeFList).pNext = pAttributeFL;
		}

	}
	else{
		abort();
	}

	printf( "createObjectAttributeF();\n" );  

	return pAttributeF;

}

struct OBJECT_ATTRIBUTES* createObjectAttributes( const char* name ){

	struct OBJECT_ATTRIBUTES* pObjectAttributes = lookup( name );
	currentObj = pObjectAttributes;

	printf( "createObjectAttributes( %s );\n", name );  

	return pObjectAttributes;
}

void dump_all_objects(){

	if( currentObj != NULL ){
		printf( "o %s\n", (*currentObj).pObjectName );
	}
	else{
		printf( "o (anonymous)\n" );
	}

	struct ATTRIBUTE_V_LIST* pVL = (*currentObj).pAttributeVList;
	while( pVL != NULL ){

		struct ATTRIBUTE_V* pV = (*pVL).pAttributeV;
		printf( "v %#6.6g %#6.6g %#6.6g\n", (*(*pV).pVertNum).x, (*(*pV).pVertNum).y, (*(*pV).pVertNum).z );

		pVL = (*pVL).pNext;
	}

	struct ATTRIBUTE_VN_LIST* pVNL = (*currentObj).pAttributeVNList;
	while( pVNL != NULL ){

		struct ATTRIBUTE_VN* pVN = (*pVNL).pAttributeVN;
		printf( "vn %#6.6g %#6.6g %#6.6g\n", (*(*pVN).pVertNum).x, (*(*pVN).pVertNum).y, (*(*pVN).pVertNum).z );

		pVNL = (*pVNL).pNext;
	}

	struct ATTRIBUTE_F_LIST* pFL = (*currentObj).pAttributeFList;
	while( pFL != NULL ){

		struct ATTRIBUTE_F* pF = (*pFL).pAttributeF;
		struct FACENUM* pFaceNum = (*pF).pFaceNum;

		printf( "f " );

		do{
			switch( (*pFaceNum).type ){
				case enumFaceNumType1:
					printf( "%d/%d/%d ", (*pFaceNum).vertnum, (*pFaceNum).texnum, (*pFaceNum).normnum );
					break;
				case enumFaceNumType2:
					printf( "%d/%d ", (*pFaceNum).vertnum, (*pFaceNum).texnum );
					break;
				case enumFaceNumType3:
					printf( "%d//%d ", (*pFaceNum).vertnum, (*pFaceNum).normnum );
					break;
				case enumFaceNumType4:
					printf( "%d ", (*pFaceNum).vertnum );
					break;
				default:
					abort();
			}
		}while( pFaceNum = (*pFaceNum).pNext );

		printf( "\n" );

		pFL = (*pFL).pNext;
	}


}
