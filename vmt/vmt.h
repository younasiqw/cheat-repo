#pragma once
#include "../common_includes.h"

#include <cstdint>
#include <memory>

class c_vmt {
public:
	// class initializer
	c_vmt( );

	// sets up hook and replaces the vtable with new one
	bool initialize( void* base );

	// hook a specific index ( all index are shifted by one because of rtti )
	bool hook_function( const std::uint16_t index, void* function );

	// unhook specific index
	bool unhook_function( const std::uint16_t index );

	// restore old vtables ( unhook all function )
	bool unhook( void );

	// get a pointer to the original function given an index
	template <typename T = void*> T get_original_function( const std::uint16_t index ) {
		return reinterpret_cast< T >(original_vmt[ index ]);
	}

private:
	// calculates the Length of the table
	std::uint32_t get_vtable_length( std::uintptr_t* table );

	std::uintptr_t** vmt_base = nullptr;
	std::uint16_t table_length = 0;
	std::uintptr_t* original_vmt = nullptr;
	std::unique_ptr<uintptr_t[ ]> replace_vmt = nullptr;
	DWORD old_protection{ };
};

class VMTHook
{
public:
	VMTHook( void* ppClass )
	{
		ppBaseClass = static_cast< std::uintptr_t** >( ppClass );

		// loop through all valid class indexes. When it will hit invalid (not existing) it will end the loop
		while ( static_cast< std::uintptr_t* >( *ppBaseClass )[ indexCount ] )
			++indexCount;

		const std::size_t kSizeTable = indexCount * sizeof( std::uintptr_t );

		pOriginalVMT = *ppBaseClass;
		pNewVMT = std::make_unique<std::uintptr_t[ ]>( indexCount );

		// copy original vtable to our Local copy of it
		std::memcpy( pNewVMT.get( ), pOriginalVMT, kSizeTable );

		// replace original class with our Local copy
		*ppBaseClass = pNewVMT.get( );
	};
	~VMTHook( ) { *ppBaseClass = pOriginalVMT; };

	template<class Type>
	Type GetOriginal( const std::size_t index )
	{
		return reinterpret_cast< Type >( pOriginalVMT[ index ] );
	};

	HRESULT Hook( const std::size_t index, void* fnNew )
	{
		if ( index > indexCount )   // check if given index is valid
			return E_INVALIDARG;

		pNewVMT[ index ] = reinterpret_cast< std::uintptr_t >( fnNew );
		return S_OK;
	};

	HRESULT Unhook( const std::size_t index )
	{
		if ( index > indexCount )
			return E_INVALIDARG;

		pNewVMT[ index ] = pOriginalVMT[ index ];
		return S_OK;
	};

private:
	std::unique_ptr<std::uintptr_t[ ]> pNewVMT = nullptr;    // Actual used vtable
	std::uintptr_t** ppBaseClass = nullptr;             // Saved pointer to original class
	std::uintptr_t* pOriginalVMT = nullptr;             // Saved original pointer to the VMT
	std::size_t                       indexCount = 0;                     // Count of indexes inside out f-ction
};