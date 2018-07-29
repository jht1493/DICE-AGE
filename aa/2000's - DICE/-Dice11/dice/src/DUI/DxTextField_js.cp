// --------------------------------------------------------------------------------
//
// Bridge to TextFild Control.
//
// 2005.02.18 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxTextField_js.h"
#include "DxTextField.h"
#include "DxWindow_js.h"
#include "DxWindow.h"
#include "dsDOM.h"

#include "jscntxt.h"
#include "jslock.h"

#define DS_CLASS_NAME_UI "TextField"

// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static void
ds_finalize(JSContext *cx, JSObject *obj);

static JSBool
ds_Show(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_Hide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"show",            ds_Show,            0,0,0},
    {"hide",            ds_Hide,            0,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_VISIBLE_PROP,
    DS_ENABLED_PROP,
    DS_TITLE_PROP,
    DS_EDITABLE_PROP,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"visible",		DS_VISIBLE_PROP,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"enabled",		DS_ENABLED_PROP,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"title",		DS_TITLE_PROP,			JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"editable",	DS_EDITABLE_PROP,			JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    //{"npoly",     	DS_NPOLY_PROP,			JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
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
static DxTextField *
ds_Caption_GetPrivate(JSContext *cx, JSObject *obj, jsval *argv)
{
	return (DxTextField*) JS_GetInstancePrivate(cx, obj, &ds_Class, argv );
}

// --------------------------------------------------------------------------------
static JSBool
ds_Show(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxTextField 	*ucontrol;

	ucontrol =  ds_Caption_GetPrivate(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
	//ucontrol->Show();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
static JSBool
ds_Hide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxTextField 	*ucontrol;

	ucontrol =  ds_Caption_GetPrivate(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
	//ucontrol->Hide();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
// PushButton( "Title", left, top, width, height )
//
static JSBool
ds_Constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxTextField 			*ucontrol;
    JSBool 				ok;
	//SPaneInfo			paneInfo;
	//MessageT			valueMessage = 1000;
	//Str255				title;
	//char				*titleStr;
	int32				left = 10, top = 20, width = 80, height = 20;
	JSObject			*windObj = 0;
	DxWindow*			uwindow = 0;
	//ResIDT				textTraitsID = 1;
	
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
    
    ok = JS_ConvertArguments(cx, argc, argv, "o/iiii", &windObj, &left, &top, &width, &height);
    if (! ok)
    	goto exit;

	uwindow = (DxWindow*)ds_Window_GetPrivate(cx, windObj, 0);
	ok = uwindow != NULL;
	if (! ok)
		goto exit;
	
    ucontrol = new DxTextField( obj, uwindow, left, top, width, height );

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
    DxTextField 	*ucontrol;

    ucontrol = (DxTextField *) JS_GetPrivate(cx, obj);
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
	//Str255			title;
    DxTextField 	*ucontrol;

	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
		
    JS_LOCK_OBJ(cx, obj);
    
	ucontrol = (DxTextField*) JS_GetPrivate(cx, obj);
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
		case DS_EDITABLE_PROP:
		{
    		if (doSet)
    		{
	    		if (!JS_ValueToInt32(cx, *vp, &num))
		    		goto exit;
	    		ucontrol->SetEditable(num != 0);
    		}
    		else
    		{
    			*vp = BOOLEAN_TO_JSVAL(ucontrol->GetEditable());
    		}
    		break;
		}
    	case DS_TITLE_PROP:
    	{
			jschar *chars;
			size_t length;
			
    		if (doSet)
    		{
	    		ok = ds_ConvertJSValueToStringUC( cx, *vp, &chars, &length );
				if (! ok) {
					// !!@ string expected
				}
				ucontrol->SetTextValue( chars, length );
    		}
    		else
    		{
    			ucontrol->GetTextValue(&chars, &length);
    			ds_ConvertUCStringToJSValue(cx, chars, length, vp);
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
JSObject *
ds_TextField_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
