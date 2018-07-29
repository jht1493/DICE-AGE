// --------------------------------------------------------------------------------
//
// QuickTime Movie player
//
// 2005.03.01 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxMovie_js.h"
#include "DxMovie.h"

#include "DxImage_js.h"
#include "DxImage.h"

#include "dicejs.h"

//#include <Navigation.h>

#include "QTFrameStepper.h"


// --------------------------------------------------------------------------------
DxMovie::DxMovie(void *wrapperObj )
	: DxGeneric(wrapperObj )
{
	this->movie = 0;
	this->track = 0;
	this->bounds.left = 0;
	this->bounds.right = 640;
	this->bounds.top = 0;
	this->bounds.bottom = 480;
	this->started = false;
	this->doPlay = true;
	this->frameCount = -1;
}

// --------------------------------------------------------------------------------
DxMovie::~DxMovie()
{
	Close();
}

// --------------------------------------------------------------------------------
void DxMovie::Process( )
{	
	if (this->movie)
	{
		SpendTime();
	}
}

// --------------------------------------------------------------------------------
JSBool DxMovie::GetProp( DxPropID propId, DxPropType propType, void **outResult )
{
	return JS_FALSE;
}

// --------------------------------------------------------------------------------
JSBool DxMovie::SetProp( DxPropID propId, DxPropType propType, void *outResult )
{
	return JS_FALSE;
}

// ---------------------------------------------------------------------------------
void DxMovie::SetGWorldPtr( GWorldPtr gworldPtr)
{
	// gworldPtr may be 0 to turn off outputing.
	
	this->gworldPtr = gworldPtr;
		
	if (! gworldPtr)
	{
		return;
	}
}

// ---------------------------------------------------------------------------------
void DxMovie::SpendTime()
{	
	CGrafPtr	savePort;
	
	GetPort( &savePort);
	SetPort( this->gworldPtr );
	
	SetMovieGWorld(this->movie, this->gworldPtr, NULL );
	if (this->doPlay && !this->started)
	{
		StartMovie( this->movie );
		this->started = 1;	
	}
	MoviesTask( this->movie, 0);
	if (this->doPlay && IsMovieDone( this->movie))
	{
		GoToBeginningOfMovie( this->movie );
		StartMovie( this->movie );
	}
	
exit:;

	SetPort( savePort );
}

// ---------------------------------------------------------------------------------
void DxMovie::AddRef()
{
	this->refCount++;
}

// ---------------------------------------------------------------------------------
void DxMovie::ReleaseRef()
{
	if ( --this->refCount <= 0)
	{
		delete this;
	}
}

// ---------------------------------------------------------------------------------


typedef const OSType *		QTFrameTypeListPtr;

//////////
//
// QTFrame_CreateOpenHandle
// Return a handle to a dynamically-created 'open' resource.
//
//////////

static Handle QTFrame_CreateOpenHandle (OSType theApplicationSignature, short theNumTypes, QTFrameTypeListPtr theTypeList)
{
	Handle			myHandle = NULL;
	
	if (theTypeList == NULL)
		return(myHandle);
	
	if (theNumTypes > 0) {
		myHandle = NewHandle(sizeof(NavTypeList) + (theNumTypes * sizeof(OSType)));
		if (myHandle != NULL) {
			NavTypeListHandle 	myOpenResHandle	= (NavTypeListHandle)myHandle;
			
			(*myOpenResHandle)->componentSignature = theApplicationSignature;
			(*myOpenResHandle)->osTypeCount = theNumTypes;
			BlockMoveData(theTypeList, (*myOpenResHandle)->osType, theNumTypes * sizeof(OSType));
		}
	}
	
	return(myHandle);
}


//////////
//
// QTFrame_GetOneFileWithPreview
// Display the appropriate file-opening dialog box, with an optional QuickTime preview pane. If the user
// selects a file, return information about it using the theFSSpecPtr parameter.
//
// Note that both StandardGetFilePreview and NavGetFile use the function specified by theFilterProc as a
// file filter. This framework always passes NULL in the theFilterProc parameter. If you use this function
// in your own code, keep in mind that on Windows the function specifier must be of type FileFilterUPP and 
// on Macintosh it must be of type NavObjectFilterUPP. (You can use the QTFrame_GetFileFilterUPP to create
// a function specifier of the appropriate type.) Also keep in mind that Navigation Services expects a file 
// filter function to return true if a file is to be displayed, while the Standard File Package expects the
// filter to return false if a file is to be displayed.
//
//////////


static OSErr QTFrame_GetOneFileWithPreview (short theNumTypes, QTFrameTypeListPtr theTypeList, FSSpecPtr theFSSpecPtr, void *theFilterProc)
{
#if TARGET_OS_WIN32
	StandardFileReply	myReply;
#endif
#if TARGET_OS_MAC
	NavReplyRecord		myReply;
	NavDialogOptions	myDialogOptions;
	NavTypeListHandle	myOpenList = NULL;
	// !!@ NavEventUPP			myEventUPP = NewNavEventUPP(QTFrame_HandleNavEvent);
	NavEventUPP			myEventUPP = 0;
#endif
	OSErr				myErr = noErr;
	
	if (theFSSpecPtr == NULL)
		return(paramErr);
	
	// deactivate any frontmost movie window
	// !!@ QTFrame_ActivateController(QTFrame_GetFrontMovieWindow(), false);

#if TARGET_OS_WIN32
	// prompt the user for a file
	StandardGetFilePreview((FileFilterUPP)theFilterProc, theNumTypes, (ConstSFTypeListPtr)theTypeList, &myReply);
	if (!myReply.sfGood)
		return(userCanceledErr);
	
	// make an FSSpec record
	myErr = FSMakeFSSpec(myReply.sfFile.vRefNum, myReply.sfFile.parID, myReply.sfFile.name, theFSSpecPtr);
#endif

#if TARGET_OS_MAC
	// specify the options for the dialog box
	NavGetDefaultDialogOptions(&myDialogOptions);
	myDialogOptions.dialogOptionFlags -= kNavNoTypePopup;
	myDialogOptions.dialogOptionFlags -= kNavAllowMultipleFiles;
	// !!@ BlockMoveData(gAppName, myDialogOptions.clientName, gAppName[0] + 1);
	
	// create a handle to an 'open' resource
	// !!@ myOpenList = (NavTypeListHandle)QTFrame_CreateOpenHandle(kApplicationSignature, theNumTypes, theTypeList);
	myOpenList = (NavTypeListHandle)QTFrame_CreateOpenHandle('????', theNumTypes, theTypeList);
	if (myOpenList != NULL)
		HLock((Handle)myOpenList);
	
	// prompt the user for a file
	myErr = NavGetFile(NULL, &myReply, &myDialogOptions, myEventUPP, NULL, (NavObjectFilterUPP)theFilterProc, myOpenList, NULL);
	if ((myErr == noErr) && myReply.validRecord) {
		AEKeyword		myKeyword;
		DescType		myActualType;
		Size			myActualSize = 0;
		
		// get the FSSpec for the selected file
		if (theFSSpecPtr != NULL)
			myErr = AEGetNthPtr(&(myReply.selection), 1, typeFSS, &myKeyword, &myActualType, theFSSpecPtr, sizeof(FSSpec), &myActualSize);

		NavDisposeReply(&myReply);
	}
	
	if (myOpenList != NULL) {
		HUnlock((Handle)myOpenList);
		DisposeHandle((Handle)myOpenList);
	}
	
	DisposeNavEventUPP(myEventUPP);
#endif
 
	return(myErr);
}

// ---------------------------------------------------------------------------------
/*
	QTUSimpleGetMovie - Get a Movie from a specific file (simpler version)

pascal OSErr QTUSimpleGetMovie(Movie *theMovie)

theMovie				will contain the selected movie when function exits.

DESCRIPTION
	QTUSimpleGetMovie is a simplified version of getting a movie from a file, no need for
	returning refnums, res IDs of keeping track of FSSpecs (compared with QTUGetMovie)
*/

#define	kTypeListCount	1

static OSErr QTUSimpleGetMovie(Movie *theMovie)
{
	OSErr 			anErr = noErr;
	short			resFile = 0;
	short			resID = 0;
	Str255			movieName;
	Boolean			wasChanged;
	OSType 			myTypeList[kTypeListCount] = {kQTFileTypeMovie/*, kQTFileTypeQuickTimeImage*/};
	FSSpec			theFSSpec;
    
    //anErr = GetOneFileWithPreview (kTypeListCount, (TypeListPtr)&myTypeList, &theFSSpec, NULL);
    anErr = QTFrame_GetOneFileWithPreview (kTypeListCount, (QTFrameTypeListPtr) &myTypeList, &theFSSpec, NULL);
	if(anErr == noErr)
	{
		anErr = OpenMovieFile(&theFSSpec, &resFile, fsRdPerm); 
		// !!@ DebugAssert(anErr == noErr);
		if(anErr == noErr)
		{
			anErr = NewMovieFromFile(theMovie, resFile, &resID, movieName, newMovieActive, &wasChanged);
			// !!@ DebugAssert(anErr == noErr);

			CloseMovieFile(resFile);
		}
	}
	return anErr;
}

// ---------------------------------------------------------------------------------
void DxMovie::PickFile()
{
	OSErr		err;
	
	Close();
	
	err = QTUSimpleGetMovie(&this->movie);
}

// ---------------------------------------------------------------------------------
void DxMovie::OpenFile(JSString *path)
{
	OSErr		err;
	FSRef		fref;
	FSSpec		theFSSpec;
	 
	short			resFile = 0;
	short			resID = 0;
	Boolean			wasChanged;
	
	Close();
	
	char		*path1;
	
	path1 = JS_GetStringBytes(path);
	
	err = FSPathMakeRef( (UInt8*)path1, &fref, NULL);
	if (err)
	{
		return;
	}
	
	err = FSGetCatalogInfo(&fref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
	if (err) 
	{
		return;
	}
	err = OpenMovieFile(&theFSSpec, &resFile, fsRdPerm); 
	// !!@ DebugAssert(anErr == noErr);
	if(err == noErr)
	{
		err = NewMovieFromFile(&this->movie, resFile, &resID, NULL, newMovieActive, &wasChanged);
		// !!@ DebugAssert(anErr == noErr);

		CloseMovieFile(resFile);
	}
}

// ---------------------------------------------------------------------------------
void DxMovie::Play()
{
	this->doPlay = true;
	if (this->movie) 
	{
		this->started = false;
	}
}

// ---------------------------------------------------------------------------------
void DxMovie::Stop()
{
	this->doPlay = false;
	if (this->movie && this->started) 
	{
		StopMovie(this->movie);
	}
}

// ---------------------------------------------------------------------------------
long DxMovie::GetCurrentTime()
{
	if (this->movie) 
	{
		return GetMovieTime(this->movie, NULL);
	}
	return -1;
}

// ---------------------------------------------------------------------------------
void DxMovie::SetCurrentTime(long newTime)
{
	if (this->movie)
	{
		SetMovieTimeValue( this->movie, newTime);
	}
}

// ---------------------------------------------------------------------------------
long DxMovie::GetFrameCount()
{
	if (this->frameCount >= 0)
		return this->frameCount;
	
	if (this->movie) 
	{
		this->track = GetMovieIndTrackType(this->movie, 1, VideoMediaType, movieTrackMediaType);
	}
	if (this->track)
	{
		this->frameCount = QTStep_GetFrameCount(this->track);
	}
	return this->frameCount;
}

// ---------------------------------------------------------------------------------
void DxMovie::GoNextFrame()
{
	if (this->movie)
	{
		QTStep_GoToNextVideoSample( this->movie );
	}
}

// ---------------------------------------------------------------------------------
void DxMovie::GoPreviousFrame()
{
	if (this->movie)
	{
		QTStep_GoToPrevVideoSample( this->movie );
	}
}

// ---------------------------------------------------------------------------------
void DxMovie::GoFirstFrame()
{
	if (this->movie)
	{
		QTStep_GoToFirstVideoSample( this->movie );
	}
}

// ---------------------------------------------------------------------------------
long DxMovie::GetTimeScale()
{
	if (this->movie)
	{
		return GetMovieTimeScale( this->movie );
	}
	return -1;
}

// ---------------------------------------------------------------------------------
void DxMovie::Close()
{
	if (this->movie)
	{
		DisposeMovie( this->movie );
		this->movie = 0;
		this->track = 0;
		this->frameCount = -1;
	}
}

// ---------------------------------------------------------------------------------
long DxMovie::GetWidth()
{
	Rect	box;
	if (this->movie)
	{
		GetMovieBox( this->movie, &box );
		return box.right - box.left;
	}
	return -1;
}

// ---------------------------------------------------------------------------------
long DxMovie::GetHeight()
{
	Rect	box;
	if (this->movie)
	{
		GetMovieBox( this->movie, &box );
		return box.bottom - box.top;
	}
	return -1;
}

// ---------------------------------------------------------------------------------
long DxMovie::GetDuration()
{
	if (this->movie)
	{
		return GetMovieDuration( this->movie );
	}
	return -1;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
