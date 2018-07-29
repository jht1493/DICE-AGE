// --------------------------------------------------------------------------------
//
// Video Source Input
//
// 2005.02.21 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxProjector_js.h"
#include "DxProjector.h"

#include "jscntxt.h"
#include "jslock.h"
#include "dicejs.h"

#include "DxGeneric_js.h"

// --------------------------------------------------------------------------------

#define DS_CLASS_NAME "Projector"

// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static void
ds_finalize(JSContext *cx, JSObject *obj);

// --------------------------------------------------------------------------------
static JSBool
ds_process(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);


// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"process",            ds_process,            0,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_PROP_input,
    DS_PROP_output,
    DS_PROP_inkMode,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"input",			DS_PROP_input,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"output",			DS_PROP_output,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"inkMode",			DS_PROP_inkMode,	JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {0,0,0,0,0}
};

// --------------------------------------------------------------------------------
static JSClass ds_Class = {
    DS_CLASS_NAME,
    JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,  JS_PropertyStub,  ds_getProperty, 	ds_setProperty,
    JS_EnumerateStub, JS_ResolveStub,   JS_ConvertStub,     ds_finalize,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

// --------------------------------------------------------------------------------
static DxProjector *
ds_DxProjector_GetPrivate(JSContext *cx, JSObject *obj, jsval *argv)
{
	return (DxProjector*) JS_GetInstancePrivate(cx, obj, &ds_Class, argv );
}

// --------------------------------------------------------------------------------
static JSBool
ds_process(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxProjector 	*ucontrol;

	ucontrol =  ds_DxProjector_GetPrivate(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
	ucontrol->Process();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
// Projector( )
//
static JSBool
ds_Constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool 				ok;
	DxProjector			*ucontrol;
	int32				inkMode = srcCopy;
	
	/* PushButton called as function 
	If called with new, replace with new PushButton object
	*/
    if (!(cx->fp->flags & JSFRAME_CONSTRUCTING)) 
    {
		obj = js_NewObject(cx, &ds_Class, NULL, NULL);
		if (!obj)
		    return JS_FALSE;
		*rval = OBJECT_TO_JSVAL(obj);
    }
    
    ok = JS_ConvertArguments(cx, argc, argv, "/i", &inkMode);
    if (! ok)
    	goto exit;
    
    ucontrol = new DxProjector( obj );
    if (! ucontrol)
    {
    	ok = JS_FALSE;
    	goto exit;
    }
    ucontrol->inkMode = inkMode;
    
    ok = JS_SetPrivate(cx, obj, ucontrol);
 
 exit:;
    return ok;
}

// --------------------------------------------------------------------------------
static void
ds_finalize(JSContext *cx, JSObject *obj)
{
    DxProjector 	*ucontrol;

    ucontrol = (DxProjector *) JS_GetPrivate(cx, obj);
    if (!ucontrol)
		return;

	delete ucontrol;
}

// --------------------------------------------------------------------------------
static JSBool
ds_accessProperty(JSContext *cx, JSBool doSet, JSObject *obj, jsval id, jsval *vp)
{
    JSBool 			ok = JS_FALSE;
    jsint 			slot;
	//int32			num;
    DxProjector 	*ucontrol;

	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
		
    JS_LOCK_OBJ(cx, obj);
    
	ucontrol = (DxProjector*) JS_GetPrivate(cx, obj);
	if (! ucontrol)
		goto exit;
		
    switch (slot)
    {
    	case DS_PROP_input:
    	{
    		if (doSet)
    		{
    			ucontrol->input = (DxSurface*)ds_GenericGetProp( cx, *vp, 
    												kDxProp_value, kDxType_DxSurface);
    		}
    		else
    		{
    		}
    		break;
    	}
    	case DS_PROP_output:
    	{
    		if (doSet)
    		{
    			ucontrol->output = (DxSurface*)ds_GenericGetProp( cx, *vp, 
    												kDxProp_value, kDxType_DxSurface);
    		}
    		else
    		{
    		}
    		break;
    	}
    	case DS_PROP_inkMode:
    	{
			jsdouble			num;
			
    		if (doSet)
    		{
	    		if (!JS_ValueToNumber(cx, *vp, &num))
		    		goto exit;
	    		ucontrol->inkMode = num;
    		}
    		else
    		{
    			ok = JS_NewNumberValue(cx, ucontrol->inkMode, vp);
    		}
    	}
    }
    ok = JS_TRUE;

exit:;
    JS_UNLOCK_OBJ(cx, obj);
    return ok;
}

// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	return ds_accessProperty( cx, FALSE/*doSet*/, obj, id, vp );
}

// --------------------------------------------------------------------------------
static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	return ds_accessProperty( cx, TRUE/*doSet*/, obj, id, vp );
}

// --------------------------------------------------------------------------------
JSObject *
ds_Projector_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
