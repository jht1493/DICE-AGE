//
//  MyDocument.h
//  doc
//
//  Created by jht1 on 8/23/06.
//  Copyright __MyCompanyName__ 2006 . All rights reserved.
//

#import <Cocoa/Cocoa.h>

// ------------------------------------------------------------------------------------------------------

@interface MyDocument : NSDocument
{
	//NSData	*myData;
	IBOutlet NSTextView *textView;
	NSAttributedString	*mString;
	NSTimer				*mTimer;
}
- (NSAttributedString *) string;
- (void) setString: (NSAttributedString *) newValue;
- (void) appendString: (NSString *) newString;
+ (void) AppendString: (NSString *) newString;
- (IBAction) execClick: (id)sender;
- (void) kickTimer: (NSTimer*)timer;
@end


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------