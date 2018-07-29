// --------------------------------------------------------------------------------
//
// Image - wrap gworld
//
// 2005.02.21 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxImage_js.h"
#include "DxImage.h"

#include "jscntxt.h"
#include "jslock.h"
#include "dicejs.h"

// --------------------------------------------------------------------------------

#define DS_CLASS_NAME "Image"


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

static JSBool
ds_drawText(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"process",            ds_process,            0,0,0},
    {"drawText",           ds_drawText,            0,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_WIDTH_PROP,
    DS_HEIGHT_PROP,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"width",		DS_WIDTH_PROP,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"height",		DS_HEIGHT_PROP,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
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
void *
ds_Image_GetPrivate(JSContext *cx, JSObject *obj, jsval *argv)
{
	return JS_GetInstancePrivate(cx, obj, &ds_Class, argv );
}

// --------------------------------------------------------------------------------
static JSBool
ds_process(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxImage 	*ucontrol;

	ucontrol =  (DxImage*) ds_Image_GetPrivate(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
	//ucontrol->Process();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
// drawText("Text", x, y, size );
//
static JSBool
ds_drawText(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool 				ok;
    DxImage 	*ucontrol;
	int32		locX = 100, locY = 100, size = 50;
	char		*str;
	int32		len;
	
	ucontrol =  (DxImage*) ds_Image_GetPrivate(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
    ok = JS_ConvertArguments(cx, argc, argv, "s/iii", &str, &locX, &locY, &size);
    if (! ok)
    	goto exit;
	
	len = ucontrol->DrawText(str, locX, locY, size);
	
	JS_NewNumberValue(cx, len, rval);

exit:;
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
// Image( width, height, depth, paletteResid )
//
static JSBool
ds_Constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool 				ok;
	int32				width = 640, height = 480;
	int32				depth = 32;
	int32				resID;
	DxImage			*ucontrol;

	Rect 			bounds;
	
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
    
    ok = JS_ConvertArguments(cx, argc, argv, "/iiii", &width, &height, &depth, &resID);
    if (! ok)
    	goto exit;

	bounds.left = 0;
	bounds.top = 0;
	bounds.right = width;
	bounds.bottom = height;

    ucontrol = new DxImage( obj, depth, bounds, resID );
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
static void
ds_finalize(JSContext *cx, JSObject *obj)
{
    DxImage 	*ucontrol;

    ucontrol = (DxImage *) JS_GetPrivate(cx, obj);
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
	int32			num;
    DxImage 	*ucontrol;

	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
		
    JS_LOCK_OBJ(cx, obj);
    
	ucontrol = (DxImage*) JS_GetPrivate(cx, obj);
	if (! ucontrol)
		goto exit;
		
    switch (slot)
    {
    	case DS_WIDTH_PROP:
    	{
    		if (doSet)
    		{
    		}
    		else
    		{
    			num = ucontrol->GetWidth();
    			ok = JS_NewNumberValue(cx, num, vp);
    		}
    		break;
    	}
    	case DS_HEIGHT_PROP:
    	{
    		if (doSet)
    		{
    		}
    		else
    		{
    			num = ucontrol->GetHeight();
    			ok = JS_NewNumberValue(cx, num, vp);
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
ds_Image_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

