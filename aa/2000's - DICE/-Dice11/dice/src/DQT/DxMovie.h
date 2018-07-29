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
#include "DxImage.h"

#include <QuickTime/QuickTime.h>

// --------------------------------------------------------------------------------
class DxMovie : public DxGeneric
{
public:
	DxMovie(void *wrapperObj );
	
	~DxMovie();
			
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;
	
	
	// --------------------------------------------------------------------------------

	void 	PickFile();
	void	OpenFile(JSString *path);
	
	void	Stop();
	void	Play();
	
	long	GetCurrentTime();
	void	SetCurrentTime(long newTime);
	long	GetDuration();
	long	GetWidth();
	long	GetHeight();
	long	GetFrameCount();
	long	GetTimeScale();
	
	void	GoFirstFrame();
	void 	GoNextFrame();
	void	GoPreviousFrame();
	
	// --------------------------------------------------------------------------------

	void	SpendTime();
	
	void	SetGWorldPtr( GWorldPtr gworldPtr);
	void	AddRef();
	void	ReleaseRef();
	
	void 	Close();

	// --------------------------------------------------------------------------------
	Movie					movie;
	Track					track;
	Boolean					started;
	Boolean					doPlay;
	Rect					bounds;
	DxImage					*output;
	GWorldPtr				gworldPtr;

	int						grabStarted;
	int						refCount;
	long					frameCount;
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
