// --------------------------------------------------------------------------------
//
// Basic Color object.
//
// 2005.03.01 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "DxGeneric.h"

//#include <QTML.h>
//#include <QuickTimeComponents.h>


// --------------------------------------------------------------------------------
class DxColor : public DxGeneric
{
public:
	DxColor(void *wrapperObj, int red, int green, int blue  );
	
		
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;

	// --------------------------------------------------------------------------------
	
	int		red;
	int		green;
	int		blue;
	
	// --------------------------------------------------------------------------------
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
