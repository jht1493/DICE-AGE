/* 

Bridge from scripting world to LWindow object.

Disable close for permanent windows.

2006-08-27	jht		Port to xcode. Farewell CodeWarrior and PowerPlant.
2003.04.04 	jht		Created.

*/

#include "DxWindow.h"
#include "dicejs.h"


#if 0
// --------------------------------------------------------------------------------
DxWindow::DxWindow( 
	void *				wrapperObj,
	WindowClass			inWindowClass,
	WindowAttributes	inAttributes,
	const Rect&			inBounds)
	: DxSurface(wrapperObj),
		LWindow( inWindowClass, inAttributes, inBounds, LCommander::GetDefaultCommander())
{
	this->gworldPtr = this->GetMacPort();
}

// --------------------------------------------------------------------------------
DxWindow::DxWindow(LStream *inStream)
	: LWindow(inStream)
{
}

// --------------------------------------------------------------------------------
void DxWindow::FinishCreateSelf()
{
	int num;
	
	num = 1;
}
#endif

// --------------------------------------------------------------------------------
DxWindow::DxWindow(void *wrapperObj, const Rect& inBounds) 
	: DxSurface(wrapperObj)
{
}

// --------------------------------------------------------------------------------
DxWindow::~DxWindow()
{
}

#if 0
// --------------------------------------------------------------------------------
void DxWindow::ShowSelect()
{
	UDesktop::ShowDeskWindow( this );
	UDesktop::SelectDeskWindow( this );
}
		
// --------------------------------------------------------------------------------
void DxWindow::AttemptClose()
{
	::HideWindow( this->GetMacWindow() );
}

// --------------------------------------------------------------------------------
void DxWindow::DoClose()
{
	::HideWindow( this->GetMacWindow() );
}

// --------------------------------------------------------------------------------
void DxWindow::Process( )
{
	
}

// --------------------------------------------------------------------------------
Boolean DxWindow::GetProp( DxPropID propId, DxPropType propType, void **outResult )
{
	return DxSurface::GetProp(propId, propType, outResult);
}

// --------------------------------------------------------------------------------
Boolean DxWindow::SetProp( DxPropID propId, DxPropType propType, void *outResult )
{
	return FALSE;
}

// --------------------------------------------------------------------------------
void DxWindow::ListenToMessage(MessageT inMessage, void *ioParam)
{
	int		num;
	
	num = 1;
}
#endif

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
