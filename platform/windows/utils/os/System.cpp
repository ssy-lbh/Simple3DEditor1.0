#include <utils/os/System.h>


static HMODULE GetCurrentModule(){
	HMODULE hModule;

	if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
        | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)GetCurrentModule, &hModule)){
		return hModule;
	}
	
	return NULL;
}

HMODULE GetModule(){
    static HMODULE hModule = GetCurrentModule();

    return hModule;
}