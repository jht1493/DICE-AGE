/* 

Bridge from scripting world to LWindow object.

Disable close for permanent windows.

2003.04.04 	jht		Created.

*/

#include "DxPushButton.h"
#include "dicejs.h"

#if 0
// --------------------------------------------------------------------------------
DxPushButton::DxPushButton(
	void				*wrapperObj,
	const SPaneInfo&	inPaneInfo,
	MessageT			inValueMessage,
	ConstStringPtr		inTitle
	)
	: DxGeneric(wrapperObj), 
		LPushButton( inPaneInfo, inValueMessage, inTitle )
{
}

// --------------------------------------------------------------------------------
DxPushButton::DxPushButton(LStream *inStream)
	: LPushButton(inStream)
{
}
#endif

// --------------------------------------------------------------------------------
DxPushButton::DxPushButton()
{
}

// --------------------------------------------------------------------------------
DxPushButton::~DxPushButton()
{
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
