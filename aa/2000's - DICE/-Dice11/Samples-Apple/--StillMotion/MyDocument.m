//
//  MyDocument.m
//  StillMotion
//

/*
 
 Copyright (C) 2007 Apple Inc. All Rights Reserved.
 
 */

#import "MyDocument.h"

@implementation MyDocument

- (NSString *)windowNibName
{
    return @"MyDocument";
}

- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{
    NSError *error = nil;
    
    [super windowControllerDidLoadNib:aController];
    
    [[aController window] setDelegate:self];
    
    if (!mMovie) {
        // Create an empty movie that writes to mutable data in memory
        mMovie = [[QTMovie alloc] initToWritableData:[NSMutableData data] error:&error];
        if (!mMovie) {
            [[NSAlert alertWithError:error] runModal];
            return;
        }
    }
    
    [mMovieView setMovie:mMovie];
    
    if (!mCaptureSession) {
        // Set up a capture session that outputs raw frames
        BOOL success;
        
        mCaptureSession = [[QTCaptureSession alloc] init];
        
        // Find a video device
        QTCaptureDevice *device = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
        success = [device open:&error];
        if (!success) {
            [[NSAlert alertWithError:error] runModal];
            return;
        }
        
        // Add a device input for that device to the capture session
        mCaptureDeviceInput = [[QTCaptureDeviceInput alloc] initWithDevice:device];
        success = [mCaptureSession addInput:mCaptureDeviceInput error:&error];
        if (!success) {
            [[NSAlert alertWithError:error] runModal];
            return;
        }
        
        // Add a decompressed video output that returns raw frames to the session
        mCaptureDecompressedVideoOutput = [[QTCaptureDecompressedVideoOutput alloc] init];
        [mCaptureDecompressedVideoOutput setDelegate:self];
        success = [mCaptureSession addOutput:mCaptureDecompressedVideoOutput error:&error];
        if (!success) {
            [[NSAlert alertWithError:error] runModal];
            return;
        }
        
        // Preview the video from the session in the document window
        [mCaptureView setCaptureSession:mCaptureSession];
        
        // Start the session
        [mCaptureSession startRunning];
    }
}

- (void)windowWillClose:(NSNotification *)notification
{
    [mCaptureSession stopRunning];
    
    QTCaptureDevice *device = [mCaptureDeviceInput device];
    if ([device isOpen])
        [device close];    
}

- (void)dealloc
{
    [mMovie release];
    
    [mCaptureSession release];
    [mCaptureDeviceInput release];
    [mCaptureDecompressedVideoOutput release];
    
    [super dealloc];
}

- (BOOL)readFromURL:(NSURL *)absoluteURL ofType:(NSString *)typeName error:(NSError **)outError
{
    QTMovie *newMovie = [[QTMovie alloc] initWithURL:absoluteURL error:outError];
    if (newMovie) {
        [newMovie setAttribute:[NSNumber numberWithBool:YES] forKey:QTMovieEditableAttribute];
        
        [mMovie release];
        mMovie = newMovie;
    }
    
    return (newMovie != nil);
}

- (BOOL)writeToURL:(NSURL *)absoluteURL ofType:(NSString *)typeName error:(NSError **)outError
{
    return [mMovie writeToFile:[absoluteURL path] withAttributes:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:QTMovieFlatten] error:outError];
}

// This delegate method is called whenever the QTCaptureDecompressedVideoOutput receives a frame
- (void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection
{
    // Store the latest frame
	// This must be done in a @synchronized block because this delegate method is not called on the main thread
    CVImageBufferRef imageBufferToRelease;
    
    CVBufferRetain(videoFrame);
    
    @synchronized (self) {
        imageBufferToRelease = mCurrentImageBuffer;
        mCurrentImageBuffer = videoFrame;
    }
    
    CVBufferRelease(imageBufferToRelease);
}

- (IBAction)addFrame:(id)sender
{
    // Get the most recent frame
	// This must be done in a @synchronized block because the delegate method that sets the most recent frame is not called on the main thread
    CVImageBufferRef imageBuffer;
    
    @synchronized (self) {
        imageBuffer = CVBufferRetain(mCurrentImageBuffer);
    }
    
    if (imageBuffer) {
        // Create an NSImage and add it to the movie
        NSCIImageRep *imageRep = [NSCIImageRep imageRepWithCIImage:[CIImage imageWithCVImageBuffer:imageBuffer]];
        NSImage *image = [[[NSImage alloc] initWithSize:[imageRep size]] autorelease];
        [image addRepresentation:imageRep];
        CVBufferRelease(imageBuffer);
        
        [mMovie addImage:image forDuration:QTMakeTime(1, 10) withAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
                                                                             @"jpeg", QTAddImageCodecType,
                                                                             nil]];
        [mMovie setCurrentTime:[mMovie duration]];
        
        [mMovieView setNeedsDisplay:YES];
        
        [self updateChangeCount:NSChangeDone];
    }
}

@end
