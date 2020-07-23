#pragma once
#include <string>
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/dt_recv.h
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/client_class.h

struct RecvProxyData;
struct RecvTable;
struct RecvProp;

using RecvVarProxy_t = void( __cdecl* )(const RecvProxyData*, void*, void*);
using CreateClientClass_t = void* (*)(int, int);
using CreateEvent_t = void* (*)();

struct DVariant {
	union {
		float float_type;
		long int_type;
		char* string_type;
		void* data_type;
		float vector_type[ 3 ];
		__int64 int_64_type;
	};
	int type;
};

struct RecvProxyData {
	const RecvProp* prop;
	DVariant value;
	int element;
	int object_id;
};

struct RecvTable {
	RecvProp* prop;
	int props;
	void* decoder;
	char* net_table_name;
	bool is_initialized;
	bool is_in_main_list;
};
//
struct RecvProp {
	char* var_name;
	int recv_type;
	int flags;
	int string_buffer_size;
	bool is_inside_array;
	const void* extra_data;
	RecvProp* array_prop;
	void* array_length_proxy;
	RecvVarProxy_t var_proxy;
	void* data_table_proxy;
	RecvTable* data_table;
	int offset;
	int element_stride;
	int elements;
	const char* parent_array_prop_name;
};

class ClientClass {
public:
	CreateClientClass_t client;
	CreateEvent_t event;
	char* network_name;
	RecvTable* table;
	ClientClass* next;
	int class_id;
};

enum class ClientFrameStage_t {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class cconvar
{
public:

	void set_value(int value) {
		using OriginalFn = void(__thiscall*)(void*, int);
		return (*(OriginalFn**)this)[16](this, value);
	}
	void set_value(const char* value) {
		using OriginalFn = void(__thiscall*)(void*, const char*);
		return (*(OriginalFn**)this)[14](this, value);
	}
	const char* read_string() {
		using OriginalFn = const char*(__thiscall*)(void*);
		return (*(OriginalFn**)this)[11](this);
	}

	float GetFloat() {
		using OriginalFn = float(__thiscall*)(void*);
		return (*(OriginalFn**)this)[12](this);
	}

	int get_int( void ) const {
		return pParent->nValue;
	}

	const char* get_string(void) const {
		return pParent->pszDefaultValue;
	}
	char pad_0x0000[0x4]; //0x0000
	cconvar* pNext; //0x0004 
	__int32 bRegistered; //0x0008 
	char* pszName; //0x000C 
	char* pszHelpString; //0x0010 
	__int32 nFlags; //0x0014 
	char pad_0x0018[0x4]; //0x0018
	cconvar* pParent; //0x001C 
	char* pszDefaultValue; //0x0020 
	char* strString; //0x0024 
	__int32 StringLength; //0x0028 
	float fValue; //0x002C 
	__int32 nValue; //0x0030 
	__int32 bHasMin; //0x0034 
	float fMinVal; //0x0038 
	__int32 bHasMax; //0x003C 
	float fMaxVal; //0x0040 
	void* fnChangeCallback; //0x0044 

};
class IAppSystem
{
public:
	virtual ~IAppSystem()
	{
	}

	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};
class color;
class IConvar : public IAppSystem
{
public:

	virtual void			func10() = 0;
	virtual void			register_on_command(cconvar* pCommandBase) = 0;
	virtual void			unregister_on_command(cconvar* pCommandBase) = 0;
	virtual void			func13() = 0;
	virtual cconvar* FindVar(const char* var_name) = 0;
	virtual void			func15() = 0;
	virtual void			func16() = 0;
	virtual void			func17() = 0;
	virtual void			func18() = 0;
	virtual void			func19() = 0;
	virtual void			func20() = 0;

	void const ConsoleColorPrintf(const color& clr, const char* pFormat, ...)
	{

		using OriginalFn = void(__cdecl*)(void*, const color&, const char*, ...);

		if (pFormat == nullptr)
			return;

		char buf[8192];

		va_list list;
		va_start(list, pFormat);
		_vsnprintf_s(buf, sizeof(buf) - 1, pFormat, list);
		va_end(list);
		buf[sizeof(buf) - 1] = 0;

		return (*(OriginalFn**)this)[25](this, clr, pFormat);
	}
	virtual void ConsolePrintf(const char* pFormat, ...) const = 0;
	virtual void console_color_printf(const color& clr, const char* pFormat, ...) const = 0;
	virtual void console_printf2(const char* pFormat, ...) const = 0;
	void ConsolePrintf(const char* pFormat, ...)
	{
		using OriginalFn = void(__cdecl*)(void*, const char*, ...);
		return (*(OriginalFn**)this)[26](this, pFormat);
	}
};

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,    // An array of the base types (can't be of datatables).
	DPT_DataTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
	DPT_Quaternion,
#endif
	DPT_NUMSendPropTypes
} SendPropType;

class RecvProp2;
class RecvTable2;
class DVariant2 {
public:
	union {
		float	   m_Float;
		long	   m_Int;
		char* m_pString;
		void* m_pData;
		float	   m_Vector[ 3 ];
		__int64  m_Int64;
	};
	SendPropType  m_Type;
};


class IClientNetworkable;

typedef IClientNetworkable* ( *CreateClientClassFn )( int entnum, int serialNum );
typedef IClientNetworkable* ( *CreateEventFn )( );

class ClientClass2
{
public:
	CreateClientClassFn      pCreateFn;
	CreateEventFn            pCreateEventFn;
	char* pNetworkName;
	RecvTable2* pRecvTable;
	ClientClass2* pNext;
	int						 ClassID;
};

// This is passed into RecvProxy functions.
class CRecvProxyData {
public:
	const RecvProp2* m_pRecvProp2;        // The property it's receiving.
	DVariant2		    m_Value;            // The value given to you to store.
	int				    m_iElement;         // Which array element you're getting.
	int				    m_ObjectID;         // The object being referred to.
};

//-----------------------------------------------------------------------------
// pStruct = the base structure of the datatable this variable is in (like C_BaseEntity)
// pOut    = the variable that this this proxy represents (like C_BaseEntity::SomeValue).
//
// Convert the network-standard-type value in Value into your own format in pStruct/pOut.
//-----------------------------------------------------------------------------
typedef void( *RecvVarProxyFn )( const CRecvProxyData* pData, void* pStruct, void* pOut );

// ------------------------------------------------------------------------ //
// ArrayLengthRecvProxies are optionally used to get the Length of the 
// incoming array when it changes.
// ------------------------------------------------------------------------ //
typedef void( *ArrayLengthRecvProxyFn )( void* pStruct, int objectID, int currentArrayLength );


// NOTE: DataTable receive proxies work differently than the other proxies.
// pData points at the object + the recv table's offset.
// pOut should be set to the location of the object to unpack the data table into.
// If the parent object just contains the child object, the default proxy just does *pOut = pData.
// If the parent object points at the child object, you need to dereference the pointer here.
// NOTE: don't ever return null from a DataTable receive proxy function. Bad things will happen.
typedef void( *DataTableRecvVarProxyFn )( const RecvProp2* pProp, void** pOut, void* pData, int objectID );

class RecvProp2
{
	// This info comes from the receive data table.
public:
	RecvProp2( );

	void                        InitArray( int nElements, int elementStride );

	int                         GetNumElements( ) const;
	void                        SetNumElements( int nElements );

	int                         GetElementStride( ) const;
	void                        SetElementStride( int stride );

	int                         GetFlags( ) const;

	const char* GetName( ) const;
	SendPropType                GetType( ) const;

	RecvTable2* GetDataTable( ) const;
	void                        SetDataTable( RecvTable2* pTable );

	RecvVarProxyFn              GetProxyFn( ) const;
	void                        SetProxyFn( RecvVarProxyFn fn );

	DataTableRecvVarProxyFn     GetDataTableProxyFn( ) const;
	void                        SetDataTableProxyFn( DataTableRecvVarProxyFn fn );

	int                         GetOffset( ) const;
	void                        SetOffset( int o );

	// Arrays only.
	RecvProp2* GetArrayProp( ) const;
	void                        SetArrayProp( RecvProp2* pProp );

	// Arrays only.
	void                        SetArrayLengthProxy( ArrayLengthRecvProxyFn proxy );
	ArrayLengthRecvProxyFn      GetArrayLengthProxy( ) const;

	bool                        IsInsideArray( ) const;
	void                        SetInsideArray( );

	// Some property types bind more data to the prop in here.
	const void* GeTextraData( ) const;
	void                        SeTextraData( const void* pData );

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* GetParentArrayPropName( );
	void                        SetParentArrayPropName( const char* pArrayPropName );

public:

	const char* pVarName;
	SendPropType                RecvType;
	int                         Flags;
	int                         StringBufferSize;


public:

	bool                        bInsideArray;        // Set to true by the engine if this property sits inside an array.

												   // Extra data that certain special property types bind to the property here.
	const void* pExtraData;

	// If this is an array (DPT_Array).
	RecvProp2* pArrayProp;
	ArrayLengthRecvProxyFn      ArrayLengthProxy;

	RecvVarProxyFn              ProxyFn;
	DataTableRecvVarProxyFn     DataTableProxyFn;    // For RDT_DataTable.

	RecvTable2* pDataTable;        // For RDT_DataTable.
	int                         Offset;

	int                         ElementStride;
	int                         nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* pParentArrayPropName;
};

class CRecvDecoder;
class RecvTable2
{
public:

	typedef RecvProp2    PropType;

	RecvTable2( );
	RecvTable2( RecvProp2* pProps, int nProps, const char* pNetTableName );
	~RecvTable2( );

	void                Construct( RecvProp2* pProps, int nProps, const char* pNetTableName );

	int                 GetNumProps( );
	RecvProp2* GetProp( int i );

	const char* GetName( );

	// Used by the engine while initializing array props.
	void                SetInitialized( bool bInitialized );
	bool                IsInitialized( ) const;

	// Used by the engine.
	void                SetInMainList( bool bInList );
	bool                IsInMainList( ) const;


public:

	// Properties described in a table.
	RecvProp2* pProps;
	int                 nProps;

	// The decoder. NOTE: this covers each RecvTable2 AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	CRecvDecoder* pDecoder;

	const char* pNetTableName;    // The name matched between client and server.


private:

	bool                bInitialized;
	bool                bInMainList;
};



inline int RecvTable2::GetNumProps( )
{
	return this->nProps;
}

inline RecvProp2* RecvTable2::GetProp( int i )
{
	return &this->pProps[ i ];
}

inline const char* RecvTable2::GetName( )
{
	return this->pNetTableName;
}

inline void RecvTable2::SetInitialized( bool bInitialized )
{
	this->bInitialized = bInitialized;
}

inline bool RecvTable2::IsInitialized( ) const
{
	return this->bInitialized;
}

inline void RecvTable2::SetInMainList( bool bInList )
{
	this->bInMainList = bInList;
}

inline bool RecvTable2::IsInMainList( ) const
{
	return this->bInMainList;
}


inline void RecvProp2::InitArray( int nElements, int elementStride )
{
	this->RecvType = DPT_Array;
	this->nElements = nElements;
	this->ElementStride = elementStride;
}

inline int RecvProp2::GetNumElements( ) const
{
	return this->nElements;
}

inline void RecvProp2::SetNumElements( int nElements )
{
	this->nElements = nElements;
}

inline int RecvProp2::GetElementStride( ) const
{
	return this->ElementStride;
}

inline void RecvProp2::SetElementStride( int stride )
{
	this->ElementStride = stride;
}

inline int RecvProp2::GetFlags( ) const
{
	return this->Flags;
}

inline const char* RecvProp2::GetName( ) const
{
	return this->pVarName;
}

inline SendPropType RecvProp2::GetType( ) const
{
	return this->RecvType;
}

inline RecvTable2* RecvProp2::GetDataTable( ) const
{
	return this->pDataTable;
}

inline void RecvProp2::SetDataTable( RecvTable2* pTable )
{
	this->pDataTable = pTable;
}

inline RecvVarProxyFn RecvProp2::GetProxyFn( ) const
{
	return this->ProxyFn;
}

inline void RecvProp2::SetProxyFn( RecvVarProxyFn fn )
{
	this->ProxyFn = fn;
}

inline DataTableRecvVarProxyFn RecvProp2::GetDataTableProxyFn( ) const
{
	return this->DataTableProxyFn;
}

inline void RecvProp2::SetDataTableProxyFn( DataTableRecvVarProxyFn fn )
{
	this->DataTableProxyFn = fn;
}

inline int RecvProp2::GetOffset( ) const
{
	return this->Offset;
}

inline void RecvProp2::SetOffset( int o )
{
	this->Offset = o;
}

inline RecvProp2* RecvProp2::GetArrayProp( ) const
{
	return this->pArrayProp;
}

inline void RecvProp2::SetArrayProp( RecvProp2* pProp )
{
	this->pArrayProp = pProp;
}

inline void RecvProp2::SetArrayLengthProxy( ArrayLengthRecvProxyFn proxy )
{
	this->ArrayLengthProxy = proxy;
}

inline ArrayLengthRecvProxyFn RecvProp2::GetArrayLengthProxy( ) const
{
	return this->ArrayLengthProxy;
}

inline bool RecvProp2::IsInsideArray( ) const
{
	return this->bInsideArray;
}

inline void RecvProp2::SetInsideArray( )
{
	this->bInsideArray = true;
}

inline const void* RecvProp2::GeTextraData( ) const
{
	return this->pExtraData;
}

inline void RecvProp2::SeTextraData( const void* pData )
{
	this->pExtraData = pData;
}

inline const char* RecvProp2::GetParentArrayPropName( )
{
	return this->pParentArrayPropName;
}

inline void    RecvProp2::SetParentArrayPropName( const char* pArrayPropName )
{
	this->pParentArrayPropName = pArrayPropName;
}