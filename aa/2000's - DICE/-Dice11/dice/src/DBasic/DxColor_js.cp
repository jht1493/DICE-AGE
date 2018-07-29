// --------------------------------------------------------------------------------
//
// Basic Color object.
//
// 2005.03.01 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxColor_js.h"
#include "DxColor.h"
#include "DxGeneric_js.h"

#include "jscntxt.h"
#include "jslock.h"
#include "dicejs.h"

// --------------------------------------------------------------------------------

#define DS_CLASS_NAME "Color"
#define DS_TYPE		DxColor


// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"process",            ds_Generic_process,            0,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_PROP_red,
    DS_PROP_green,
    DS_PROP_blue,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"red",			DS_PROP_red,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"green",		DS_PROP_green,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"blue",		DS_PROP_blue,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {0,0,0,0,0}
};

// --------------------------------------------------------------------------------
static JSClass ds_Class = {
    DS_CLASS_NAME,
    JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,  JS_PropertyStub,  ds_getProperty, 	ds_setProperty,
    JS_EnumerateStub, JS_ResolveStub,   JS_ConvertStub,     ds_GenericFinalize,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

// --------------------------------------------------------------------------------
// Color( red, green, blue )
//
static JSBool
ds_Constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool 			ok;
	int32			red = 255, green = 255, blue = 255;
	DxColor			*ucontrol;
	
    if (!(cx->fp->flags & JSFRAME_CONSTRUCTING)) 
    {
		obj = js_NewObject(cx, &ds_Class, NULL, NULL);
		if (!obj)
		    return JS_FALSE;
		*rval = OBJECT_TO_JSVAL(obj);
    }
    
    ok = JS_ConvertArguments(cx, argc, argv, "/iii", &red, &green, &blue);
    if (! ok)
    	goto exit;

    ucontrol = new DxColor( obj, red, green, blue );
    if (! ucontrol)
    {
    	ok = JS_FALSE;
    	goto exit;
    }
    
    ok = JS_SetPrivate(cx, obj, ucontrol);
 
 exit:;
    return ok;
}


// --------------------------------------------------------------------------------
static JSBool
ds_accessProperty(JSContext *cx, JSBool doSet, JSObject *obj, jsval id, jsval *vp)
{
    JSBool 			ok = JS_FALSE;
    jsint 			slot;
    DS_TYPE 		*ucontrol;

	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
		
    JS_LOCK_OBJ(cx, obj);
    
	ucontrol = (DS_TYPE*) JS_GetPrivate(cx, obj);
	if (! ucontrol)
		goto exit;
		
    switch (slot)
    {
		jsdouble			num;

    	case DS_PROP_red:
    	{
    		if (doSet)
    		{
	    		if (!JS_ValueToNumber(cx, *vp, &num))
		    		goto exit;
	    		ucontrol->red = (int) num ;
    		}
    		else
    		{
    			ok = JS_NewNumberValue(cx, ucontrol->red, vp);
    		}
    		break;
    	}
    	case DS_PROP_green:
    	{
    		if (doSet)
    		{
	    		if (!JS_ValueToNumber(cx, *vp, &num))
		    		goto exit;
	    		ucontrol->green = (int) num ;
    		}
    		else
    		{
    			ok = JS_NewNumberValue(cx, ucontrol->green, vp);
    		}
    		break;
    	}
    	case DS_PROP_blue:
    	{
    		if (doSet)
    		{
	    		if (!JS_ValueToNumber(cx, *vp, &num))
		    		goto exit;
	    		ucontrol->blue = (int) num ;
    		}
    		else
    		{
    			ok = JS_NewNumberValue(cx, ucontrol->blue, vp);
    		}
    		break;
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
	return ds_accessProperty( cx, JS_FALSE/*doSet*/, obj, id, vp );
}

// --------------------------------------------------------------------------------
static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	return ds_accessProperty( cx, JS_TRUE/*doSet*/, obj, id, vp );
}

// --------------------------------------------------------------------------------
JSObject *ds_Color_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

