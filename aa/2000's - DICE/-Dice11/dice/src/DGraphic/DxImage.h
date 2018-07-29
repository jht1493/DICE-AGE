// --------------------------------------------------------------------------------
//
// Image wraps gworld
//
// 2005.02.21 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "DxSurface.h"

#include <QuickTime/QuickTime.h>


// --------------------------------------------------------------------------------
class DxImage : public DxSurface
{
public:
	//DxImage(void *wrapperObj, int pixelDepth, Rect aRect, ResIDT ctableID  );
	DxImage(void *wrapperObj, int pixelDepth, Rect aRect, int ctableID  );
	
	~DxImage();
		
	// --------------------------------------------------------------------------------
			
	virtual void Process();
	
	virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;

	int32 DrawText(char *str, int32 locx, int32 locy, int32 size);
	
	// --------------------------------------------------------------------------------

	void GetPixBaseInfo(UInt8** basePtr, int *rowBytes, Rect *bounds, int *pixelSize = 0);
	
	//static GWorldPtr CreateGWorldForDepth(int pixelDepth, Rect aRect, ResIDT ctableID  );
	static GWorldPtr CreateGWorldForDepth(int pixelDepth, Rect aRect, int ctableID  );
	
	int		GetWidth();
	int		GetHeight();
	
	void 	Close();
	
	// --------------------------------------------------------------------------------
};


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
