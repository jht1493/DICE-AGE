// --------------------------------------------------------------------------------
//
// Bridge to TextFild Control.
//
// 2003.04.04 	jht		Created.
//
// --------------------------------------------------------------------------------

#include "DxTextField.h"
#include "dicejs.h"
#include "DxWindow.h"

#import <Cocoa/Cocoa.h>

// --------------------------------------------------------------------------------
DxTextField::DxTextField(void *wrapperObj, void *uwindow, int32 left, int32 top, int32 width, int32 height)
	: DxGeneric(wrapperObj)
{
	NSWindow *nwindow = (NSWindow*)((DxWindow *)uwindow)->mWindow;
	NSView *contentView = [nwindow contentView];
	NSRect contentFrame = [contentView frame];
	NSRect frame;
	
	frame.origin.x = left;
	frame.origin.y = contentFrame.size.height - (top + height);
	frame.size.width = width;
	frame.size.height = height;
	
	NSTextField *textField = [[NSTextField alloc] initWithFrame:frame ];
	this->mTextField = textField;
	
	[textField setEditable: NO];
	
	[contentView addSubview: textField];
}

// --------------------------------------------------------------------------------
DxTextField::~DxTextField()
{
	NSTextField *textField = (NSTextField *)this->mTextField;
	[textField release];
}

// --------------------------------------------------------------------------------
void DxTextField::SetTextValue(jschar *chars, size_t length)
{
	NSTextField *textField = (NSTextField *)this->mTextField;
	NSString *newString = 
		[[NSString alloc] initWithCharactersNoCopy:chars length:length freeWhenDone:NO];
	[textField setStringValue: newString];
	[newString release];
}

// --------------------------------------------------------------------------------
void DxTextField::GetTextValue(jschar **chars, size_t *length)
{
	NSTextField *textField = (NSTextField *)this->mTextField;
	NSString *str = [textField stringValue];
	*length = [str length];
	*chars = (jschar*)malloc( *length * sizeof( jschar) );
	[str getCharacters: *chars];
	// !!@ need to free or pass owner ship of *chars
}

// --------------------------------------------------------------------------------
void DxTextField::SetEditable(JSBool flag)
{
	NSTextField *textField = (NSTextField *)this->mTextField;
	
	[textField setEditable: flag];
}

// --------------------------------------------------------------------------------
JSBool DxTextField::GetEditable()
{
	NSTextField *textField = (NSTextField *)this->mTextField;
	
	return [textField isEditable];
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
