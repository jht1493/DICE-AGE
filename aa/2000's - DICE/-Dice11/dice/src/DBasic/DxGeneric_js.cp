// --------------------------------------------------------------------------------
//
// Access Generic DICE object
//
// 2005.02.22 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxGeneric_js.h"

// --------------------------------------------------------------------------------
JSBool ds_Generic_process(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	DxGeneric	*ucontrol;
	
	ucontrol = (DxGeneric *) JS_GetPrivate(cx, obj);
	
	if (! ucontrol || ucontrol->safetyTag !=  kDxGenericTag)
	{
		// !!@  error
		return JS_FALSE;
	}
		
	ucontrol->Process();
	
	return JS_TRUE;
}


// --------------------------------------------------------------------------------
// Final delete our object
void ds_GenericFinalize(JSContext *cx, JSObject *obj)
{
	DxGeneric	*ucontrol;
	
	ucontrol = (DxGeneric *) JS_GetPrivate(cx, obj);
	
	if (! ucontrol || ucontrol->safetyTag !=  kDxGenericTag)
	{
		// !!@ Serious internal error
		return ;
	}
	
	delete ucontrol;
}

// --------------------------------------------------------------------------------
// Return the private data of our object
DxGeneric *ds_GenericGetPrivate(JSContext *cx, jsval jval)
{
	DxGeneric	*ucontrol;
    //void		*result = 0;
    JSObject	*obj = 0;
    
    // could simpe use lower level macros, no need to have number etc. converted to obj
    //JS_ValueToObject(cx, jval, &obj);

    if (! JSVAL_IS_OBJECT(jval))
    	return 0;
    
    obj = JSVAL_TO_OBJECT(jval);
    
    if (! obj)
    	return 0;
   	
	ucontrol = (DxGeneric *) JS_GetPrivate(cx, obj);
	
	if (! ucontrol)
		return 0;
	
	if (ucontrol->safetyTag !=  kDxGenericTag)
		return 0;
	
	return ucontrol;
}

// --------------------------------------------------------------------------------
void *ds_GenericGetProp(JSContext *cx, jsval jval, DxPropID propId, DxPropType propType)
{
	DxGeneric	*ucontrol;
    void		*result = 0;
    
	ucontrol = ds_GenericGetPrivate(cx, jval);
	
	if (! ucontrol)
		return 0;
	
	ucontrol->GetProp( propId, propType, &result);
	return result;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
