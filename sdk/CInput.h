#pragma once
class CInput {
public:
	template<typename T>
	T getvfunc( const void* instance, const std::uint32_t index ) {
		if ( instance == nullptr )
			return ( T )nullptr;

		return ( T ) ( ( *( void*** ) instance )[ index ] );
	}

	c_usercmd* get_user_uCmd( int nSlot, int sequence_number ) {
		typedef c_usercmd* ( __thiscall* GetUseruCmd_t )( void*, int, int );
		return getvfunc<GetUseruCmd_t>( this, 8 )( this, nSlot, sequence_number );
	}
};