/* 

Bridge from scripting world to LWindow object.

Disable close for permanent windows.

2006-08-27	jht		Port to xcode. Farewell CodeWarrior and PowerPlant.
2003.04.04 	jht		Created.

*/

#include "DxWindow.h"
#include "DxWindowUI.h"
#include "dicejs.h"
#include "jsapi.h"
#include "dsDOM.h"

#import <Cocoa/Cocoa.h>

// --------------------------------------------------------------------------------
static NSWindow* CreateFullScreen(NSDocument* iself)
{
	NSWindow* mScreenWindow = 0;
    // Get the screen information.
    NSScreen* mainScreen = [NSScreen mainScreen]; 
    NSDictionary* screenInfo = [mainScreen deviceDescription]; 
    NSNumber* screenID = [screenInfo objectForKey:@"NSScreenNumber"];
 
    // Capture the screen.
    CGDirectDisplayID displayID = (CGDirectDisplayID)[screenID longValue]; 
    CGDisplayErr err = CGDisplayCapture(displayID);
    if (err == CGDisplayNoErr)
    {
        // Create the full-screen window if it doesnÕt already  exist.
        if (!mScreenWindow)
        {
            // Create the full-screen window.
            NSRect winRect = [mainScreen frame];
            mScreenWindow = [[DxWindowUI alloc] initWithContentRect:winRect
                    styleMask:NSBorderlessWindowMask 
                    backing:NSBackingStoreBuffered 
                    defer:NO 
                    screen:[NSScreen mainScreen]];
 
            // Establish the window attributes.
            [mScreenWindow setReleasedWhenClosed:NO];
            [mScreenWindow setDisplaysWhenScreenProfileChanges:YES];
            //[mScreenWindow setDelegate:iself];
 
 			#if 0
           // Create the content for the window.
            FullScreenView* theView = [[FullScreenView alloc]
                                             initWithFrame:winRect];
            [mScreenWindow setContentView:theView];
            [theView setNeedsDisplay:YES];
            [theView release];
			#endif
        }
 
		#if 0
        // Make the screen window the current document window.
        // Be sure to retain the previous window if you want to  use it again.
        NSWindowController* winController = [[iself windowControllers]
                                                 objectAtIndex:0];
        [winController setWindow:mScreenWindow];
		#endif
		
        // The window has to be above the level of the shield window.
        int32_t     shieldLevel = CGShieldingWindowLevel();
        [mScreenWindow setLevel:shieldLevel];
 
		#if 1
        // Show the window.
        //[mScreenWindow makeKeyAndOrderFront:iself];
		//[mScreenWindow orderFront:nil];
		//[mScreenWindow makeKeyAndOrderFront:nil];
		[mScreenWindow orderFront:nil];
		#endif
    }
	return mScreenWindow;
}

// --------------------------------------------------------------------------------
DxWindow::DxWindow(void *cx, void *wrapperObj, int32 left, int32 top, int32 width, int32 height) 
	: DxSurface(wrapperObj)
{
	NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
	NSRect contentRect;
	unsigned int styleMask = 
		NSTitledWindowMask|NSClosableWindowMask|
		NSMiniaturizableWindowMask|NSResizableWindowMask;
	NSBackingStoreType bufferingType = NSBackingStoreBuffered;
		//NSBackingStoreBuffered
		//NSBackingStoreRetained
		//NSBackingStoreNonretained
	NSWindow *nwindow;
	
	this->cx = cx;
	
	this->mWindow = 0;
	this->gworldPtr = 0;
	
	contentRect.origin.x = left;
	contentRect.origin.y = screenFrame.origin.y + screenFrame.size.height - top;
	contentRect.size.width = width;
	contentRect.size.height = height;
	if (width > 0) {
		mOffset = 22;
		nwindow = [[DxWindowUI alloc] 
						initWithContentRect:contentRect 
						styleMask:styleMask 
						backing:bufferingType defer:NO];
						
		[nwindow setLevel:NSNormalWindowLevel];		
		[nwindow orderFront:nil];
		//[nwindow makeKeyAndOrderFront:nil];
	}
	else {
		mOffset = 0;
		nwindow = CreateFullScreen( (NSDocument*) JS_GetContextPrivate( (JSContext*)cx ) );
	}
	
	if (! nwindow) 
		goto exit;
		
	this->mWindow = nwindow;
	WindowRef winRef = (WindowRef) [nwindow windowRef];
	this->gworldPtr = GetWindowPort(winRef);
	//int level = [nwindow level];
	//BOOL isVis = [nwindow isVisible];
	
	// !!@ Consider starting window invis
	#if 0
	[nwindow setLevel:NSNormalWindowLevel];		
	[nwindow orderFront:nil];
	#endif
	//isVis = [nwindow isVisible];
	//isVis = 0;
	
	[nwindow setOwner: this ];
	
exit:;
}

// --------------------------------------------------------------------------------
DxWindow::~DxWindow()
{
	NSWindow *nwindow = (NSWindow *)this->mWindow;
	[nwindow release];
}

// --------------------------------------------------------------------------------
void DxWindow::GetBounds( Rect *outRect)
{
	DxSurface::GetBounds( outRect );
	outRect->bottom -= mOffset;	// Remove window title area
}


// --------------------------------------------------------------------------------
JSBool DxWindow::DoKeyDown(jschar *chars, size_t len)
{
	char	*propName = "keyDown";
	JSBool		consumed = 0;
	JSContext *cx = (JSContext*)this->cx;
	JSObject *obj = (JSObject*)this->jsObj;
	JSBool	ok;
	jsval	foundVal;
	jsval	strVal;

    uintN argc = 1;
	jsval *argv = &strVal;
	jsval rval;
	
	ok = JS_LookupProperty( cx, obj, propName, &foundVal);
	if (foundVal == JSVAL_VOID) {
		return 0;
	}
	
	consumed = 1;
	
	ok = ds_ConvertUCStringToJSValue( cx, chars, len, &strVal);
	if (! ok )
		goto exit;
			
	ok = JS_CallFunctionName(cx, obj, propName, argc, argv, &rval);
	
exit:;
	return consumed;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
