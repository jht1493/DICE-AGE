// --------------------------------------------------------------------------------
//
// DelayEffect - average up a frame, creates a "Be Still" Effect
//
// 2005.02.26 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "jsapi.h"

JS_BEGIN_EXTERN_C

// --------------------------------------------------------------------------------
JSObject *
ds_DelayEffect_InitClass(JSContext *cx, JSObject *obj);


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

JS_END_EXTERN_C
