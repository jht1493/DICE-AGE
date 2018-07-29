// --------------------------------------------------------------------------------
//
// DelayEffect -- scale input and average it with previous input
//
// 2005.02.22 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxDelayEffect_js.h"
#include "DxDelayEffect.h"

#include "DxImage_js.h"
#include "DxImage.h"

#include "dicejs.h"

// --------------------------------------------------------------------------------
DxDelayEffect::DxDelayEffect(void *wrapperObj )
	: DxGeneric(wrapperObj )
{
	this->inkMode = srcCopy;
	this->stillScale = SCALE_FACTOR;
}

// --------------------------------------------------------------------------------
DxDelayEffect::~DxDelayEffect()
{
	Close();
}

// ---------------------------------------------------------------------------------
AVG_TYPE DxDelayEffect::GetDelay()
{
	return this->stillScale;
}

// ---------------------------------------------------------------------------------
void DxDelayEffect::SetDelay(AVG_TYPE newDelay)
{
	this->stillScale = newDelay;
}

// ---------------------------------------------------------------------------------
void DxDelayEffect::SetInput(DxImage *newInput)
{
	Close();
	
	this->input = newInput;
	if (! this->input)
		return;
	
	this->input->GetPixBaseInfo( 0/*basePtr*/, 0/*rowBytes*/, &this->bounds);
}

// ---------------------------------------------------------------------------------
void DxDelayEffect::SetOutput(DxImage *newOutput)
{
	this->output = newOutput;
	if (! this->output)
		return;
	
	this->output->GetPixBaseInfo( 0/*basePtr*/, 0/*rowBytes*/, &this->destRect);
}

// ---------------------------------------------------------------------------------
JSBool DxDelayEffect::PrepareBuffers()
{
	if (! this->input)
		return JS_FALSE;
	
	if (! this->output)
		return JS_FALSE;
	
	/* Create Average gworld */
	if (! this->imgTemp)
	{		
		this->imgTemp = new DxImage( 0, 32, this->bounds, 0 );
		if (! this->imgTemp)
			return JS_FALSE;
	}
	
	if (! this->avgBufferPtr)
	{
		/* Create array of floats for averaging */
		int width, height, w, h;
		Rect	box;
		AVG_TYPE *avgBufPtr;
		
		box = this->bounds;

		width = (box.right - box.left) ;
		height = (box.bottom - box.top) ;
		
		avgBufferPtr = (AVG_TYPE *)NewPtr( sizeof(AVG_TYPE) * width * height * 3 );
		if (! avgBufferPtr)
			return FALSE;
		for ( avgBufPtr = avgBufferPtr, w = 0; w < width; w++)
		{
			for (h = 0; h < height; h++)
			{
				*avgBufPtr++ = 0;
				*avgBufPtr++ = 0;
				*avgBufPtr++ = 0;
			}
		}
		
	}
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
void DxDelayEffect::Process( )
{
	if (! PrepareBuffers())
		return;
	
	ComputeAverage();
}

// ---------------------------------------------------------------------------------
void	DxDelayEffect::ComputeAverage()
{
	//const BitMap	*pSource;
	//const BitMap	*pDest;
	AVG_TYPE 				*avgBufPtr;
	int					hcount, vcount, width, height;
	Rect				box;
	UInt8*				srcPtr;
	UInt8*				dstPtr;
	int					srcRowBytes, dstRowBytes;
	UInt8*				srcLinePtr;
	UInt8*				dstLinePtr;
	int					i;
	AVG_TYPE			stillScale = this->stillScale;
	
	this->input->GetPixBaseInfo( &srcPtr, &srcRowBytes, &box);
	this->imgTemp->GetPixBaseInfo( &dstPtr, &dstRowBytes, 0);
		
	avgBufPtr = this->avgBufferPtr;
	
	width = box.right - box.left;
	height = box.bottom - box.top;

	//dstPtr += width*4;
	dstPtr += 1;
	
	for (vcount = height; vcount > 0; vcount--, srcPtr += srcRowBytes, dstPtr += dstRowBytes)
	{
		srcLinePtr = srcPtr+1;
		dstLinePtr = dstPtr;
		for (hcount =  width; hcount > 0; hcount--, srcLinePtr+=4, dstLinePtr+=4, avgBufPtr+=3)
		{
			for (i = 0; i < 3; i++)
			{
				avgBufPtr[i] = (srcLinePtr[i] + avgBufPtr[i]*(stillScale-1))/stillScale;
				dstLinePtr[i] = avgBufPtr[i];
			}
		}
	}
	
	// !!@ Consider providing mask region
	this->output->CopyPixels( this->imgTemp, &this->bounds, &this->destRect, this->inkMode, 0);
}

// --------------------------------------------------------------------------------
JSBool DxDelayEffect::GetProp( DxPropID propId, DxPropType propType, void **outResult )
{
	switch (propId)
	{
		case kDxProp_value:
		{
			if (this->output)
				return this->output->GetProp(propId, propType, outResult );
			break;
		}
	}
	if (outResult)
		*outResult = 0;
	return JS_FALSE;
}

// --------------------------------------------------------------------------------
JSBool DxDelayEffect::SetProp( DxPropID propId, DxPropType propType, void *outResult )
{
	return JS_FALSE;
}

// ---------------------------------------------------------------------------------
void DxDelayEffect::Close()
{
	if (this->imgTemp)
	{
		delete this->imgTemp;
		this->imgTemp = 0;
	}
	if (this->avgBufferPtr)
	{
		DisposePtr( (char *)this->avgBufferPtr );
		this->avgBufferPtr = 0;
	}
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
