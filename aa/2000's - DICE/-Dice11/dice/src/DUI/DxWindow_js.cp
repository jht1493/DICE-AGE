/* 

JavaScript wrapper for note player

Copyright (c) 2003 John Henry Thompson. All rights reserved.

History:

2003.03.20	jht	Created. Integrating JavaScript.

*/

#include "DxWindow_js.h"
#include "DxWindow.h"
#include "dsDOM.h"

#include "jscntxt.h"
#include "jslock.h"

#define DS_CLASS_NAME_UI "Window"

// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static void
ds_finalize(JSContext *cx, JSObject *obj);

static JSBool
ds_Select(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_Show(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_Hide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"select",            ds_Select,            0,0,0},
    {"show",            ds_Show,            0,0,0},
    {"hide",            ds_Hide,            0,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_VISIBLE_PROP,
    DS_ENABLED_PROP,
    DS_TITLE_PROP,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"visible",		DS_VISIBLE_PROP,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"enabled",		DS_ENABLED_PROP,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"title",		DS_TITLE_PROP,			JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {0,0,0,0,0}
};

// --------------------------------------------------------------------------------
static JSClass ds_Class = {
    DS_CLASS_NAME_UI,
    JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,  JS_PropertyStub,  ds_getProperty, 	ds_setProperty,
    JS_EnumerateStub, JS_ResolveStub,   JS_ConvertStub,     ds_finalize,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

// --------------------------------------------------------------------------------
void *
ds_Window_GetPrivate(JSContext *cx, JSObject *obj, jsval *argv)
{
	return JS_GetInstancePrivate(cx, obj, &ds_Class, argv );
}
 
// --------------------------------------------------------------------------------
static JSBool
ds_Show(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxWindow 	*uwindow;
		
	uwindow = (DxWindow*)ds_Window_GetPrivate(cx, obj, argv);
	if (! uwindow)
		return JS_FALSE;
		
	//uwindow->Show();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
static JSBool
ds_Hide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxWindow 	*uwindow;
		
	uwindow = (DxWindow*)ds_Window_GetPrivate(cx, obj, argv);
	if (! uwindow)
		return JS_FALSE;
		
	//uwindow->Hide();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
// method o.play( pitchNum, velocityNum )
static JSBool
ds_Select(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval * /*rval*/ )
{
    DxWindow 	*uwindow;
		
	uwindow = (DxWindow*)ds_Window_GetPrivate(cx, obj, argv);
	if (! uwindow)
		return JS_FALSE;
		
	//uwindow->ShowSelect();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
// Window( left, top, width, height )
// !!@ Need Rect here.
//
static JSBool
ds_Constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxWindow 	*uwindow;
    JSBool		ok;
	int32		left = 10, top = 20, width = 640, height = 480;
	
	/* NotePlayer called as function 
	If called with new, replace with new NotePlayer object
	*/
    if (!(cx->fp->flags & JSFRAME_CONSTRUCTING)) 
    {
		obj = js_NewObject(cx, &ds_Class, NULL, NULL);
		if (!obj)
		    return JS_FALSE;
		*rval = OBJECT_TO_JSVAL(obj);
    }
    	
    ok = JS_ConvertArguments(cx, argc, argv, "iiii", &left, &top, &width, &height);
    if (! ok)
    	goto exit;
  	
    //uwindow = new DxWindow( obj, windowClass, attributes, bounds );
    uwindow = new DxWindow( cx, obj, left, top, width, height );
    if (! uwindow)
    	return JS_FALSE;
	
    ok = JS_SetPrivate(cx, obj, uwindow);
	
exit:;
    return ok;
}

// --------------------------------------------------------------------------------
static void
ds_finalize(JSContext *cx, JSObject *obj)
{
    DxWindow 	*uwindow;

    uwindow = (DxWindow *) JS_GetPrivate(cx, obj);
    if (!uwindow)
		return;

	// !!@ Need to dissconnet window on shutdown
	delete uwindow;
}

// --------------------------------------------------------------------------------
static JSBool
ds_accessProperty(JSContext *cx, JSBool doSet, JSObject *obj, jsval id, jsval *vp)
{
    JSBool 			ok = JS_FALSE;
    jsint 			slot;
	int32			num;
	//Str255			title;
    DxWindow 		*ucontrol;

	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
		
    JS_LOCK_OBJ(cx, obj);
    
    ucontrol = (DxWindow *) JS_GetPrivate(cx, obj);
	if (! ucontrol)
		goto exit;
		
    switch (slot)
    {
    	case DS_VISIBLE_PROP:
    	{
    		if (doSet)
    		{
	    		if (!JS_ValueToInt32(cx, *vp, &num))
		    		goto exit;
	    		//ucontrol->SetVisible(num != 0);
    		}
    		else
    		{
    			//*vp = BOOLEAN_TO_JSVAL(ucontrol->IsVisible());
    		}
    		break;
    	}
    	case DS_ENABLED_PROP:
    	{
    		if (doSet)
    		{
	    		if (!JS_ValueToInt32(cx, *vp, &num))
		    		goto exit;
	    		//ucontrol->SetEnabled(num != 0);
    		}
    		else
    		{
    			//*vp = BOOLEAN_TO_JSVAL(ucontrol->IsEnabled());
    		}
    		break;
    	}
    	case DS_TITLE_PROP:
    	{
    		if (doSet)
    		{
	    		//ds_ConvertJSValueToStringPtr( cx, *vp, title );
	    		//ucontrol->SetDescriptor( title );
    		}
    		else
    		{
    			//ucontrol->GetDescriptor(title);
    			//ds_ConvertStringPtrToJSValue(cx, title, vp);
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
ds_Window_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
