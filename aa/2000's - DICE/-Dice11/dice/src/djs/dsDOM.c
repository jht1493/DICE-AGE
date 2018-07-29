/*
Dice General DOM functions.
Keeping it simply and mapping directly to Macintosh/QuickTime API

Copyright (c) 2005 John Henry Thompson. All rights reserved.

History:

2006-08-27	jht	Ported to xcode.
2005.02.14	jht	Created.

*/ 

#include "dsDOM.h"

#include "jscntxt.h"
#include "jslock.h"

#include "DxColor_js.h"
#include "DxCamera_js.h"
#include "DxImage_js.h"
#include "DxProjector_js.h"
#include "DxDiffEffect_js.h"
#include "DxNotePlayer_js.h"
#include "DxMovie_js.h"
#include "DxMovieMaker_js.h"
#include "DxDelayEffect_js.h"
#include "DxConvolveEffect_js.h"
#include "DxWindow_js.h"
#include "DxPushButton_js.h"
#include "DxCaption_js.h"
#include "DxTextField_js.h"

#import <Carbon/Carbon.h>
#import <CoreFoundation/CoreFoundation.h>
#import <QuickTime/QuickTime.h>
//#include <CFBundle.h>

#include "MyDocument_c.h"

// --------------------------------------------------------------------------------
static JSBool
dsGetDocPath(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	void *docInst;
	jschar *chars;
	size_t length;
	
	docInst = JS_GetContextPrivate( cx );
	MyDocument_GetDocPath(docInst, &chars, &length);
	ds_ConvertUCStringToJSValue(cx, chars, length, rval);
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
static JSBool
dsGetAppPath(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	OSErr err = fnfErr;
	CFBundleRef myAppsBundle = CFBundleGetMainBundle();
	//FSCatalogInfo	catInfo;
	FSRef			parentRef;
	//FSCatalogInfo	catInfo;
	UInt8			path[kHFSPlusMaxFileNameChars+1];

    JSString *str;

	if (myAppsBundle == NULL) return err;
	
	CFURLRef myBundleURL = CFBundleCopyExecutableURL(myAppsBundle);
	
	if (myBundleURL == NULL) return err;


	FSRef myBundleRef;
	Boolean ok = CFURLGetFSRef(myBundleURL, &myBundleRef);
	CFRelease(myBundleURL);
	if (!ok) return err;

	err = FSGetCatalogInfo(&myBundleRef, kFSCatInfoNone, NULL, NULL, NULL, &parentRef);
	if (err)
		return false;
	#if 1
	// Go up 3 levels
	int i;
	for (i = 0; i < 3; i++) 
	{
		myBundleRef = parentRef;
		err = FSGetCatalogInfo(&myBundleRef, kFSCatInfoNone, NULL, NULL, NULL, &parentRef);
		if (err)
			return false;
	}
	#endif
	
	// !!@ Unicode - how?
	// Add trailing "/"
	err = FSRefMakePath(&parentRef, &path[0], sizeof(path)-1 );
	if (err)
		return false;
	int len = strlen(path);
	path[ len ] = '/';
	path[ len+1 ] = 0;
	
    str = JS_NewStringCopyZ(cx, (char *)&path[0]);
    if (!str)
		return JS_FALSE;
    *rval = STRING_TO_JSVAL(str);
    
    return JS_TRUE;
}

// --------------------------------------------------------------------------------
static JSBool
dsGetCurrentEventTime(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	return JS_NewNumberValue(cx, GetCurrentEventTime(), rval);
}

// --------------------------------------------------------------------------------
static JSBool
dsMicroseconds(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	UnsignedWide	usec;
#if 0
	struct UnsignedWide {   // big-endian version
	   UInt32    hi;
	   UInt32    lo;
	   };
#endif

	Microseconds( &usec );
	
	return JS_NewNumberValue(cx, (double)usec.lo, rval);

}

// --------------------------------------------------------------------------------
static JSBool
dsGetKeys(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	KeyMap		keyMap;
	JSObject 	*ovec = JSVAL_NULL;
	jsval		elm;
	long		num;
	
	GetKeys(keyMap);
	
	if (argc < 1)
	{
		// !!@ Need to store in gc root, best to take input as array
		ovec = JS_NewArrayObject(cx, 0, 0);
	}
	else if (JSVAL_IS_OBJECT(argv[0]))
	{
		ovec = JSVAL_TO_OBJECT(argv[0] );
		if (! JS_IsArrayObject(cx, ovec))
		{
			// !!@ Array expected.
			return JS_FALSE;
		}
	}
	if (ovec)
	{
		int i ;
		for (i = 0; i < 4; i++)
		{
			//num = (long)( CFSwapInt32BigToHost( keyMap[i] ) );
			num = (long)( CFSwapInt32BigToHost( *(long*) &keyMap[i] ) );
			if (INT_FITS_IN_JSVAL(num))
			{
				elm = INT_TO_JSVAL( num );
			}
			else
			{
				elm = DOUBLE_TO_JSVAL(JS_NewDouble(cx, num ));
			}
			JS_SetElement(cx, ovec, i, &elm);
		}
	}
	*rval = OBJECT_TO_JSVAL(ovec);
    return JS_TRUE;
}

// --------------------------------------------------------------------------------
static JSFunctionSpec dom_functions[] = 
{
	{"DocPath",					dsGetDocPath,	0},
	{"AppPath",					dsGetAppPath,	0},
    {"GetKeys",         		dsGetKeys,        0},
    {"Microseconds",			dsMicroseconds,        0},
    {"GetCurrentEventTime",		dsGetCurrentEventTime,        0},
    {0}
};

// --------------------------------------------------------------------------------
JSBool ds_ConvertUCStringToJSValue( JSContext *cx, jschar *chars, size_t length, jsval *rval)
{
	JSString 		*jstr ;

	jstr = JS_NewUCStringCopyN(cx, chars, length);
	if ( jstr)
	{
		*rval = STRING_TO_JSVAL(jstr);
		return JS_TRUE;
	}
	else
	{
		*rval = JSVAL_NULL;
		return JS_FALSE;
	}
}

// --------------------------------------------------------------------------------
JSBool ds_ConvertJSValueToStringUC( JSContext *cx, jsval jval, jschar **chars, size_t *length)
{
	JSString*	pjstr;
	
	if (JSVAL_IS_STRING(jval))
	{
		pjstr = JS_ValueToString(cx, jval);
		if (pjstr)
		{
			*chars = JS_GetStringChars(pjstr);
			*length = JS_GetStringLength(pjstr);
			return (*chars != 0);
		}
	}
	return JS_FALSE;
}

#if 0
// --------------------------------------------------------------------------------
JSBool ds_ConvertStringPtrToJSValue( JSContext *cx, StringPtr strp, jsval *rval)
{
	JSString 		*jstr ;

	jstr = JS_NewStringCopyN(cx, (char*)&strp[1], strp[0]);
	if ( jstr)
	{
		*rval = STRING_TO_JSVAL(jstr);
		return JS_TRUE;
	}
	else
	{
		*rval = JSVAL_NULL;
		return JS_FALSE;
	}
}

// --------------------------------------------------------------------------------
JSBool ds_ConvertJSValueToStringPtr( JSContext *cx, jsval jval, StringPtr strp)
{
	JSString*	pjstr;
	char		*ptr;
	
	if (JSVAL_IS_STRING(jval))
	{
		pjstr = JS_ValueToString(cx, jval);
		if (pjstr)
		{
			ptr = JS_GetStringBytes(pjstr);
			if (ptr)
			{
  				CopyCStringToPascal(ptr, strp);
  				return JS_TRUE;
			}
		}
	}
	return JS_FALSE;
}
#endif

// --------------------------------------------------------------------------------
JSObject *
ds_InitDOM(JSContext *cx, JSObject *obj)
{
    if (!JS_DefineFunctions(cx, obj, dom_functions))
    {
    	// !!@ function def failure
    }
	ds_Color_InitClass(cx, obj);
	ds_Camera_InitClass(cx, obj);
	ds_Image_InitClass(cx, obj);
	ds_Projector_InitClass(cx, obj);	
	ds_DiffEffect_InitClass(cx, obj);
	ds_DiffGrayEffect_InitClass(cx, obj);
	ds_DiffTrailEffect_InitClass(cx, obj);
	ds_DiffNoiseEffect_InitClass(cx, obj);
	ds_DiffStreakEffect_InitClass(cx, obj);
	ds_DiffTrailColorEffect_InitClass(cx, obj);
	ds_DiffStreakTrailColorEffect_InitClass(cx, obj);
	ds_DelayEffect_InitClass(cx, obj);
	ds_ConvolveEffect_InitClass(cx, obj );
 	ds_NotePlayer_InitClass(cx, obj);
	ds_Movie_InitClass(cx, obj);
	ds_MovieMaker_InitClass(cx, obj);
 	ds_Window_InitClass(cx, obj); 
 	ds_PushButton_InitClass(cx, obj);
 	ds_Caption_InitClass(cx, obj);
 	ds_TextField_InitClass(cx, obj);

	// !!@ Do we need ExitMovies??
	EnterMovies();
	
    return 0;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
