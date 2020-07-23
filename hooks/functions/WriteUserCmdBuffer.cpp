#include  "../hooks.h"
#include <intrin.h>
namespace n_hooks {
#define FOFFSET( type, ptr, offset ) ( *( type* ) ( ( std::uintptr_t ) (ptr) + ( offset ) ) )

	bool transmit( bool only_reliable ) {
		auto nc = FOFFSET( void*, Interfaces::ClientState, 0x9C );

		if ( only_reliable ) {
			( ( std::uint32_t* ) nc )[ 24 ] = 0;
			*( ( std::uint8_t* ) nc + 100 ) = 0;
		}

		return ( *( int( __stdcall** )( std::uint32_t ) )( *( std::uint32_t* ) nc + 184 ) )( 0 ) != 0;
	}
	void write_user_uCmd( bf_write* buf, c_usercmd* in, c_usercmd* out ) {
		using write_user_uCmd = void( __fastcall* ) ( bf_write*, c_usercmd*, c_usercmd* );
		static write_user_uCmd fn_write_user_uCmd = ( write_user_uCmd ) Utilities::FindPattern( "client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" );

		__asm {
			mov ecx, buf
			mov edx, in
			push out
			call fn_write_user_uCmd
			add esp, 4
		}
	}
	bool __fastcall n_functions::WriteUseruCmdDeltaToBuffer_hook( void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool isnewcommand )
	{
		//static auto OriginalFn = n_hooks::ClientHook->get_original_function< decltype( &WriteUseruCmdDeltaToBuffer_hook ) >( 24 );
		static auto OriginalFn = n_hooks::pClientHook->GetOriginal<WriteUseruCmdDeltaToBufferFn>( 24 );

		if ( Global::tick_base_shift <= 0 )
			return OriginalFn( ecx, slot, buf, from, to, isnewcommand );

		if ( from != -1 )
			return true;

		auto CL_SendMove = [ ] ( )
		{
			using CL_SendMove_t = void( __fastcall* )( void );
			static CL_SendMove_t CL_SendMoveF = ( CL_SendMove_t ) Utilities::FindPattern( "engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98" );

			CL_SendMoveF( );
		};

		auto WriteUseruCmd = [ ] ( bf_write* buf, c_usercmd* in, c_usercmd* out )
		{
			using WriteUseruCmd_t = void( __fastcall* )( bf_write*, c_usercmd*, c_usercmd* );
			static WriteUseruCmd_t WriteUseruCmdF = ( WriteUseruCmd_t ) Utilities::FindPattern( "client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" );

			WriteUseruCmdF( buf, in, out );
		};



		int* pNumBackupCommands = ( int* ) ( reinterpret_cast < uintptr_t > ( buf ) - 0x30 );
		int* pNumNewCommands = ( int* ) ( reinterpret_cast < uintptr_t > ( buf ) - 0x2C );
		int32_t new_commands = *pNumNewCommands;

		int32_t next_uCmdnr = Interfaces::ClientState->lastoutgoingcommand + Interfaces::ClientState->chokedcommands + 1;
		int32_t total_new_commands = min( Global::tick_base_shift, 16 );
		Global::tick_base_shift -= total_new_commands;

		from = -1;
		*pNumNewCommands = total_new_commands;
		*pNumBackupCommands = 0;

		for ( to = next_uCmdnr - new_commands + 1; to <= next_uCmdnr; to++ )
		{
			if ( !OriginalFn( ecx, slot, buf, from, to, isnewcommand ) )
			{
				return false;
			}

			from = to;
		}

		c_usercmd* last_realuCmd = Interfaces::Input->get_user_uCmd( slot, from );
		c_usercmd fromuCmd;

		if ( last_realuCmd )
		{
			fromuCmd = *last_realuCmd;
		}

		c_usercmd touCmd = fromuCmd;
		touCmd.command_number++;
		touCmd.TickCount += 200;

		for ( int i = new_commands; i <= total_new_commands; i++ )
		{
			write_user_uCmd( buf, &touCmd, &fromuCmd );

			fromuCmd = touCmd;

			std::cout << "IN LOOP" << i << std::endl;
			touCmd.command_number++;
			touCmd.TickCount++;
		}

		return false;
	}
}