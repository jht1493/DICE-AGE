/* 

JavaScript wrapper for note player

Copyright (c) 2003 John Henry Thompson. All rights reserved.

History:

2003.03.20	jht	Created. Integrating JavaScript.

*/

#include "DxNotePlayer_js.h"
#include "DxNotePlayer.h"

#include "jscntxt.h"
#include "jslock.h"

#define DS_CLASS_NAME "NotePlayer"

// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static void
ds_finalize(JSContext *cx, JSObject *obj);

static JSBool
ds_play(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_Task(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_getController(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_setController(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"play",			ds_play,            	2,0,0},
    {"getController",	ds_getController,       2,0,0},
    {"setController",	ds_setController,		2,0,0},
    {"Task",			ds_Task,				2,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_PROP_instrument,
    DS_PROP_npoly,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"instrument",	DS_PROP_instrument,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"npoly",     	DS_PROP_npoly,			JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {0,0,0,0,0}
};

static const char ds_NotePlayer_str[]           = "NotePlayer";

// --------------------------------------------------------------------------------
static JSClass ds_Class = {
    DS_CLASS_NAME,
    JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,  JS_PropertyStub,  ds_getProperty, 	ds_setProperty,
    JS_EnumerateStub, JS_ResolveStub,   JS_ConvertStub,     ds_finalize,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

// --------------------------------------------------------------------------------
static void *
ds_DxNotePlayer_GetPrivate(JSContext *cx, JSObject *obj, jsval *argv)
{
	return  JS_GetInstancePrivate(cx, obj, &ds_Class, argv );
}

// --------------------------------------------------------------------------------
// method o.Task(  )
static JSBool
ds_Task(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval * /*rval*/ )
{
    DxNotePlayer 	*ucontrol;

	ucontrol =  (DxNotePlayer*)ds_DxNotePlayer_GetPrivate(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
	ucontrol->Task();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
// method o.setController( controllerNum, value ) 
//
static JSBool
ds_setController(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval * /*rval*/ )
{
    DxNotePlayer 	*ucontrol;
	int32			controllerNum = 1, newValue = 0x010;
    JSBool ok = JS_FALSE;

	ucontrol =  (DxNotePlayer*)ds_DxNotePlayer_GetPrivate(cx, obj, argv);
	if (! ucontrol)
    	goto exit;
	
    ok = JS_ConvertArguments(cx, argc, argv, "/ii", &controllerNum, &newValue);
    if (! ok)
    	goto exit;
		
	ucontrol->SetController(controllerNum, newValue);

exit:;
	return ok;
}

// --------------------------------------------------------------------------------
// method o.getController( controllerNum  ) 
//
static JSBool
ds_getController(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval * rval)
{
    DxNotePlayer 	*ucontrol;
	int32			controllerNum = 1, newValue = 0x010;
    JSBool ok = JS_FALSE;

	ucontrol =  (DxNotePlayer*)ds_DxNotePlayer_GetPrivate(cx, obj, argv);
	if (! ucontrol)
    	goto exit;
	
    ok = JS_ConvertArguments(cx, argc, argv, "/i", &controllerNum );
    if (! ok)
    	goto exit;
		
	ucontrol->GetController(controllerNum);
	
	ok = JS_NewNumberValue(cx, ucontrol->GetController(controllerNum), rval);

exit:;
	return ok;
}

// --------------------------------------------------------------------------------
// method o.play( pitchNum, velocityNum )
static JSBool
ds_play(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval * /*rval*/ )
{
    DxNotePlayer *notePlayer;
	int32		pitchNum = 70;
	int32		velocityNum = 127;
	JSBool		ok = JS_FALSE;
	
	notePlayer =  (DxNotePlayer*)ds_DxNotePlayer_GetPrivate(cx, obj, argv);

    if (!notePlayer)
		return JS_FALSE;
		
    ok = JS_ConvertArguments(cx, argc, argv, "ii", &pitchNum, &velocityNum);
    if (! ok)
    	goto exit;
		
	notePlayer->Play( pitchNum, velocityNum );

exit:;
	return ok;
}

// --------------------------------------------------------------------------------
static JSBool
NotePlayer_constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DxNotePlayer *notePlayer;
    JSBool 		ok = JS_FALSE;
	int32		instrumentNum = 1;
	int32		polyphonyNum = 3;
	
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
    
    ok = JS_ConvertArguments(cx, argc, argv, "/ii", &instrumentNum, &polyphonyNum);
    if (! ok)
    	goto exit;
    
    notePlayer = new DxNotePlayer( instrumentNum, polyphonyNum, (double)polyphonyNum );
    if (! notePlayer)
    	return JS_FALSE;
    
    ok = JS_SetPrivate(cx, obj, notePlayer);
   
exit:;
    return ok;
}

// --------------------------------------------------------------------------------
static void
ds_finalize(JSContext *cx, JSObject *obj)
{
    DxNotePlayer *notePlayer;

    notePlayer = (DxNotePlayer *) JS_GetPrivate(cx, obj);
    if (!notePlayer)
		return;
		
   delete notePlayer;
}

// --------------------------------------------------------------------------------
static JSBool
ds_accessProperty(JSContext *cx, JSBool doSet, JSObject *obj, jsval id, jsval *vp)
{
    JSBool ok = JS_FALSE;
    jsint slot;
    DxNotePlayer *notePlayer;
	int32	num;

	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
		
    JS_LOCK_OBJ(cx, obj);
    
	notePlayer = (DxNotePlayer*) JS_GetPrivate(cx, obj);
	if (! notePlayer)
		goto exit;
		
    switch (slot)
    {
    	case DS_PROP_instrument:
    		if (doSet)
    		{
				
	    		if (!JS_ValueToInt32(cx, *vp, &num))
		    		goto exit;
	    		notePlayer->SetInstrument(num);
    		}
    		else
    		{
    			*vp = INT_TO_JSVAL(notePlayer->GetInstrument());
    		}
    		break;
    	case DS_PROP_npoly:
    		if (doSet)
    		{
	    		if (!JS_ValueToInt32(cx, *vp, &num))
		    		goto exit;
	    		notePlayer->SetNPoly(num);
    		}
    		else
    		{
    			*vp = INT_TO_JSVAL(notePlayer->GetNPoly());
    		}
    		break;
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
ds_NotePlayer_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, NotePlayer_constructor, 2,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
