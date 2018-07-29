// --------------------------------------------------------------------------------
//
// DelayEffect - average up a frame, creates a "Be Still" Effect
//
// 2005.02.22 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "DxImage.h"

#include <QuickTime/QuickTime.h>

#define SCALE_FACTOR (2)
#if 0
#define AVG_TYPE	int
#else
#define AVG_TYPE	float
#endif

// --------------------------------------------------------------------------------
class DxDelayEffect : public DxGeneric
{
public:
	DxDelayEffect(void *wrapperObj);
	
	~DxDelayEffect();
			
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;
	
	
	// --------------------------------------------------------------------------------
		
	void 	Close();

	// --------------------------------------------------------------------------------
	AVG_TYPE	GetDelay();
	void		SetDelay(AVG_TYPE newDelay);
	
	void 		SetInput(DxImage	*newInput);
	void 		SetOutput(DxImage	*output);
	
	JSBool 	PrepareBuffers();
	
	void		ComputeAverage();

	DxImage		*input;
	DxImage		*output;

	Rect		bounds;
	Rect		destRect;
	DxImage		*imgTemp;				/* Frame for averged pixels */
	AVG_TYPE	*avgBufferPtr;
	AVG_TYPE	stillScale;
	int			inkMode; // srcCopy = 0
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------