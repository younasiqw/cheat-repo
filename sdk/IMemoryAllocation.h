#pragma once

struct _CrtMemState;
#define MEMALLOC_VERSION 1
typedef size_t( *MemAllocFailHandler_t )( size_t );
#define HANDLE( type )				void*
class IMemoryAllocate
{
public:
	virtual ~IMemoryAllocate( );

	virtual void* Alloc( size_t nSize ) = 0;
	virtual void* Realloc( void* pMem, size_t nSize ) = 0;
	virtual void Free( void* pMem ) = 0;
	virtual void* Expand_NoLongerSupported( void* pMem, size_t nSize ) = 0;
	virtual size_t GetSize( void* pMem ) = 0;
	virtual void PushAllocDbgInfo( const char* pFileName, int nLine ) = 0;
	virtual void PopAllocDbgInfo( ) = 0;
	virtual long CrtSetBreakAlloc( long lNewBreakAlloc ) = 0;
	virtual	int CrtSetReportMode( int nReportType, int nReportMode ) = 0;
	virtual int CrtIsValidHeapPointer( const void* pMem ) = 0;
	virtual int CrtIsValidPointer( const void* pMem, unsigned int size, int access ) = 0;

	virtual int CrtCheckMemory( void ) = 0;
	virtual int CrtSetDbgFlag( int nNewFlag ) = 0;
	virtual void CrtMemCheckpoint( _CrtMemState* pState ) = 0;
	virtual void DumpStats( ) = 0;
	virtual void DumpStatsFileBase( char const* pchFileBase ) = 0;

	virtual void* CrtSetReportFile( int nRptType, void* hFile ) = 0;
	virtual void* CrtSetReportHook( void* pfnNewHook ) = 0;
	virtual int CrtDbgReport( int nRptType, const char* szFile,
		int nLine, const char* szModule, const char* pMsg ) = 0;

	virtual int heapchk( ) = 0;

	virtual bool IsDebugHeap( ) = 0;

	virtual void GetActualDbgInfo( const char*& pFileName, int& nLine ) = 0;
	virtual void RegisterAllocation( const char* pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime ) = 0;
	virtual void RegisterDeallocation( const char* pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime ) = 0;

	virtual int GetVersion( ) = 0;

	virtual void CompactHeap( ) = 0;

	virtual MemAllocFailHandler_t SetAllocFailHandler( MemAllocFailHandler_t pfnMemAllocFailHandler ) = 0;

	virtual void DumpBlockStats( void* ) = 0;

#if defined( _MEMTEST )	
	virtual void SetStatsExtraInfo( const char* pMapName, const char* pComment ) = 0;
#endif
	virtual size_t MemoryAllocFailed( ) = 0;
	virtual int  GetDebugInfoSize( ) = 0;
	virtual void SaveDebugInfo( void* pvDebugInfo ) = 0;
	virtual void RestoreDebugInfo( const void* pvDebugInfo ) = 0;
	virtual void InitDebugInfo( void* pvDebugInfo, const char* pchRootFileName, int nLine ) = 0;
	virtual void GlobalMemoryStatus( size_t* pUsedMemory, size_t* pFreeMemory ) = 0;
};