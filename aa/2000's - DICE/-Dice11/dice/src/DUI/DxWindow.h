/* 


2006-08-27	jht		Port to xcode. Farewell CodeWarrior and PowerPlant.
2003.03.25 	jht		Created.

*/

#pragma once

#include "DxSurface.h"
#include "jsapi.h"

class DxWindow : public DxSurface
{
public:
	void *mWindow;
	void *cx;
	int	mOffset;
	
	DxWindow(void *cx, void *wrapperObj, int32 left, int32 top, int32 width, int32 height);
	
	virtual					~DxWindow();
	
	virtual void GetBounds( Rect *outRect);

	JSBool DoKeyDown(jschar *chars, size_t len);
	
	#if 0
	DxWindow(
			void 				*wrapperObj,
			WindowClass			inWindowClass,
			WindowAttributes	inAttributes,
			const Rect&			inBounds);

			DxWindow(LStream *inStream);
	virtual	void			FinishCreateSelf();

	void				ShowSelect();
	virtual void		AttemptClose();
	virtual void		DoClose();

	virtual void	ListenToMessage(MessageT inMessage, void *ioParam);

	virtual void Process();
	
	virtual Boolean GetProp( DxPropID propId, DxPropType propType, void **outResult );
	
	virtual Boolean SetProp( DxPropID propId, DxPropType propType, void *outResult );
	#endif
};

