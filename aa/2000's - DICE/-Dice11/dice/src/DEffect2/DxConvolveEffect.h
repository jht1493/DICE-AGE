// --------------------------------------------------------------------------------
//
// ConvolveEffect - average neighbors of a pixel
//
// 2006.08.06	jht		Mod for convolve. The beat goes on.
// 2005.02.22 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "DxImage.h"

#include "DxColor.h"
#include <QuickTime/QuickTime.h>

// --------------------------------------------------------------------------------
class DxConvolveEffect : public DxGeneric
{
public:
	DxConvolveEffect(void *wrapperObj);
	
	~DxConvolveEffect();
			
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;
	
	
	// --------------------------------------------------------------------------------
		
	void 	Close();

	// --------------------------------------------------------------------------------
	
	void 		SetInput(DxImage	*newInput);
	void 		SetOutput(DxImage	*output);
	
	JSBool		PrepareBuffers();
	
	void		ComputeConvolution();
	void		ComputeConvolution8();
	void		ComputeConvolution32();
	
	DxImage		*input;
	DxImage		*output;

	Rect		bounds;
	Rect		destRect;
	
	int			scaleFactor;		// Amount to scale the 8 neighboring pixels
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
