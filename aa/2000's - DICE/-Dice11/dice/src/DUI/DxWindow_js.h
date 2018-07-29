/* History:

Copyright (c) 2003 John Henry Thompson. All rights reserved.

Dice java script class interface to CNotePlayer.

2003.04.04	jht	Created.

*/

#pragma once

#include "jsapi.h"

JS_BEGIN_EXTERN_C

// --------------------------------------------------------------------------------
JSObject *
ds_Window_InitClass(JSContext *cx, JSObject *obj);

// --------------------------------------------------------------------------------
void *
ds_Window_GetPrivate(JSContext *cx, JSObject *obj, jsval *argv);

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

JS_END_EXTERN_C

