// --------------------------------------------------------------------------------
//
// Bridge to Caption control.
//
// 2005.02.18 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxCaption_js.h"
#include "DxCaption.h"
#include "DxWindow_js.h"
#include "DxWindow.h"
#include "dsDOM.h"

#include "jscntxt.h"
#include "jslock.h"
//#include <TextUtils.h>

#define DS_CLASS_NAME_UI "Caption"

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
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"visible",		DS_VISIBLE_PROP,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"enabled",		DS_ENABLED_PROP,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"title",		DS_TITLE_PROP,			JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
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
static DxCaption *
ds_Caption_GetPrivate(JSContext *cx, JSObject *obj, jsval *argv)
{
	return (DxCaption*) JS_GetInstancePrivate(cx, obj, &ds_Class, argv );
}

// --------------------------------------------------------------------------------
static JSBool
ds_Show(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxCaption 	*ucaption;

	ucaption =  ds_Caption_GetPrivate(cx, obj, argv);
	if (! ucaption)
		return JS_FALSE;
		
	//ucaption->Show();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
static JSBool
ds_Hide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxCaption 	*ucaption;

	ucaption =  ds_Caption_GetPrivate(cx, obj, argv);
	if (! ucaption)
		return JS_FALSE;
		
	//ucaption->Hide();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
// PushButton( "Title", left, top, width, height )
//
static JSBool
ds_Constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxCaption 			*ucaption;
    JSBool 				ok;
	//SPaneInfo			paneInfo;
	//MessageT			valueMessage = 1000;
	//Str255				title;
	char				*titleStr;
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
    
    ok = JS_ConvertArguments(cx, argc, argv, "o/siiii", &windObj, &titleStr, &left, &top, &width, &height);
    if (! ok)
    	goto exit;

	uwindow = (DxWindow*)ds_Window_GetPrivate(cx, windObj, 0);
	ok = uwindow != NULL;
	if (! ok)
		goto exit;
		
#if 0
  	paneInfo.paneID = 1001; /* !!@ from DUWindow */
  	paneInfo.width = (SInt16) width;
  	paneInfo.height = (SInt16) height;
  	//paneInfo.visible = TRUE; 
  	//paneInfo.enabled = TRUE; // starting out TRUE does not seem to work. can't enable latter
  	paneInfo.visible = TRUE;
  	paneInfo.enabled = FALSE;
  	paneInfo.bindings.left = FALSE;
  	paneInfo.bindings.top = FALSE;
  	paneInfo.bindings.right = FALSE;
  	paneInfo.bindings.bottom = FALSE;
  	paneInfo.left = left;
  	paneInfo.top = top;
  	paneInfo.userCon = (SInt32) obj;
  	paneInfo.superView = uwindow;
  	
  	CopyCStringToPascal(titleStr, title);
  	
    ucaption = new DxCaption( obj, paneInfo, title, textTraitsID );
    if (! ucaption)
    {
    	ok = JS_FALSE;
    	goto exit;
    }
#endif

    ok = JS_SetPrivate(cx, obj, ucaption);
 
 exit:;
    return ok;
}

// --------------------------------------------------------------------------------
static void
ds_finalize(JSContext *cx, JSObject *obj)
{
    DxCaption 	*ucaption;

    ucaption = (DxCaption *) JS_GetPrivate(cx, obj);
    if (!ucaption)
		return;

	delete ucaption;
}

// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JSBool 			ok = JS_FALSE;
    jsint 			slot;
    DxCaption 		*ucaption;
	//Str255			title;

	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
		
    JS_LOCK_OBJ(cx, obj);
    
	ucaption = (DxCaption*) JS_GetPrivate(cx, obj);
	if (! ucaption)
		goto exit;
		
    switch (slot)
    {
    	case DS_VISIBLE_PROP:
    		//*vp = BOOLEAN_TO_JSVAL(ucaption->IsVisible());
    		break;
    	case DS_ENABLED_PROP:
    		//*vp = BOOLEAN_TO_JSVAL(ucaption->IsEnabled());
    		break;
    	case DS_TITLE_PROP:
    	{
    		//ucaption->GetDescriptor(title);
    		//ds_ConvertStringPtrToJSValue(cx, title, vp);
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
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JSBool 			ok = JS_FALSE;
    jsint 			slot;
    DxCaption 		*ucaption;
	int32			num;
	//Str255				title;
	
	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
    
    JS_LOCK_OBJ(cx, obj);

	ucaption = (DxCaption*) JS_GetPrivate(cx, obj);
	if (! ucaption)
		goto exit;

    switch (slot)
    {
    	case DS_VISIBLE_PROP:
    		if (!JS_ValueToInt32(cx, *vp, &num))
	    		goto exit;
    		//ucaption->SetVisible(num != 0);
    		break;
    	case DS_ENABLED_PROP:
    		if (!JS_ValueToInt32(cx, *vp, &num))
	    		goto exit;
    		//ucaption->SetEnabled(num != 0);
    		break;
    	case DS_TITLE_PROP:
    	{
    		//ds_ConvertJSValueToStringPtr( cx, *vp, title );
    		//ucaption->SetDescriptor( title );
    		break;
    	}
    }
    ok = JS_TRUE;
    
exit:;
    JS_UNLOCK_OBJ(cx, obj);
    return ok;
}

// --------------------------------------------------------------------------------
JSObject *
ds_Caption_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
