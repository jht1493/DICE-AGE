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
class DxMovieMaker : public DxGeneric
{
public:
	DxMovieMaker(void *wrapperObj );
	
	~DxMovieMaker();
			
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;
	
	
	// --------------------------------------------------------------------------------

	void 	PickFile();
	void	CreateFile(JSString *path, JSString *fileName);
	void	SetTimeScale(long timeScale);
	int32	GetTimeScale();
	void	SetFrameDuration(long dur);
	int32	GetFrameDuration();
	void 	AddFrame(DxImage *image, long dur);
	void 	Finish();
		
	// --------------------------------------------------------------------------------

	void	SpendTime();
	
	//void	SetGWorldPtr( GWorldPtr gworldPtr);
	void	AddRef();
	void	ReleaseRef();
	

	// --------------------------------------------------------------------------------
	Rect			bounds;
	FSSpec     	 	mySpec; 
	short       	resRefNum;
	Movie			movie;
	Track           theTrack; 
	Media           theMedia; 
	Handle          compressedData; 
	Ptr             compressedDataPtr; 
	ImageDescriptionHandle          imageDesc;
	int32			quality;
	int32			codec;
	int32			timeScale;
	int32			frameDuration;
		
	int				refCount;
	
	// --------------------------------------------------------------------------------
	void PickFileSingle(void);
	void InitForImage(DxImage *image);
	void AddFrameSingle(DxImage *image, long dur);
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
