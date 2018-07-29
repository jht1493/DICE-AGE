// --------------------------------------------------------------------------------
//
// Image - wrap gworld
//
// 2005.02.21 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxImage.h"

#include "dicejs.h"

// --------------------------------------------------------------------------------
DxImage::DxImage(void *wrapperObj, int pixelDepth, Rect aRect, int ctableID  )
	: DxSurface(wrapperObj )
{	
	this->gworldPtr = CreateGWorldForDepth(  pixelDepth,  aRect,  ctableID );
}

// --------------------------------------------------------------------------------
DxImage::~DxImage()
{
	Close();
}

// --------------------------------------------------------------------------------
void DxImage::Process( )
{
	
}

// --------------------------------------------------------------------------------
JSBool DxImage::GetProp( DxPropID propId, DxPropType propType, void **outResult )
{
	switch (propId)
	{
		case kDxProp_value:
		{
			switch (propType)
			{
				case kDxType_GWorldPtr:
				case kDxType_CGrafPortPtr:
					if (outResult)
						*outResult = this->gworldPtr;
					return JS_TRUE;
					
				case kDxType_DxSurface:
				case kDxType_DxImage:
					if (outResult)
						*outResult = this;
					return JS_TRUE;
			}
			break;
		}
	}
	if (outResult)
		*outResult = 0;
	return JS_FALSE;
}

// --------------------------------------------------------------------------------
JSBool DxImage::SetProp( DxPropID propId, DxPropType propType, void *newValue )
{
	return JS_FALSE;
}

// ---------------------------------------------------------------------------------
GWorldPtr DxImage::CreateGWorldForDepth(int pixelDepth, Rect aRect, int ctableID  )
{
	CTabHandle ctable = 0;
	QDErr				err;
	GDHandle			aGDevice = 0;
	GWorldFlags			flags = 0;
	PixMapHandle		pixMapHd;
	GWorldPtr			gworldPtr;

	CGrafPtr		savePort;
	
	GetPort( &savePort);
	if (ctableID)
		ctable = GetCTable(ctableID);
	
	gworldPtr = 0;
		
	err = NewGWorld( &gworldPtr, pixelDepth, &aRect, ctable, aGDevice, flags);
	if (gworldPtr)
	{
		pixMapHd = GetGWorldPixMap(gworldPtr);
		LockPixels( pixMapHd );
	}
	
	if (ctable)
	{
		DisposeCTable( ctable );
	}
	
	SetPort( savePort );
	
	return gworldPtr;
}

// ---------------------------------------------------------------------------------
void DxImage::GetPixBaseInfo(UInt8** basePtr, int *rowBytes, Rect *bounds, int *pixelSize)
{
	PixMapHandle		pixMapHd;

	//this->gworldPtr->portPixMap;
	
	pixMapHd = GetGWorldPixMap( this->gworldPtr );
	if (! pixMapHd)
	{
		if (basePtr)
			*basePtr = (UInt8*)0;
		return;
	}
	
	if (basePtr)
		*basePtr = (UInt8*)GetPixBaseAddr( pixMapHd );
	if (rowBytes)
		*rowBytes = (**pixMapHd).rowBytes & 0x3FFF;
	if (bounds)
		*bounds = (**pixMapHd).bounds;
	if (pixelSize)
		*pixelSize = (**pixMapHd).pixelSize;
}

// ---------------------------------------------------------------------------------
int DxImage::GetWidth()
{
	PixMapHandle		pixMapHd;
	Rect				box;

	if (! this->gworldPtr)
		return 0;
	
	pixMapHd = GetGWorldPixMap(this->gworldPtr);
	box = (**pixMapHd).bounds;
	return box.right - box.left;
}
		
// ---------------------------------------------------------------------------------
int DxImage::GetHeight()
{
	PixMapHandle		pixMapHd;
	Rect				box;

	if (! this->gworldPtr)
		return 0;
	
	pixMapHd = GetGWorldPixMap(this->gworldPtr);
	box = (**pixMapHd).bounds;
	return box.bottom - box.top;
}

// ---------------------------------------------------------------------------------
void DxImage::Close()
{
	if (this->gworldPtr)
	{
		DisposeGWorld( this->gworldPtr );
	}
	this->gworldPtr = 0;
}

// ---------------------------------------------------------------------------------
int32 DxImage::DrawText(char *str, int32 locx, int32 locy, int32 size)
{
	GrafPtr			origPort;
	GDHandle		origDev;
	//Rect				box;
    //int					wid, hi;
    //int					hor, vert;
	//SInt16	yMax;
	//SInt16	yMin;
	//Point	numer = { 1, 1 };
	//Point	denom = { 1, 1 };
	short	width;
	//FontInfo	finfo;
    StyleParameter	astyle = outline | extend | bold | shadow;
    //int		hoff;
    //int		ran;
	
	int	len = strlen(str) ;
	
	if (! this->gworldPtr)
		return 0;
	
	GetGWorld(&origPort, &origDev);
	SetGWorld(this->gworldPtr, 0);
	
	TextSize( size );
	TextMode( srcBic );
	TextFace( astyle );
	
	SetOutlinePreferred( TRUE );
	SetPreserveGlyph( TRUE );
	//GetFontInfo( &finfo );
	//OutlineMetrics( len, str, numer, denom, &yMax, &yMin, 0, 0, 0);
	width = TextWidth( str, 0, len );

	MoveTo( locx, locy);

	::MacDrawText( str, 0, len );
	
	SetGWorld(origPort, origDev); 
	
	return width;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
