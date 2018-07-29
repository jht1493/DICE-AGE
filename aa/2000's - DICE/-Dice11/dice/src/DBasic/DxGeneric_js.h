// --------------------------------------------------------------------------------
//
// Access Generic DICE object
//
// 2005.02.22 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "jsapi.h"
#include "DxGeneric.h"

JS_BEGIN_EXTERN_C

// --------------------------------------------------------------------------------
DxGeneric *ds_GenericGetPrivate(JSContext *cx, jsval obj);

// --------------------------------------------------------------------------------
void *ds_GenericGetProp(JSContext *cx, jsval obj, DxPropID propId, DxPropType propType);

// --------------------------------------------------------------------------------
// Final delete our object
void ds_GenericFinalize(JSContext *cx, JSObject *obj);

// --------------------------------------------------------------------------------
// Call Process func on our object.
JSBool ds_Generic_process(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

JS_END_EXTERN_C