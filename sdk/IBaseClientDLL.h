#pragma once

#include "../utilities/utilities.h"
#include "a.h"
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h

class ClientClass;
class ClientClass2;

class IBaseClientDLL {
public:


	// request a pointer to the list of client datatable classes
	virtual_fn( get_all_classes( ), 8, ClientClass* ( __thiscall* )( void* ) );
	virtual_fn( get_all_classes2( ), 8, ClientClass2* ( __thiscall* )( void* ) );
};