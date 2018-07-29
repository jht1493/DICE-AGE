// --------------------------------------------------------------------------------
//
// ConvolveEffect - average neighbors of a pixel
//
// 2006.08.06	jht		Mod for convolve. The beat goes on.
// 2005.02.22 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxConvolveEffect_js.h"
#include "DxConvolveEffect.h"

#include "DxImage_js.h"
#include "DxImage.h"

#include "dicejs.h"

// --------------------------------------------------------------------------------
DxConvolveEffect::DxConvolveEffect(void *wrapperObj )
	: DxGeneric(wrapperObj )
{
	this->scaleFactor = 9;
}

// --------------------------------------------------------------------------------
DxConvolveEffect::~DxConvolveEffect()
{
	Close();
}

// ---------------------------------------------------------------------------------
void DxConvolveEffect::SetInput(DxImage *newInput)
{
	Close();
	
	this->input = newInput;
	if (! this->input)
		return;
	
	this->input->GetPixBaseInfo( 0/*basePtr*/, 0/*rowBytes*/, &this->bounds);
}

// ---------------------------------------------------------------------------------
void DxConvolveEffect::SetOutput(DxImage *newOutput)
{
	this->output = newOutput;
	if (! this->output)
		return;
	
	this->output->GetPixBaseInfo( 0/*basePtr*/, 0/*rowBytes*/, &this->destRect);
}

// ---------------------------------------------------------------------------------
JSBool DxConvolveEffect::PrepareBuffers()
{
	if (! this->input)
		return JS_FALSE;
	
	if (! this->output)
		return JS_FALSE;
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
void DxConvolveEffect::Process( )
{
	if (! PrepareBuffers())
		return;
	
	ComputeConvolution();
}

// ---------------------------------------------------------------------------------
void	DxConvolveEffect::ComputeConvolution()
{
	this->ComputeConvolution32();
}

// ---------------------------------------------------------------------------------
void	DxConvolveEffect::ComputeConvolution32()
{
	int					hcount, vcount, width, height;
	Rect				box;
	Rect				destBox;
	
	UInt8*				sourcePtr; 
	UInt8*				destPtr; 
	
	int					rowBytes;
	int					rowBytesDest;
	
	UInt8*				sourceLinePtr;
	UInt8*				destLinePtr;
	UInt8*				sourceLine2Ptr;
	
	int					sum0, sum1, sum2;
	//int					covSumScale = this->scaleFactor;
		
	this->input->GetPixBaseInfo( &sourcePtr, &rowBytes, &box);
	this->output->GetPixBaseInfo( &destPtr, &rowBytesDest, &destBox);

	width = box.right - box.left;
	height = box.bottom - box.top;

	// Inset by 1 pixel
	height -= 2;
	width -= 2;
	
	sourcePtr += rowBytes;
	destPtr += rowBytesDest;
	
	destPtr += 4;
	sourcePtr += 4;
	
	for (vcount = height; 
		vcount > 0; 
		vcount--, sourcePtr += rowBytes, destPtr += rowBytesDest)
	{
		sourceLinePtr = sourcePtr+1;
		
		destLinePtr = destPtr+1;
				
		for (hcount =  width; 
			hcount > 0; 
			hcount--, sourceLinePtr+=4, destLinePtr += 4)
		{
		
			// Compute 3x3 avg
			
			// Previous Line
			sourceLine2Ptr = sourceLinePtr - rowBytesDest;
			
			sum0 = sourceLine2Ptr[0-4];
			sum1 = sourceLine2Ptr[1-4];
			sum2 = sourceLine2Ptr[2-4];
			
			sum0 += sourceLine2Ptr[0];
			sum1 += sourceLine2Ptr[1];
			sum2 += sourceLine2Ptr[2];
			
			sum0 += sourceLine2Ptr[0+4];
			sum1 += sourceLine2Ptr[1+4];
			sum2 += sourceLine2Ptr[2+4];
			
			// Next Line
			sourceLine2Ptr = sourceLinePtr + rowBytesDest;
			
			sum0 += sourceLine2Ptr[0-4];
			sum1 += sourceLine2Ptr[1-4];
			sum2 += sourceLine2Ptr[2-4];
			
			sum0 += sourceLine2Ptr[0];
			sum1 += sourceLine2Ptr[1];
			sum2 += sourceLine2Ptr[2];
			
			sum0 += sourceLine2Ptr[0+4];
			sum1 += sourceLine2Ptr[1+4];
			sum2 += sourceLine2Ptr[2+4];

			// Current Line
			sum0 += sourceLinePtr[0-4];
			sum1 += sourceLinePtr[1-4];
			sum2 += sourceLinePtr[2-4];
			
			sum0 += sourceLinePtr[0];
			sum1 += sourceLinePtr[1];
			sum2 += sourceLinePtr[2];
			
			sum0 += sourceLinePtr[0+4];
			sum1 += sourceLinePtr[1+4];
			sum2 += sourceLinePtr[2+4];

			// Replace current pixel with average of all 9 in 3x3 matrix
			destLinePtr[0] = sum0 / scaleFactor; 
			destLinePtr[1] = sum1 / scaleFactor; 
			destLinePtr[2] = sum2 / scaleFactor; 

		}
	}
}
// ---------------------------------------------------------------------------------
void	DxConvolveEffect::ComputeConvolution8()
{
}

// --------------------------------------------------------------------------------
JSBool DxConvolveEffect::GetProp( DxPropID propId, DxPropType propType, void **outResult )
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
JSBool DxConvolveEffect::SetProp( DxPropID propId, DxPropType propType, void *outResult )
{
	return JS_FALSE;
}

// ---------------------------------------------------------------------------------
void DxConvolveEffect::Close()
{
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
