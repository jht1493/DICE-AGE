// --------------------------------------------------------------------------------
//
// Projector - send input to output (typically a Window)
//
// 2005.02.22 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxProjector_js.h"
#include "DxProjector.h"

#include "DxImage_js.h"
#include "DxImage.h"

#include "dicejs.h"

// --------------------------------------------------------------------------------
DxProjector::DxProjector(void *wrapperObj )
	: DxGeneric(wrapperObj )
{
	
}

// --------------------------------------------------------------------------------
DxProjector::~DxProjector()
{
	Close();
}

// --------------------------------------------------------------------------------
void DxProjector::Process( )
{
	if ( !(this->input && this->output))
		return;
	
	Rect			sourceRect;
	Rect			destRect;

	this->input->GetBounds( &sourceRect );
	this->output->GetBounds( &destRect );
	this->output->CopyPixels( this->input, &sourceRect, &destRect, this->inkMode, 0);
	//this->output->CopyPixels( this->input, &sourceRect, &sourceRect, this->inkMode, 0);
}

// --------------------------------------------------------------------------------
JSBool DxProjector::GetProp( DxPropID propId, DxPropType propType, void **outResult )
{
	return JS_FALSE;
}

// --------------------------------------------------------------------------------
JSBool DxProjector::SetProp( DxPropID propId, DxPropType propType, void *outResult )
{
	return JS_FALSE;
}

// ---------------------------------------------------------------------------------
void DxProjector::Close()
{
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
