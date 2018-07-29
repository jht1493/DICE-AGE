//

#import "DxWindowUI.h"

#include "DxWindow.h"
#include "dicejs_fio.h"

@implementation DxWindowUI

- (void)keyDown:(NSEvent *)theEvent
{
	//dicejs_fprintf(0, "keyDown\n");
	
	DxWindow* own = (DxWindow*)self->owner;
	
	NSString *keys = [theEvent charactersIgnoringModifiers];
	size_t length = [keys length];
	jschar *chars = (jschar*)malloc( length * sizeof( jschar) );
	[keys getCharacters: chars];

	if (! own->DoKeyDown(chars, length)) {
		[super keyDown: theEvent];
	}
	
	free( chars );
}

- (void)keyUp:(NSEvent *)theEvent
{
	//dicejs_fprintf(0, "keyUp\n");
	
	DxWindow* own = (DxWindow*)self->owner;
	
	//own->DoMouseClick();
}


- (void) setOwner: (void*)newOwner
{
	self->owner = newOwner;
}
@end

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
