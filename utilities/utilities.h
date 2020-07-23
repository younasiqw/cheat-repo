#pragma once

#include "..\common_includes.h"

#include <cstdint>

#include <sstream>
namespace Utilities {
	bool initialize_console( const char* window_name );

	bool unload_console( void );

	void* CaptureInterface( const char* module_name, const char* interface_name );

	std::uint8_t* FindPattern( const char* module_name, const char* signature );

	DWORD pattern_scan(std::string module_name, BYTE* mask, const char* sz_mask);

	template < typename T > static T get_virtual_function( void* base, const std::uint16_t index ) {
		return (*reinterpret_cast< T** >(base))[ index ];
	}
}

#define in_range( x, a, b )	\
	( x >= a && x <= b ) 

#define get_bits( x ) \
	( in_range( x, '0', '9' ) ? ( x - '0' ) : ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) )

#define get_byte( x ) \
	( get_bits( x[ 0 ] ) << 4 | get_bits( x[ 1 ] ) )

#define virtual_fn( function_name, index, type, ... ) \
	auto function_name { \
		return Utilities::get_virtual_function< type >( this, index )( this, __VA_ARGS__ ); \
	};