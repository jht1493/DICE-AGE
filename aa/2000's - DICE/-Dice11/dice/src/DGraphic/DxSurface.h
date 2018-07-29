// --------------------------------------------------------------------------------
//
// Abstract Drawing surface, to accomdate windows & gworlds
//
// 2005.02.24 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "DxGeneric.h"

#include <QuickTime/QuickTime.h>


// --------------------------------------------------------------------------------
class DxSurface : public DxGeneric
{
public:
	DxSurface( );
	
	DxSurface(void *wrapperObj  );
	
	~DxSurface();
	
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;

	// --------------------------------------------------------------------------------

	virtual void GetBounds( Rect *outRect);
	virtual void CopyPixels( DxSurface *input, Rect *sourceRect, Rect *destRect, int inkMode, RgnHandle maskRgn);
	
	// --------------------------------------------------------------------------------

	// --------------------------------------------------------------------------------

	
	// --------------------------------------------------------------------------------
	
	GWorldPtr	gworldPtr;
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
