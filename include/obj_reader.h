#ifndef _OBJ_READER_
#define _OBJ_READER_

extern int yylineno;

void yyerror(char* s, ...);
extern int yyparse();
extern int yylex();

/* type definition */
enum TYPE {
	enumFACENUM = 1,
	enumFACENUMS,
	enumV,
	enumVN,
	enumVT,
	enumS,
	enumF
};

	

/* object table */
struct symbol{
	char* name;
	struct FACENUM* pFaceNum;
};

#define NHASH 9999

struct OBJECT_ATTRIBUTES* lookup( const char* );

struct symlist{
	struct symbol* sym;
	struct symlist* next;
};

struct VERTNUM{
	float x;
	float y;
	float z;
};

extern struct VERT* createVert( float x, float y, float z );	

struct VERTLIST{
	struct VERT* vert;
	struct VERTLIST* next;
};

struct VT{
	float x;
	float y;
};

extern struct VT* createVt( float x, float y );

struct VTLIST{
	struct VT* vt;
	struct VTLIST* next;
};

struct VN{
	float x;
	float y;
	float z;
};

extern struct VN* createVn( float x, float y, float z );	

struct VNLIST{
	struct VN* vn;
	struct VNLIST* next;
};

struct FACENUM{
	enum TYPE type;
	int vertnum;
	int texnum;
	int normnum;
	struct FACENUM* pNext;
};

enum FaceNumType{
	enumFaceNumType1 = 1,
	enumFaceNumType2,
	enumFaceNumType3,
	enumFaceNumType4
};


extern struct FACENUM* createFaceNum( enum FaceNumType type, double vertnum, double texnum, double normnum );
extern struct VERTNUM* createVertNum( double x, double y, double z );

struct ATTRIBUTE_VN{
	enum TYPE type;
	struct VERTNUM* pVertNum;
};

struct ATTRIBUTE_VN_LIST{
	struct ATTRIBUTE_VN* pAttributeVN;
	struct ATTRIBUTE_VN_LIST* pNext;
};

struct ATTRIBUTE_V{
	enum TYPE type;
	struct VERTNUM* pVertNum;
};

struct ATTRIBUTE_V_LIST{
	struct ATTRIBUTE_V* pAttributeV;
	struct ATTRIBUTE_V_LIST* pNext;
};

struct ATTRIBUTE_F{
	enum TYPE type;
	struct FACENUM* pFaceNum;
};

struct ATTRIBUTE_F_LIST{
	struct ATTRIBUTE_F* pAttributeF;
	struct ATTRIBUTE_F_LIST* pNext;
};

extern struct ATTRIBUTE_V* createAttributeV( struct VERTNUM* pVertNum );
extern struct ATTRIBUTE_F* createAttributeF( struct FACENUM* pFaceNum );

struct OBJECT_ATTRIBUTES{
	const char* pObjectName;
	struct ATTRIBUTE_V_LIST* pAttributeVList;
	struct ATTRIBUTE_VN_LIST* pAttributeVNList;
	struct ATTRIBUTE_F_LIST* pAttributeFList;
};

extern void setAttribute( enum TYPE type, struct FACENUM* pFaceNum );
	

struct OBJ{
	char* objName;
	struct VERTLIST* vertlist;
	struct VTLIST* vtlist;
	struct VNLIST* vnlist;
};

extern void setCurrentObj(const char* objName );

struct OBJLIST{
	struct OBJ* obj;
	struct OBJLIST* next;
};

extern struct OBJECT_ATTRIBUTES* createObjectAttributes( const char* name );

struct OBJECT_ATTRIBUTES symtab[NHASH];

extern void dump_all_objects();

#endif

