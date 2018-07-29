// --------------------------------------------------------------------------------
//
// Basic Color object.
//
// 2005.03.01 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxColor.h"

#include "dicejs.h"

// --------------------------------------------------------------------------------
DxColor::DxColor(void *wrapperObj, int red, int green, int blue  )
	: DxGeneric(wrapperObj )
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

// --------------------------------------------------------------------------------
void DxColor::Process( )
{
	
}

// --------------------------------------------------------------------------------
JSBool DxColor::GetProp( DxPropID propId, DxPropType propType, void **outResult )
{
	switch (propId)
	{
		case kDxProp_value:
		{
			switch (propType)
			{
				case kDxType_DxColor:
					if (outResult)
						*outResult = this;
					return JS_TRUE;
			}
			break;
		}
	}
	if (outResult)
		*outResult = 0;
	return JS_FALSE;
}

// --------------------------------------------------------------------------------
JSBool DxColor::SetProp( DxPropID propId, DxPropType propType, void *newValue )
{
	return JS_FALSE;
}



// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
