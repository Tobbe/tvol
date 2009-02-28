#ifndef __TVOL_H_
#define __TVOL_H_

#include "../LiteStep24x/lsapi/lsapi.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec( dllexport ) int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath);
__declspec( dllexport ) void quitModule(HINSTANCE dllInst);

#ifdef __cplusplus
}
#endif


#endif
