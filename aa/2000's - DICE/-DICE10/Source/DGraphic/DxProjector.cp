// --------------------------------------------------------------------------------//// Projector - send input to output (typically a Window)//// 2005.02.22 	jht		Created.//// Copyright (c) 2005 John Henry Thompson. All rights reserved.// --------------------------------------------------------------------------------#include "dsProjector.h"#include "DxProjector.h"#include "dsImage.h"#include "DxImage.h"#include "dicejs.h"// --------------------------------------------------------------------------------DxProjector::DxProjector(void *wrapperObj )	: DxGeneric(wrapperObj ){	}// --------------------------------------------------------------------------------DxProjector::~DxProjector(){	Close();}// --------------------------------------------------------------------------------void DxProjector::Process( ){	if ( !(this->input && this->output))		return;		Rect			sourceRect;	Rect			destRect;	this->input->GetBounds( &sourceRect );	this->output->GetBounds( &destRect );	this->output->CopyPixels( this->input, &sourceRect, &destRect, this->inkMode, 0);}// --------------------------------------------------------------------------------Boolean DxProjector::GetProp( DxPropID propId, DxPropType propType, void **outResult ){	return FALSE;}// --------------------------------------------------------------------------------Boolean DxProjector::SetProp( DxPropID propId, DxPropType propType, void *outResult ){	return FALSE;}// ---------------------------------------------------------------------------------void DxProjector::Close(){}// --------------------------------------------------------------------------------// --------------------------------------------------------------------------------