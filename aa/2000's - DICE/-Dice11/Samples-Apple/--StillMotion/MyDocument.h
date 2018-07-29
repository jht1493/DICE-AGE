//
//  MyDocument.h
//  StillMotion
//

/*
  
 Copyright (C) 2007 Apple Inc. All Rights Reserved.
 
 */

#import <QTKit/QTKit.h>

@interface MyDocument : NSDocument
{
    IBOutlet QTCaptureView              *mCaptureView;
    IBOutlet QTMovieView                *mMovieView;
    
    QTMovie                             *mMovie;
    
    QTCaptureSession                    *mCaptureSession;
    QTCaptureDeviceInput                *mCaptureDeviceInput;
    QTCaptureDecompressedVideoOutput    *mCaptureDecompressedVideoOutput;
    
    CVImageBufferRef                    mCurrentImageBuffer;
}

- (IBAction)addFrame:(id)sender;

@end
