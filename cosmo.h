#pragma once



#include <stdbool.h>


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;





typedef union COS_Cell
{
    u64 val;
    void* ptr;
} COS_Cell;




typedef bool(*COS_CellFromStr)(void* pool, const char* str, COS_Cell* out);
typedef void(*COS_CellDtor)(void* pool, COS_Cell* x);
typedef void(*COS_CellCopier)(void* pool, const COS_Cell* x, COS_Cell* out);
typedef void*(*COS_PoolCtor)(void);
typedef void(*COS_PoolDtor)(void* pool);
typedef u32(*COS_CellPrinter)(void* pool, const COS_Cell* x, char* buf, u32 bufLen);
typedef u32(*COS_CellDumper)(const COS_Cell* x, char* buf, u32 bufSize);
typedef void(*COS_CellLoader)(COS_Cell* x, const char* ptr, u32 size);

typedef struct COS_TypeInfo
{
    const char* name;
    COS_CellFromStr formStr;
    COS_CellDtor dtor;
    COS_CellCopier copier;
    COS_PoolCtor poolCtor;
    COS_PoolDtor poolDtor;
    COS_CellPrinter printer;
    COS_CellDumper dumper;
    COS_CellLoader loader;
    u64 flags;
} COS_TypeInfo;





typedef struct COS_LocalType
{
    u32 locId;
    u32 modId;
} COS_LocalType;

typedef void(*COS_OprCall)(void*** poolTable, const COS_Cell* ins, COS_Cell* outs);

enum
{
    COS_OprIO_MAX = 16,
};

typedef struct COS_OprInfo
{
    const char* name;
    u32 numIns;
    COS_LocalType ins[COS_OprIO_MAX];
    u32 numOuts;
    COS_LocalType outs[COS_OprIO_MAX];
    COS_OprCall call;
    bool hasSideEffect;
} COS_OprInfo;






typedef struct COS_Modu
{
    const char* name;
    u32 numTypes;
    const COS_TypeInfo* typeInfos;
    u32 numOprs;
    const COS_OprInfo* oprInfos;
    u32 numDeps;
    const char** deps;
} COS_Modu;

u32 COS_moduTypeId(COS_Modu* modu, const char* name);
u32 COS_moduOprId(COS_Modu* modu, const char* name);






typedef struct COS_Lang COS_Lang;

COS_Lang* COS_langNew(u32 numModus, const COS_Modu* modus);
void COS_langFree(COS_Lang* lang);

u32 COS_langModuId(COS_Lang* lang, const char* name);






typedef struct COS_Code COS_Code;

typedef struct COS_Block { u32 id; } COS_Block;
typedef struct COS_Var { u32 id; } COS_Var;

typedef struct COS_Type
{
    u32 modId;
    u32 locId;
} COS_Type;

typedef struct COS_Opr
{
    u32 modId;
    u32 locId;
} COS_Opr;


COS_Code* COS_codeNew(COS_Lang* lang);
void COS_codeFree(COS_Code* code);


COS_Block COS_codeAddBlock(COS_Code* code);


COS_Var COS_blockAddPopVar(COS_Code* code, COS_Block blk);
void COS_blockAddPushImm(COS_Code* code, COS_Block blk, COS_Type type, const char* str);
void COS_blockAddPushVar(COS_Code* code, COS_Block blk, COS_Var v);
void COS_blockAddPushBlock(COS_Code* code, COS_Block blk, COS_Block b);
void COS_blockAddCall(COS_Code* code, COS_Block blk, COS_Block callee);
void COS_blockAddApply(COS_Code* code, COS_Block blk);
void COS_blockAddOpr(COS_Code* code, COS_Block blk, COS_Opr opr);


u32 COS_codeDump(COS_Code* code, char* buf, u32 bufSize);
bool COS_codeLoad(COS_Code* code, const char* data, u32 dataSize);















































































































