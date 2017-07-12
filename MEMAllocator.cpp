//=======================================================================================
//
//  [ MEMAllocator ] Wii sample program
//  Wii メモリーアロケーションプログラム
//
//=======================================================================================

#include    <revolution.h>
#include    <math.h>

#include    <revolution/mem.h> 

#include    "MEMAllocator.h"

void         *arenaLo, *arenaHi;

//-------------------------------------------------------------
//  メモリの初期化
//-------------------------------------------------------------
void init_memory( void )
{
    OSHeapHandle  heap;


    //----- 空き領域を調べる
    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();

    //----- 空き領域いっぱいにヒープを１つ作ることを宣言
    arenaLo = OSInitAlloc( arenaLo, arenaHi, 1 );
    OSSetArenaLo( arenaLo );    // 空き領域が変化するので再セット


    //----- 空き領域いっぱいにヒープを作ってカレントに設定する
    heap = OSCreateHeap( arenaLo, arenaHi );
    (void)OSSetCurrentHeap( heap );
    //----- 空き領域を使い果たしたことをハッキリさせておく
    arenaLo = arenaHi;


    OSSetArenaLo( arenaLo );
}



//-------------------------------------------------------------
//  メモリの初期化(MEM2)
//  
//  フレームヒープからメモリの確保と解放を行うようにアロケータを初期化する。
//  アロケータを通じて確保される全てのメモリブロックのアライメント値を指定する。
//-------------------------------------------------------------
static MEMAllocator  s_mem2Allocator;
static MEMHeapHandle s_handle;

void init_memory2( void )
{
    void *lo = OSGetMEM2ArenaLo();
    void *hi = OSGetMEM2ArenaHi();
//    s_handle = MEMCreateFrmHeap( lo, (u32)hi - (u32)lo );
    s_handle = MEMCreateFrmHeap( lo, 0x100000 );
    if ( s_handle == MEM_HEAP_INVALID_HANDLE )
    {
        OSHalt("MEM2 heap allocation error.\n");
    }
    else
    {
        OSSetMEM2ArenaLo((void *)((u32)lo+0x100000));
        // アライメント設定
        MEMInitAllocatorForFrmHeap( &s_mem2Allocator, s_handle, 32 );   // buffer requies 32byte alignment.
    }
    
}

void* alloc32( u32 size )
{
    return MEMAllocFromAllocator( &s_mem2Allocator, size );
}

u8 free32( void *addr )
{
    MEMFreeToAllocator( &s_mem2Allocator, addr );
    return 1;
}

