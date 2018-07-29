/* 

Bridge to PushButton control.

2003.03.25 	jht		Created.

*/

#pragma once

#include "DxGeneric.h"
#include "jsapi.h"

class DxPushButton : public DxGeneric 
{
public:
	void *mButton;
	void *cx;
	
	DxPushButton(void *cx, void *wrapperObj, void *uwindow, int32 left, int32 top, int32 width, int32 height);
	
	virtual					~DxPushButton();

	void SetTitle(jschar *chars, size_t length);
	void GetTitle(jschar **chars, size_t *length);
	
	void DoMouseClick();
	#if 0
	DxPushButton(
			void 				*wrapperObj,
			const SPaneInfo&	inPaneInfo,
			MessageT			inValueMessage,
			ConstStringPtr		inTitle);

	DxPushButton(LStream *inStream);
	#endif
	//virtual void		ClickSelf( const SMouseDownEvent &inMouseDown );
		
	//virtual void Process();
	
	//virtual JSBool GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	//virtual JSBool SetProp( DxPropID propId, DxPropType propType, void *outResult );

};

