// --------------------------------------------------------------------------------
//
// Bridge to TextFild Control.
//
// 2003.04.04 	jht		Created.
//
// --------------------------------------------------------------------------------

#include "DxTextField.h"
#include "dicejs.h"

// --------------------------------------------------------------------------------
DxTextField::DxTextField(void *wrapperObj, void *uwindow, int32 left, int32 top, int32 width, int32 height)
	: DxGeneric(wrapperObj)
{
}

// --------------------------------------------------------------------------------
DxTextField::~DxTextField()
{
}

// --------------------------------------------------------------------------------
void DxTextField::SetDescriptor(jschar *chars, size_t length);
{
}

// --------------------------------------------------------------------------------
void DxTextField::GetDescriptor(jschar **chars, size_t *length)
{
}

#if 0
// --------------------------------------------------------------------------------
DxTextField::DxTextField(
	void				*wrapperObj,
	const SPaneInfo&	inPaneInfo,
	ConstStringPtr		inString,
	ResIDT				inTextTraitsID,
	SInt16				inMaxChars,
	UInt8				inAttributes,
	TEKeyFilterFunc		inKeyFilter,
	LCommander*			inSuper)
	: DxGeneric(wrapperObj),
		LEditField( inPaneInfo, inString, inTextTraitsID, inMaxChars, inAttributes, inKeyFilter, inSuper )
{
}

// --------------------------------------------------------------------------------
DxTextField::DxTextField(LStream *inStream)
	: LEditField(inStream)
{
}
#endif

// --------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
