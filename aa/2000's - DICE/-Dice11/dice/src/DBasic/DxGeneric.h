// --------------------------------------------------------------------------------
//
// Generic Wrapper for any Dice value that will be placed in a JS wrapper
//
// 2005.02.21 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "jstypes.h"

// --------------------------------------------------------------------------------
typedef enum  
{
	kDxProp_value,
	kDxProp_in,
	kDxProp_out,
	kDxProp_XXX
} DxPropID;

typedef enum  
{
	kDxType_GWorldPtr,
	kDxType_CGrafPortPtr,
	kDxType_DxSurface,
	kDxType_DxImage,
	kDxType_DxColor,
	kDxType_XXX
} DxPropType;

#define kDxGenericTag	(0xC0D0E001)

// --------------------------------------------------------------------------------

class DxGeneric 
{
public:
					DxGeneric();
					DxGeneric(void *jsobj);
	virtual 		~DxGeneric();
	
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult );
	
	// ?? Do we need refcounting 
	
	int			safetyTag;	// Mark for safety
	void		*jsObj;		// Pointer to js object created to wrap us
};

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
