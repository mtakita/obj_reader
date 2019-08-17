#include <stdio.h>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "obj_reader.h"

extern struct OBJECT_ATTRIBUTES* currentObj;
extern struct MATERIAL* currentMtl;
extern struct USE_MATERIAL* currentUseMaterial;

extern char* yytext;

#define DEBUG 1

#ifndef DEBUG

#define YYLOG( msg ) yylog( char* msg )

void yylog( char* msg ){
	/* empty */
}

#else

#define YYLOG( msg ) yylog( char* msg )

void yylog( char* msg ){
	printf( "yylog: %s\n", msg );
}

#endif


void yyerror(char *s, ...) {
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "yylineno:%d yytext:%s error desc: ", yylineno, yytext );
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

static unsigned symhash( const char* sym){

	unsigned int hash = 0;
	unsigned c;

	while( c = *sym++ ) hash = hash*9^c;

	return hash;
}


struct OBJECT_ATTRIBUTES* lookupObject( const char* sym){

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


struct MATERIAL* createNewmtl( const char* pNewmtlName ){
	struct MATERIAL* pMaterial = createMaterial( pNewmtlName );
	currentMtl = pMaterial;

	return currentMtl;
}

struct ATTRIBUTE_NS* createAttributeNs( float ns ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	struct ATTRIBUTE_NS* pAttributeNs = malloc( sizeof(struct ATTRIBUTE_NS) );
	(*pAttributeNs).ns = ns;
	(*currentMtl).pAttributeNs = pAttributeNs;

	return pAttributeNs;
}

struct ATTRIBUTE_KA* createAttributeKa( float x, float y, float z ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	struct ATTRIBUTE_KA* pAttributeKa = malloc( sizeof(struct ATTRIBUTE_KA) );
	(*pAttributeKa).x = x;
	(*pAttributeKa).y = y;
	(*pAttributeKa).z = z;
	(*currentMtl).pAttributeKa = pAttributeKa;

	return pAttributeKa;
}

struct ATTRIBUTE_KD* createAttributeKd( float x, float y, float z ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	struct ATTRIBUTE_KD* pAttributeKd = malloc( sizeof(struct ATTRIBUTE_KD) );
	(*pAttributeKd).x = x;
	(*pAttributeKd).y = y;
	(*pAttributeKd).z = z;
	(*currentMtl).pAttributeKd = pAttributeKd;

	return pAttributeKd;
}

struct ATTRIBUTE_KS* createAttributeKs( float x, float y, float z ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	struct ATTRIBUTE_KS* pAttributeKs = malloc( sizeof(struct ATTRIBUTE_KS) );
	(*pAttributeKs).x = x;
	(*pAttributeKs).y = y;
	(*pAttributeKs).z = z;
	(*currentMtl).pAttributeKs = pAttributeKs;

	return pAttributeKs;
}

struct ATTRIBUTE_KE* createAttributeKe( float x, float y, float z ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	struct ATTRIBUTE_KE* pAttributeKe = malloc( sizeof(struct ATTRIBUTE_KE) );
	(*pAttributeKe).x = x;
	(*pAttributeKe).y = y;
	(*pAttributeKe).z = z;
	(*currentMtl).pAttributeKe = pAttributeKe;

	return pAttributeKe;
}

struct ATTRIBUTE_NI* createAttributeNi( float ni ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	struct ATTRIBUTE_NI* pAttributeNi = malloc( sizeof(struct ATTRIBUTE_NI) );
	(*pAttributeNi).ni = ni;
	(*currentMtl).pAttributeNi = pAttributeNi;

	return pAttributeNi;
}

struct ATTRIBUTE_D* createAttributeD( float d ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	struct ATTRIBUTE_D* pAttributeD = malloc( sizeof(struct ATTRIBUTE_D) );
	(*pAttributeD).d = d;
	(*currentMtl).pAttributeD = pAttributeD;

	return pAttributeD;
}

struct ATTRIBUTE_ILLUM* createAttributeIllum( float illum ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	struct ATTRIBUTE_ILLUM* pAttributeIllum = malloc( sizeof(struct ATTRIBUTE_ILLUM) );
	(*pAttributeIllum).illum = illum;
	(*currentMtl).pAttributeIllum = pAttributeIllum;

	return pAttributeIllum;
}

const char* createAttributeMapKd( const char* pMapKd ){
	if( !currentMtl ){
		yyerror("current newmtl is not specified.");
		 abort();
	}

	(*currentMtl).pMapKd = strdup( pMapKd );

	return pMapKd;
}

struct MATERIAL_LIBRARY* lookupMaterialLibrary2( const char* pMaterialLibraryName){

	struct MATERIAL_LIBRARY* mL = &libtab[symhash(pMaterialLibraryName)%NHASH];

	int scount = NHASH;

	while(--scount >= 0 ){
		if(mL->pMaterialLibraryName && !strcmp(mL->pMaterialLibraryName, pMaterialLibraryName)) {
			return mL;
		}

		if(!mL->pMaterialLibraryName){
			mL->pMaterialLibraryName = strdup(pMaterialLibraryName);
			mL->pMaterialList = NULL;
			return mL;
		}

		if(++mL >= libtab+NHASH) mL = libtab;
	}

	yyerror("symbol table overflow\n");
	abort();
}

struct MATERIAL_LIBRARY* createMaterialLibrary( const char* pMaterialLibraryName ){
	return lookupMaterialLibrary2( pMaterialLibraryName );
}

struct MATERIAL* lookupMaterial( const char* pMaterialName){

	struct MATERIAL* mtl = &mtltab[symhash(pMaterialName)%NHASH];

	int scount = NHASH;

	while(--scount >= 0 ){
		if(mtl->pMaterialName && !strcmp(mtl->pMaterialName, pMaterialName)) {
			return mtl;
		}

		if(!mtl->pMaterialName){
			mtl->pMaterialName = strdup(pMaterialName);
			mtl->pAttributeNs = NULL;
			mtl->pAttributeKa = NULL;
			mtl->pAttributeKd = NULL;
			mtl->pAttributeKs = NULL;
			mtl->pAttributeKe = NULL;
			mtl->pAttributeNi = NULL;
			mtl->pAttributeD = NULL;
			mtl->pAttributeIllum = NULL;
			mtl->pMapKd = NULL;
			return mtl;
		}

		if(++mtl >= mtltab+NHASH) mtl = mtltab;
	}

	yyerror("symbol table overflow\n");
	abort();
}

struct VERTNUM* createVertNum( double x, double y, double z ){
	struct VERTNUM* pVertNum = malloc( sizeof(struct VERTNUM) );
	(*pVertNum).x = x;
	(*pVertNum).y = y;
	(*pVertNum).z = z;

#ifdef DEBUG
	printf( "createVertNum( %#6.6g, %#6.6g, %#6.6g );\n", x, y, z );
#endif

	return pVertNum;
}

struct VERT2NUM* createVert2Num( double u, double v ){
	struct VERT2NUM* pVert2Num = malloc( sizeof(struct VERT2NUM) );
	(*pVert2Num).u = u;
	(*pVert2Num).v = v;

	return pVert2Num;
}

struct FACENUM* createFaceNum( enum FaceNumType type, double vertnum, double texnum, double normnum ){
	struct FACENUM* pFaceNum = malloc( sizeof(struct FACENUM) );
	(*pFaceNum).type = type;
	(*pFaceNum).vertnum = vertnum;
	(*pFaceNum).texnum = texnum;
	(*pFaceNum).normnum = normnum;
	(*pFaceNum).pNext = NULL;

#ifdef DEBUG
	printf( "createFaceNum( %d, %#6.6g, %#6.6g, %#6.6g );\n", type, vertnum, texnum, normnum );
#endif

	return pFaceNum;
}

// void setAttribute( enum TYPE type, struct FACENUM* pFaceNum ){
// }


// void setCurrentObj( const char* name ){
// }

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

struct ATTRIBUTE_VT_LIST* getLastAttributeVT( struct OBJECT_ATTRIBUTES* pObject ){

	struct ATTRIBUTE_VT_LIST* pAttributeVTList = (*pObject).pAttributeVTList;
	if( pAttributeVTList == NULL ){
		return pAttributeVTList;
	}

	while( (*pAttributeVTList).pNext != NULL ){
		pAttributeVTList = (*pAttributeVTList).pNext;
	}

	return pAttributeVTList;
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

struct ATTRIBUTE_USE_MATERIAL_LIST* getLastAttributeUseMaterialList( struct OBJECT_ATTRIBUTES* pObject ){

	struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList = (*pObject).pAttributeUseMaterialList;
	if( pAttributeUseMaterialList == NULL ){
		return pAttributeUseMaterialList;
	}

	while( (*pAttributeUseMaterialList).pNext != NULL ){
		pAttributeUseMaterialList = (*pAttributeUseMaterialList).pNext;
	}

	return pAttributeUseMaterialList;
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

	return pAttributeV;

}

struct MATERIAL* findMaterial( const char* pUseMaterialName ){

	for( int i = 0; i < NHASH; i++ ){
		if( mtltab[i].pMaterialName != 0 ){
			if( strcasecmp( mtltab[i].pMaterialName, pUseMaterialName ) == 0 ){

				return &mtltab[i];
			}
		}
	}

	return NULL;
}

struct ATTRIBUTE_USE_MATERIAL_LIST* createUseMaterial( char* pUseMaterialName ){

	struct USE_MATERIAL* pUseMaterial = malloc( sizeof(struct USE_MATERIAL) );
	memset( pUseMaterial, 0, sizeof(struct USE_MATERIAL) );

	(*pUseMaterial).pUseMaterialName = strdup( pUseMaterialName );
	(*pUseMaterial).pMaterial = findMaterial( pUseMaterialName );
	if( (*pUseMaterial).pMaterial == NULL ){
		printf( "%s is not found.\n", pUseMaterialName );
	}
	(*pUseMaterial).pAttributeFList = NULL;

	currentUseMaterial = pUseMaterial;

	struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList = malloc( sizeof(struct ATTRIBUTE_USE_MATERIAL_LIST) );
	memset( pAttributeUseMaterialList, 0, sizeof(struct ATTRIBUTE_USE_MATERIAL_LIST) );

	(*pAttributeUseMaterialList).pUseMaterial = pUseMaterial;
	(*pAttributeUseMaterialList).pNext = NULL;

	if( currentObj != NULL ){
		struct ATTRIBUTE_USE_MATERIAL_LIST* pLastAttributeUseMaterialList = getLastAttributeUseMaterialList( currentObj );
		if( pLastAttributeUseMaterialList == NULL ){
			(*currentObj).pAttributeUseMaterialList = pAttributeUseMaterialList;
		}
		else{
			(*pLastAttributeUseMaterialList).pNext = pAttributeUseMaterialList;
		}
	}
	else{
		abort();
	}

	return pAttributeUseMaterialList;

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

	return pAttributeVN;

}

struct ATTRIBUTE_VT* createAttributeVT( struct VERT2NUM* pVert2Num ){

	struct ATTRIBUTE_VT* pAttributeVT = malloc( sizeof(struct ATTRIBUTE_VT) );
	memset( pAttributeVT, 0, sizeof(struct ATTRIBUTE_VT) );

	(*pAttributeVT).pVert2Num = pVert2Num;

	struct ATTRIBUTE_VT_LIST* pAttributeVTL = malloc( sizeof(struct ATTRIBUTE_VT_LIST) );
	memset( pAttributeVTL, 0, sizeof(struct ATTRIBUTE_VT_LIST) );

	(*pAttributeVTL).pAttributeVT = pAttributeVT;
	(*pAttributeVTL).pNext = NULL;

	if( currentObj != NULL ){
		struct ATTRIBUTE_VT_LIST* pAttributeVTList = getLastAttributeVT( currentObj );
		if( pAttributeVTList == NULL ){
			(*currentObj).pAttributeVTList = pAttributeVTL;
		}
		else{
			(*pAttributeVTList).pNext = pAttributeVTL;
		}
	}
	else{
		abort();
	}

	return pAttributeVT;

}

struct ATTRIBUTE_F_LIST* getLastAttributeF( struct OBJECT_ATTRIBUTES* pObject ){

	struct ATTRIBUTE_USE_MATERIAL_LIST* pAttributeUseMaterialList = (*pObject).pAttributeUseMaterialList;
	if( pAttributeUseMaterialList == NULL ){
		return NULL;
	}

	while( (*pAttributeUseMaterialList).pNext != NULL ){
		pAttributeUseMaterialList = (*pAttributeUseMaterialList).pNext;
	}

	
	struct USE_MATERIAL* pUseMaterial = (*pAttributeUseMaterialList).pUseMaterial;
	struct ATTRIBUTE_F_LIST* pAttributeFList = (*pUseMaterial).pAttributeFList;

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
		struct ATTRIBUTE_F_LIST* pLastAttributeFL = getLastAttributeF( currentObj );
		if( pLastAttributeFL == NULL ){
			(*currentUseMaterial).pAttributeFList = pAttributeFL;
		}
		else{
			(*pLastAttributeFL).pNext = pAttributeFL;
		}

	}
	else{
		abort();
	}

	return pAttributeF;

}

struct OBJECT_ATTRIBUTES* createObjectAttributes( char* name ){

	struct OBJECT_ATTRIBUTES* pObjectAttributes = lookupObject( name );
	currentObj = pObjectAttributes;

	return pObjectAttributes;
}

/*
Obtain a number of faces holding.
*/
int getNumOfFaces() {

	int numOfFaces = 0;

	struct ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialList = (*currentObj).pAttributeUseMaterialList;
	while (pUseMaterialList != NULL) {

		struct USE_MATERIAL* pUseMaterial = (*pUseMaterialList).pUseMaterial;
		if( (*pUseMaterial).pAttributeFList )
			continue;

		struct ATTRIBUTE_F_LIST* pAttributeFList = (*pUseMaterial).pAttributeFList;

		while( pAttributeFList != NULL ){

			numOfFaces++;
			pAttributeFList = (*pAttributeFList).pNext;
		}

		pUseMaterialList = (*pUseMaterialList).pNext;
	}

	return numOfFaces;
}

/*
Obtain a number of vertices per face.
*/
int getNumOfIndicesPerFace() {

	int numOfIndicesPerFace = 0;

	struct ATTRIBUTE_F_LIST* pFL = getLastAttributeF( currentObj );
	if (pFL != NULL) {

		struct ATTRIBUTE_F* pF = (*pFL).pAttributeF;
		struct FACENUM* pFaceNum = (*pF).pFaceNum;

		do {
			numOfIndicesPerFace++;

		} while (pFaceNum = (*pFaceNum).pNext);

	}

	return numOfIndicesPerFace;
}

/*
Obtain all faces to specified storage.
*/
void xgetFaces( unsigned int* pFacesSuzannu) {

	struct ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialList = (*currentObj).pAttributeUseMaterialList;
	while (pUseMaterialList != NULL) {

		struct USE_MATERIAL* pUseMaterial = (*pUseMaterialList).pUseMaterial;
		if( (*pUseMaterial).pAttributeFList )
			continue;

		struct ATTRIBUTE_F_LIST* pFL = (*pUseMaterial).pAttributeFList;
		while (pFL != NULL) {

			struct ATTRIBUTE_F* pF = (*pFL).pAttributeF;
			struct FACENUM* pFaceNum = (*pF).pFaceNum;

			do {
				switch ((*pFaceNum).type) {
				case enumFaceNumType1:
#ifdef DEBUG
					printf( "%d/%d/%d ", (*pFaceNum).vertnum, (*pFaceNum).texnum, (*pFaceNum).normnum );
#endif
					*pFacesSuzannu++ = (*pFaceNum).vertnum-1;
					*pFacesSuzannu++ = (*pFaceNum).texnum-1;
					*pFacesSuzannu++ = (*pFaceNum).normnum-1;
					break;

				case enumFaceNumType2:
#ifdef DEBUG
					printf( "%d/%d ", (*pFaceNum).vertnum, (*pFaceNum).texnum );
#endif
					*pFacesSuzannu++ = (*pFaceNum).vertnum - 1;
					*pFacesSuzannu++ = (*pFaceNum).texnum - 1;
					*pFacesSuzannu++ = 0;
					break;
				case enumFaceNumType3:
#ifdef DEBUG
					printf( "%d//%d ", (*pFaceNum).vertnum, (*pFaceNum).normnum );
#endif
					*pFacesSuzannu++ = (*pFaceNum).vertnum - 1;
					*pFacesSuzannu++ = 0;
					*pFacesSuzannu++ = (*pFaceNum).normnum - 1;
					break;
				case enumFaceNumType4:
#ifdef DEBUG
					printf( "%d ", (*pFaceNum).vertnum );
#endif
					*pFacesSuzannu++ = (*pFaceNum).vertnum - 1;
					*pFacesSuzannu++ = 0;
					*pFacesSuzannu++ = 0;
					break;
				default:
					abort();
				}

			} while (pFaceNum = (*pFaceNum).pNext);

			pFL = (*pFL).pNext;
		}
	}
}

/*
Return the length in bytes needed to store valid face values.
*/
int xgetLengthOfFaces() {

	// Obtain the number of faces.
	// number of faces * number of vertices per face * number of component * number of sizeof(int)
	int length = getNumOfFaces() * getNumOfIndicesPerFace() * 3 * sizeof(int);

	return length;
}

/*
Obtain only vertex indicies to specified storage.
*/
void getFacesVertexIndicies(unsigned int* pFacesSuzannu) {

	struct ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialList = (*currentObj).pAttributeUseMaterialList;
	while (pUseMaterialList != NULL) {

		struct USE_MATERIAL* pUseMaterial = (*pUseMaterialList).pUseMaterial;
		if( (*pUseMaterial).pAttributeFList )
			continue;

		struct ATTRIBUTE_F_LIST* pFL = (*pUseMaterial).pAttributeFList;
		while (pFL != NULL) {

			struct ATTRIBUTE_F* pF = (*pFL).pAttributeF;
			struct FACENUM* pFaceNum = (*pF).pFaceNum;

			do {
				switch ((*pFaceNum).type) {
				case enumFaceNumType1:
#ifdef DEBUG
					printf( "%d/%d/%d ", (*pFaceNum).vertnum, (*pFaceNum).texnum, (*pFaceNum).normnum );
#endif
					*pFacesSuzannu++ = (*pFaceNum).vertnum - 1;
					break;

				case enumFaceNumType2:
#ifdef DEBUG
					printf( "%d/%d ", (*pFaceNum).vertnum, (*pFaceNum).texnum );
#endif
					*pFacesSuzannu++ = (*pFaceNum).vertnum - 1;
					break;
				case enumFaceNumType3:
#ifdef DEBUG
					printf( "%d//%d ", (*pFaceNum).vertnum, (*pFaceNum).normnum );
#endif
					*pFacesSuzannu++ = (*pFaceNum).vertnum - 1;
					break;
				case enumFaceNumType4:
#ifdef DEBUG
					printf( "%d ", (*pFaceNum).vertnum );
#endif
					*pFacesSuzannu++ = (*pFaceNum).vertnum - 1;
					break;
				default:
					abort();
				}

			} while (pFaceNum = (*pFaceNum).pNext);

			pFL = (*pFL).pNext;
		}
	}
}

/*
Return the length in bytes needed to store vertex indicies value.
*/
int getLengthOfFacesVertexIndicies() {

	// Obtain the number of faces.
	// number of faces * number of vertices per face * number of sizeof(int)
	int length = getNumOfFaces() * getNumOfIndicesPerFace() * sizeof(int) * 3;

	return length;
}


/*
Obtain all vertices to specified storage.
*/
void getVertices( float* pVerticesSuzannu) {

	struct ATTRIBUTE_V_LIST* pVL = (*currentObj).pAttributeVList;
	while (pVL != NULL) {

		struct ATTRIBUTE_V* pV = (*pVL).pAttributeV;
		*pVerticesSuzannu++ = (*(*pV).pVertNum).x;
		*pVerticesSuzannu++ = (*(*pV).pVertNum).y;
		*pVerticesSuzannu++ = (*(*pV).pVertNum).z;

		pVL = (*pVL).pNext;
	}
}


/*
	Obtain a number of vertices holding.
*/
int getNumOfVertices() {

	int numOfVertices = 0;

	struct ATTRIBUTE_V_LIST* pVL = (*currentObj).pAttributeVList;
	while (pVL != NULL) {

		numOfVertices++;

		pVL = (*pVL).pNext;
	}

	return numOfVertices;
}

/*
Obtain all normals to specified storage.
*/
void getNormals(float* pNormalsSuzannu) {

	struct ATTRIBUTE_VN_LIST* pVNL = (*currentObj).pAttributeVNList;
	while (pVNL != NULL) {

		struct ATTRIBUTE_VN* pVN = (*pVNL).pAttributeVN;
		*pNormalsSuzannu++ = (*(*pVN).pVertNum).x;
		*pNormalsSuzannu++ = (*(*pVN).pVertNum).y;
		*pNormalsSuzannu++ = (*(*pVN).pVertNum).z;

		pVNL = (*pVNL).pNext;
	}
}

/*
Visit the index th item and return the item.
*/
struct ATTRIBUTE_VN* getItemATTRIBUTE_VN_LIST(int index) {

	struct ATTRIBUTE_VN_LIST* pVNL = (*currentObj).pAttributeVNList;

	while (index-- > 0){
		pVNL = (*pVNL).pNext;
	}

	return (*pVNL).pAttributeVN;
}

/*
Obtain all normals to specified storage after rearrangement to along with order of vertices.
*/
void getNormalsWithRearrange(float* pNormalsSuzannu) {

	struct ATTRIBUTE_VN_LIST* pVNL = (*currentObj).pAttributeVNList;

	struct VN* pNormalsSuzannuVN = (struct VN*)pNormalsSuzannu;

	struct ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialList = (*currentObj).pAttributeUseMaterialList;
	while (pUseMaterialList != NULL) {

		struct USE_MATERIAL* pUseMaterial = (*pUseMaterialList).pUseMaterial;
		if( (*pUseMaterial).pAttributeFList )
			continue;

		struct ATTRIBUTE_F_LIST* pFL = (*pUseMaterial).pAttributeFList;
		while (pFL != NULL) {

			struct ATTRIBUTE_F* pF = (*pFL).pAttributeF;
			struct FACENUM* pFaceNum = (*pF).pFaceNum;

			do {

				int srcIndex = (*pFaceNum).normnum - 1;
				int destIndex = (*pFaceNum).vertnum - 1;

				switch ((*pFaceNum).type) {
				case enumFaceNumType1:
#ifdef DEBUG
					printf( "%d/%d/%d ", (*pFaceNum).vertnum, (*pFaceNum).texnum, (*pFaceNum).normnum );
#endif
					pNormalsSuzannuVN[destIndex].x = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->x;
					pNormalsSuzannuVN[destIndex].y = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->y;
					pNormalsSuzannuVN[destIndex].z = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->z;
					break;

				case enumFaceNumType2:
#ifdef DEBUG
					printf( "%d/%d ", (*pFaceNum).vertnum, (*pFaceNum).texnum );
#endif
					pNormalsSuzannuVN[destIndex].x = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->x;
					pNormalsSuzannuVN[destIndex].y = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->y;
					pNormalsSuzannuVN[destIndex].z = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->z;
					break;
				case enumFaceNumType3:
#ifdef DEBUG
					printf( "%d//%d ", (*pFaceNum).vertnum, (*pFaceNum).normnum );
#endif
					pNormalsSuzannuVN[destIndex].x = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->x;
					pNormalsSuzannuVN[destIndex].y = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->y;
					pNormalsSuzannuVN[destIndex].z = (*getItemATTRIBUTE_VN_LIST(srcIndex)).pVertNum->z;
					break;
				case enumFaceNumType4:
#ifdef DEBUG
					printf( "%d ", (*pFaceNum).vertnum );
#endif
					/*
					abort because there is no normals.
					*/
					abort();
					break;
				default:
					abort();
				}

			} while (pFaceNum = (*pFaceNum).pNext);

			pFL = (*pFL).pNext;
		}
	}

}

/*
Obtain a number of normals holding.
*/
int getNumOfNormals() {

	int numOfNormals = 0;

	struct ATTRIBUTE_VN_LIST* pVNL = (*currentObj).pAttributeVNList;
	while (pVNL != NULL) {

		numOfNormals++;

		pVNL = (*pVNL).pNext;
	}

	return numOfNormals;
}

void dump_all_objects(){

	if( currentObj != NULL ){
		printf( "o %s\n", (*currentObj).pObjectName );
	}
	else{
		printf( "o (anonymous)\n" );
	}

	int numOfVertices = 0;

	struct ATTRIBUTE_V_LIST* pVL = (*currentObj).pAttributeVList;
	while( pVL != NULL ){

		struct ATTRIBUTE_V* pV = (*pVL).pAttributeV;
		printf( "v %#6.6g %#6.6g %#6.6g\n", (*(*pV).pVertNum).x, (*(*pV).pVertNum).y, (*(*pV).pVertNum).z );

		numOfVertices++;

		pVL = (*pVL).pNext;
	}

	printf("The number of vertices: %d\n", numOfVertices);

	int numOfTexChords = 0;

	struct ATTRIBUTE_VT_LIST* pVTL = (*currentObj).pAttributeVTList;
	while( pVTL != NULL ){

		struct ATTRIBUTE_VT* pVT = (*pVTL).pAttributeVT;
		printf( "vt %#6.6g %#6.6g\n", (*(*pVT).pVert2Num).u, (*(*pVT).pVert2Num).v );

		numOfTexChords++;

		pVTL = (*pVTL).pNext;
	}

	printf("The number of texture chordinates: %d\n", numOfTexChords);

	int numOfNormals = 0;

	struct ATTRIBUTE_VN_LIST* pVNL = (*currentObj).pAttributeVNList;
	while( pVNL != NULL ){

		struct ATTRIBUTE_VN* pVN = (*pVNL).pAttributeVN;
		printf( "vn %#6.6g %#6.6g %#6.6g\n", (*(*pVN).pVertNum).x, (*(*pVN).pVertNum).y, (*(*pVN).pVertNum).z );

		numOfNormals++;

		pVNL = (*pVNL).pNext;
	}

	printf("The number of normals: %d\n", numOfNormals);

	int numOfFaces = 0;
	struct ATTRIBUTE_USE_MATERIAL_LIST* pUseMaterialList = (*currentObj).pAttributeUseMaterialList;
	while (pUseMaterialList != NULL) {

		struct USE_MATERIAL* pUseMaterial = (*pUseMaterialList).pUseMaterial;
		if( pUseMaterial != NULL ){

			printf( "usemtl %s\n", (*pUseMaterial).pUseMaterialName );
			printf( "map_Kd %s\n", (*(*pUseMaterial).pMaterial).pMapKd );

			struct ATTRIBUTE_F_LIST* pFL = (*pUseMaterial).pAttributeFList;
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

				numOfFaces++;

				pFL = (*pFL).pNext;
			}
		}
	
		pUseMaterialList = (*pUseMaterialList).pNext;
	}

	printf("The number of faces: %d\n", numOfFaces);

}

struct MATERIAL* createMaterial( const char* pMaterialName ){
	return lookupMaterial( pMaterialName );
}


