// -------------------------------------------------------------------------------- 
//
// Generic DICE cover object
//
// 2005.02.22 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxGeneric.h"
#include "dicejs.h"

// --------------------------------------------------------------------------------
DxGeneric::DxGeneric()
{
	this->safetyTag = kDxGenericTag;
}

// --------------------------------------------------------------------------------
DxGeneric::DxGeneric(void *wrapperObj)
{
	this->jsObj = wrapperObj;
	this->safetyTag = kDxGenericTag;
}

// --------------------------------------------------------------------------------
DxGeneric::~DxGeneric()
{
	if (jsObj)
	{
		dicejs_ClearWrapperObj( jsObj, (void*) this );
	}
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
