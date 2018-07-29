//

#import "DxPushButtonUI.h"

#include "DxPushButton.h"
#include "dicejs_fio.h"

@implementation DxPushButtonUI

#if 0
- (id)init
{
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
        // If an error occurs here, send a [self release] message and return nil.
    }
    return self;
}

- (void)dealloc 
{
    [super dealloc];
}

- (void)mouseUp:(NSEvent *)theEvent
{
	NSEvent* nevt = theEvent;
	dicejs_fprintf(0, "DxPushButtonUI mouseUp\n");
}

- (void)mouseDown:(NSEvent *)theEvent
{
	NSEvent* nevt = theEvent;
	dicejs_fprintf(0, "DxPushButtonUI mouseDown 1\n");
	[super mouseDown: theEvent];
	dicejs_fprintf(0, "DxPushButtonUI mouseDown 2\n");
}
#endif

- (IBAction) mouseClick: (id)sender
{
	//dicejs_fprintf(0, "mouseClick\n");
	
	DxPushButton* own = (DxPushButton*)self->owner;
	
	own->DoMouseClick();
}

- (void) setOwner: (void*)newOwner
{
	self->owner = newOwner;
}
@end

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
