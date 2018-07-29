// --------------------------------------------------------------------------------
//
// Bridge to Caption control.
//
// 2005.02.18 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

//#include <LCaption.h>
#include "DxGeneric.h"

class DxCaption : public DxGeneric 
{
public:

#if 0
	DxCaption(
							void 				*wrapperObj,
							const SPaneInfo&	inPaneInfo,
							ConstStringPtr		inString,
							ResIDT				inTextTraitsID);

	DxCaption(LStream *inStream);
#endif
	DxCaption();
	virtual					~DxCaption();

	//virtual void Process();
	
	//virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	//virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult );
};

