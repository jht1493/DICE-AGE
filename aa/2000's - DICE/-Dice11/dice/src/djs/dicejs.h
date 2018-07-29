/* History:

Copyright © 2003 John Henry Thompson. All rights reserved.

2003.03.15	jht	Created. Integrating JavaScript.

*/

#pragma once

#include "jstypes.h"

JS_BEGIN_EXTERN_C

// --------------------------------------------------------------------------------
// Init interface to JavaScript engine.
// returns 0 if ok.
int dicejs_Init(void);

// --------------------------------------------------------------------------------
// Shutdown JavaScript engine interface.
void dicejs_Close(void);

// --------------------------------------------------------------------------------
// Evaluate the contents of string and print to scrit window.
void dicejs_EvalStringPrint( void *docInst, char *buffer, int len );

// --------------------------------------------------------------------------------
// Spend time on script exec
//
void dicejs_SpendTime( );

// --------------------------------------------------------------------------------
// obj is private value of wrapperObj.
// obj is about to be deleted so clear Private on wrapperObj
void dicejs_ClearWrapperObj( void *wrapperObj, void *obj );

// --------------------------------------------------------------------------------
// return runtime globs
//
void dicejs_GetRuntime( void **rt, void **cx, void **glob);

JS_END_EXTERN_C

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
