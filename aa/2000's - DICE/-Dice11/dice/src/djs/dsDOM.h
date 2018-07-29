// --------------------------------------------------------------------------------
// 
// Dice General DOM functions.
// Keeping it simply and mapping directly to Macintosh/QuickTime API
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
//
// -- History:
//
// 2003.04.04	jht	Created.
// 
// -------------------------------------------------------------------------------- 

#pragma once

#include "jsapi.h"

JS_BEGIN_EXTERN_C

// --------------------------------------------------------------------------------
JSObject *
ds_InitDOM(JSContext *cx, JSObject *obj);

// --------------------------------------------------------------------------------
JSBool ds_ConvertUCStringToJSValue( JSContext *cx, jschar *chars, size_t length, jsval *rval);

// --------------------------------------------------------------------------------
JSBool ds_ConvertJSValueToStringUC( JSContext *cx, jsval rval, jschar **chars, size_t *length);

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

JS_END_EXTERN_C
