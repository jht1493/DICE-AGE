// --------------------------------------------------------------------------------
//
// DiffEffect - Image difference effect
//
// 2005.02.26 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "DxImage.h"

#include "jstypes.h"


// --------------------------------------------------------------------------------
class DxDiffEffect : public DxGeneric
{
public:
	DxDiffEffect(void *wrapperObj);
	
	~DxDiffEffect();
			
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;
	
	
	// --------------------------------------------------------------------------------
		
	virtual void 	Close();

	// --------------------------------------------------------------------------------
	
	virtual void 		SetInput(DxImage	*newInput);
	virtual void 		SetOutput(DxImage	*output);
	
	virtual JSBool 	PrepareBuffers();
	
	virtual void		ComputeImage();

	DxImage		*input;
	DxImage		*output;
	DxImage		*previous;				/* Previous image */
	Rect		bounds;
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
