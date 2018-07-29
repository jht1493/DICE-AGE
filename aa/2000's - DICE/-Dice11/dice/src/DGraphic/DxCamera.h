// --------------------------------------------------------------------------------
//
// Video Source Input
//
// 2005.02.21 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "DxGeneric.h"

#include <QuickTime/QuickTime.h>

// --------------------------------------------------------------------------------
class DxCamera : public DxGeneric
{
public:
	DxCamera(void *wrapperObj);
	
	~DxCamera();
			
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;
	
	
	// --------------------------------------------------------------------------------

	void 	VideoSettings();	// Needs windowPointer
		
	// --------------------------------------------------------------------------------
	void	SetGWorldPtr( GWorldPtr gworldPtr);
	void	SetBounds( Rect bounds);
	void 	CreateSeqGrab(int devNum, Rect bounds, GWorldPtr monitor);
	int 	GetInputDeviceNum();
	void 	SetInputDeviceNum( int aDevNum);
	void 	SetInputDeviceNumOnly( int aDevNum);
	void	SpendTime();
	void	AddRef();
	void	ReleaseRef();
	
	void 	Close();

	// --------------------------------------------------------------------------------

	GWorldPtr				gworldPtr;
	Rect					bounds;
	int						inputDeviceNum;

	SeqGrabComponent		seqGrabber;
	SGChannel				videoChannel;
	int						grabStarted;
	int						refCount;
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
