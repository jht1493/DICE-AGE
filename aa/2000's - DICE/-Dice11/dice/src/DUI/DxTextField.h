// --------------------------------------------------------------------------------
//
// Bridge to TextFild Control.
//
// 2005.02.18 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "DxGeneric.h"
#include "jsapi.h"

class DxTextField : public DxGeneric 
{
public:
	void *mTextField;
	
	DxTextField(void *wrapperObj, void *uwindow, int32 left, int32 top, int32 width,int32 height);
	virtual ~DxTextField();
	
	void SetTextValue(jschar *chars, size_t length);
	void GetTextValue(jschar **chars, size_t *length);
	
	void SetEditable(JSBool flag);
	JSBool GetEditable();
	
	//virtual void Process();
	
	//virtual Boolean GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	//virtual Boolean SetProp( DxPropID propId, DxPropType propType, void *outResult );
};

