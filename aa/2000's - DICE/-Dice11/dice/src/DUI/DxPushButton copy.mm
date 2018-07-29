/* 

Bridge from scripting world to LWindow object.

Disable close for permanent windows.

2003.04.04 	jht		Created.

*/

#include "DxPushButton.h"
#include "dicejs.h"
#include "DxWindow.h"
#include "DxPushButtonUI.h"

#import <Cocoa/Cocoa.h>

// --------------------------------------------------------------------------------
DxPushButton::DxPushButton(void *wrapperObj, void *uwindow, int32 left, int32 top, int32 width, int32 height)
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
	
	DxPushButtonUI *button = [[DxPushButtonUI alloc] initWithFrame:frame ];
	this->mButton = button;
	
	//[textField setEditable: NO];
	
	[contentView addSubview: button];
}

// --------------------------------------------------------------------------------
DxPushButton::~DxPushButton()
{
}

// --------------------------------------------------------------------------------
void DxPushButton::SetTitle(jschar *chars, size_t length)
{
	DxPushButtonUI *button = (DxPushButtonUI *)this->mButton;
	NSString *newString = 
		[[NSString alloc] initWithCharactersNoCopy:chars length:length freeWhenDone:NO];
	[button setTitle: newString];
	[newString release];
}

// --------------------------------------------------------------------------------
void DxPushButton::GetTitle(jschar **chars, size_t *length)
{
	DxPushButtonUI *button = (DxPushButtonUI *)this->mButton;
	NSString *str = [button title];
	*length = [str length];
	*chars = (jschar*)malloc( *length * sizeof( jschar) );
	[str getCharacters: *chars];
	// !!@ need to free or pass owner ship of *chars
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
