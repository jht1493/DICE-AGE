//
//  MyDocument.m
//  doc
//
//  Created by jht1 on 8/23/06.
//  Copyright __MyCompanyName__ 2006 . All rights reserved.
//

#import "MyDocument.h"
#import "MyDocument_c.h"

#import "dicejs.h"

static MyDocument *gMyDocument = 0;
static BOOL inited = NO;

@implementation MyDocument

- (id)init
{
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
        // If an error occurs here, send a [self release] message and return nil.
		if (mString = nil) {
			mString = [[NSAttributedString alloc] initWithString:@""];
		}
    }
    return self;
}

- (void)dealloc 
{
	[mString release];
	[mTimer release];
    if (self == gMyDocument)
	{
		gMyDocument = 0;
	}
    [super dealloc];
}

- (NSAttributedString *) string 
{
	return [[mString retain] autorelease]; 
}

- (void) setString: (NSAttributedString *) newValue 
{
	if (mString != newValue) {
		if (mString) [mString release];
		mString = [newValue copy];
	}
}

- (NSString *)windowNibName
{
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports 
	// multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"MyDocument";
}

- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{
    [super windowControllerDidLoadNib:aController];
    // Add any code here that needs to be executed once the windowController has loaded the document's window.
	if ([self string] != nil) {
		[[textView textStorage] setAttributedString: [self string]];
	}
	
	if (! gMyDocument)
	{
		gMyDocument = self;

		dicejs_Init();
		
		mTimer = [NSTimer scheduledTimerWithTimeInterval: 0.008
                            target:self
                            selector:@selector(kickTimer:)
                            userInfo:nil
                            repeats:YES
                    ];
					
		[mTimer retain];
	}
}

- (NSData *)dataRepresentationOfType:(NSString *)aType
{
    // Insert code here to write your document from the given data.  
	// You can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.
    
    // For applications targeted for Tiger or later systems, 
	// you should use the new Tiger API -dataOfType:error:.  
	// In this case you can also choose to override -writeToURL:ofType:error:, -fileWrapperOfType:error:, 
	// or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.

    //return nil;
	//return myData;
	NSData *data;
	[self setString:[textView textStorage]];
	//data = [NSArchiver archivedDataWithRootObject:[self string]];
	NSString *str;
	const char	*cstr;
	unsigned len, slen;
	
	str = [[self string] string];
	slen = [str length];
	cstr = [str UTF8String];
	len = strlen(cstr);
	data = [NSData dataWithBytes: cstr length: len ];
	return data;
}

- (BOOL)loadDataRepresentation:(NSData *)data ofType:(NSString *)aType
{
    // Insert code here to read your document from the given data.  
	// You can also choose to override -loadFileWrapperRepresentation:ofType: or -readFromFile:ofType: instead.
    
    // For applications targeted for Tiger or later systems, you should use the new Tiger API 
	// readFromData:ofType:error:.  In this case you can also choose to 
	// override -readFromURL:ofType:error: or -readFromFileWrapper:ofType:error: instead.
    //myData = data;
	//[myData retain];
	//NSAttributedString *tempString = [NSUnarchiver unarchiveObjectWithData: data ];
	const char	*cstr;
	unsigned len;
	NSString *str;
	
	cstr = [data bytes];
	len = [data length];
	str = [[NSString alloc] initWithBytes: cstr length: len encoding:NSUTF8StringEncoding ];
	NSAttributedString *tempString = [[NSAttributedString alloc] initWithString:str];
	
	[self setString:tempString];
	
	//[self appendString: @"\n---Some new stuff"];
	
	[str release];
	[tempString release];
	
    return YES;
}

- (void) textDidChange: (NSNotification *) notification 
{
	[self setString: [textView textStorage]];
}

- (void) appendString: (NSString *) newString
{
	NSTextView *myView = textView; 
	NSString *myText = newString; 
	NSRange endRange; 
	
	if (! myView)
		return;
		
	endRange.location = [[myView textStorage] length]; 
	endRange.length = 0; 
	[myView replaceCharactersInRange:endRange withString:myText]; 
	endRange.length = [myText length]; 
	[myView scrollRangeToVisible:endRange]; 
}

- (IBAction) fax: (id)sender
{
	[self execClick: sender];
}

- (IBAction) execClick: (id)sender
{
	NSArray *selRange;
	id		rangeObj;
	NSString *store;
	NSRange	foundRange;
	NSRange	searchRange;
	
	store = [[textView textStorage] mutableString];

	// Find current insertion point
	selRange = [textView selectedRanges];
	rangeObj = [selRange objectAtIndex: 0];
	foundRange = [rangeObj rangeValue];
	
	// Search from selection point backwards for new line
	if (foundRange.length == 0) 
	{
		searchRange.location = 0;
		searchRange.length = foundRange.location;

		NSCharacterSet *newLines;	
		newLines = [NSCharacterSet characterSetWithCharactersInString: @"\r\n"];
				 
		//[self appendString: @"\n---Some new stuff"];
		foundRange = [store rangeOfCharacterFromSet:newLines options:NSBackwardsSearch range:searchRange];
		if (foundRange.location == NSNotFound)
			foundRange.location = 0;
		foundRange.length = searchRange.length - foundRange.location;
	}
	NSString *subStr = [store substringWithRange: foundRange];
	
	// Extract c-string and eval.
	const char	*cstr;
	unsigned len;
	
	cstr = [subStr UTF8String];	// autorelease ptr
	len = strlen(cstr);
	
	dicejs_EvalStringPrint( self, (char *)cstr, len );
	
	//[subStr release];
}

+ (void) AppendString: (NSString *) newString
{
	if (gMyDocument) [gMyDocument appendString: newString];
}

- (void) runStartupScript
{
	NSBundle *bundle = [NSBundle mainBundle];
	if (! bundle) 
		return;
	
	NSString *scriptPath = [bundle pathForResource:@"startup" ofType:@"txt"];
	if (! scriptPath)
		return;
	
	NSString *scriptText = [NSString stringWithContentsOfFile:scriptPath encoding:NSUTF8StringEncoding error:nil];
	
	const char	*cstr = [scriptText UTF8String];	// autorelease ptr
	unsigned len = strlen(cstr);
	
#if 0
	dicejs_EvalStringPrint( self, (char *)cstr, len );
#endif
}

- (void) kickTimer: (NSTimer*)timer
{
	if (! inited)
	{
		inited = YES;
		[self runStartupScript];
	}
	dicejs_SpendTime();
}

@end

// -----------------------------------------------------------------------------
void MyDocument_writeUTF8String( const void * aBuf, size_t len)
{
	NSString *str;
	str = [[NSString alloc] initWithBytes: aBuf length: len encoding:NSUTF8StringEncoding ];
	[MyDocument AppendString: str];
	[str release];
}

// -----------------------------------------------------------------------------
// !!@ caller must release *chars
void MyDocument_GetDocPath( void *docInst, jschar **chars, size_t *length)
{
	MyDocument *myDoc = (MyDocument*)docInst;
	
	NSString *str = [myDoc fileName];
	NSString *str2;
	
	NSRange	foundRange;
	// Remove every thing after last /
	foundRange = [str rangeOfString:@"/" options:NSBackwardsSearch];
	if (foundRange.location != NSNotFound ) {
		foundRange.length = foundRange.location+1;
		foundRange.location = 0;
		str = [str substringWithRange:foundRange];
	}
	
	*length = [str length];
	*chars = (jschar*)malloc( *length * sizeof( jschar) );
	[str getCharacters: *chars];
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
