// ===========================================================================
//	CBasicApp.cp 				©1994-2000 Metrowerks Inc. All rights reserved. 
// ===========================================================================
//	This file contains the starter code for a basic PowerPlant project

#include "CBasicApp.h"

#include <LGrowZone.h>
#include <PP_Messages.h>
#include <PP_Resources.h>
#include <UDrawingState.h>
#include <UMemoryMgr.h>
#include <URegistrar.h>
#include "UDebugging.h"

#include <LWindow.h>
#include <UControlRegistry.h>
#include <UEnvironment.h>
#include <LScroller.h>
#include <LCaption.h>
#include <UDesktop.h>

#include "CCapturePane.h"
#include "CStatusPane.h"
#include "CEditFieldNum.h"
#include "CTextEditViewForScript.h"

#include "CCaptureWind.h"
#include "CControlWind.h"
#include "CNoteWind.h"
#include "CScriptTextWind.h"
#include "COutlineWind.h"

#include "COutlineTable.h"

#include "SCFinderUtility.h"
#include "SCScriptsMenu.h"

#include "CAppCommands.h"

#include "ResDef.h"

#include "CTimeBomb.h"

#include "dicejs.h"

#include <QTML.h>

LWindow* theWindow;
LWindow* theControlWindow;
LWindow* theNoteWindow;
LWindow* theScriptTextWindow;
LWindow* theOutLineWindow;

// From COutlineTableApp
SInt16 gVolIndex = 1; // this is a lame way to do this, but it works....

// ===========================================================================
//	¥ main
// ===========================================================================

int main()
{							
		// Set Debugging options
	SetDebugThrow_(debugAction_Alert);
	SetDebugSignal_(debugAction_Alert);

		// Initialize Memory Manager. Parameter is the number of
		// master pointer blocks to allocate
	InitializeHeap(3);
	
		// Initialize standard Toolbox managers
	UQDGlobals::InitializeToolbox();
	
		// Install a GrowZone to catch low-memory situations	
	new LGrowZone(20000);

	EnterMovies();
	
		// Create the application object and run
	CBasicApp	theApp;
	
	if (TimeBomb())
	{
		UDebugging::DebugString("Time Bomb expired!");
	}
	else
	{
		dicejs_Init();
		
		//dicejs_EvalString( "2+9", 3);
		
		theApp.Run();
		
		// 2005.02.22 jht: Causes crash. Need to find right order to dispose of UI & JS
		// dicejs_Close();
	}
	
	return 0;
}


// ---------------------------------------------------------------------------
//	¥ CBasicApp										[public]
// ---------------------------------------------------------------------------
//	Application object constructor

CBasicApp::CBasicApp()
{
	RegisterClasses();
	
	inFullScreen = false;
	fullScreenWindow = nil;
}


// ---------------------------------------------------------------------------
//	¥ ~CBasicApp									[public, virtual]
// ---------------------------------------------------------------------------
//	Application object destructor

CBasicApp::~CBasicApp()
{
	// Nothing
}


// ---------------------------------------------------------------------------
//	¥ StartUp										[protected, virtual]
// ---------------------------------------------------------------------------
//	Perform an action in response to the Open Application AppleEvent.
//	Here, issue the New command to open a window.

void CBasicApp::StartUp()
{
	// ObeyCommand(cmd_New, nil);
	//  SetupScriptMenu();
	
	//LWindow *window = LWindow::CreateWindow(windID,this);
	//window->Show();

	theScriptTextWindow = LWindow::CreateWindow(PPob_ScriptTextWindow, this);
	ThrowIfNil_(theScriptTextWindow);
	theScriptTextWindow->Show();

	theNoteWindow = LWindow::CreateWindow(PPob_NoteWindow, this);
	ThrowIfNil_(theNoteWindow);
	theNoteWindow->Show();

	theControlWindow = LWindow::CreateWindow(PPob_ControlWindow, this);
	ThrowIfNil_(theControlWindow);
	theControlWindow->Enable();
	//theControlWindow->Show();

	theOutLineWindow = LWindow::CreateWindow(PPob_WorkspaceWindow, this);
	ThrowIfNil_(theOutLineWindow);
	theOutLineWindow->Show();

	//CreateMainWindow();
	
	SetSleepTime( 0 );
}

// ---------------------------------------------------------------------------
void CBasicApp::CreateMainWindow()
{
	#if 1
	theWindow = LWindow::CreateWindow(PPob_SampleWindow, this);
	ThrowIfNil_(theWindow);
	theWindow->Show();
	#endif
}

// ---------------------------------------------------------------------------
static void makeFront( LWindow* theWin)
{
	if (theWin)
	{
		UDesktop::ShowDeskWindow( theWin );
		theWin->Select();
		#if 0
		UDesktop::SelectDeskWindow( theWin );
		#endif
	}
}

// ---------------------------------------------------------------------------
//	¥ ObeyCommand									[public, virtual]
// ---------------------------------------------------------------------------
//	Respond to Commands. Returns true if the Command was handled, false if not.

Boolean
CBasicApp::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	Boolean		cmdHandled = true;	// Assume we'll handle the command
	
	switch (inCommand) 
	{
		case cmd_CanvasWindow: 
		{
			if (! theWindow)
				CreateMainWindow();
			makeFront( theWindow ); 
			break;
		}
		case cmd_ControlWindow: 
		{
			makeFront( theControlWindow ); 
			break;
		}
		case cmd_NoteWindow: 
		{
			makeFront( theNoteWindow ); 
			break;
		}
		case cmd_ScriptWindow: 
		{
			makeFront( theScriptTextWindow ); 
			break;
		}
		case cmd_WorkspaceWindow: 
		{
			makeFront( theOutLineWindow ); 
			break;
		}
		case cmd_FullScreenOn:
		{
			if (! inFullScreen)
			{
				EnterFullScreenMode();
			}
			else
			{
				ExitFullScreenMode();
			}
			break;
		}
		case cmd_FullScreenOff:
		{
			ExitFullScreenMode();
			break;
		}
		case cmd_New: 
		{
			LWindow	*window = LWindow::CreateWindow(PPob_SampleWindow, this);
			window->Show(); 
			break;
		}
		default: 
		{
			cmdHandled = LApplication::ObeyCommand(inCommand, ioParam);
			break;
		}
	}
	
	return cmdHandled;
}

// ---------------------------------------------------------------------------
void
CBasicApp::EnterFullScreenMode()
{
	if (! inFullScreen)
	{
	    //Startup Full Screen Mode
	    BeginFullScreen(&oldScreenState,nil,0,0,&fullScreenWindow,0,fullScreenAllowEvents);
	    //BeginFullScreen(&oldScreenState,nil/*whichGD*/,0/*desiredWidth*/,0/*desiredHeight*/,
	    //			nil/*newWindow*/,0/*eraseColor*/,fullScreenAllowEvents);
	    inFullScreen = true;
	}
}

// ---------------------------------------------------------------------------
void
CBasicApp::ExitFullScreenMode()
{
	if (inFullScreen)
	{
	    EndFullScreen(oldScreenState,nil);
	    inFullScreen = false;
	}
}

// ---------------------------------------------------------------------------
//	¥ FindCommandStatus								[public, virtual]
// ---------------------------------------------------------------------------
//	Determine the status of a Command for the purposes of menu updating.

void
CBasicApp::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{
	switch (inCommand) 
	{
		case cmd_CanvasWindow:
		case cmd_ControlWindow: 
		case cmd_NoteWindow: 
		case cmd_ScriptWindow: 
		case cmd_WorkspaceWindow: 
		
		case cmd_VideoSettings:
		{
			outEnabled = true;
			break;
		}
		case cmd_New: 
		{
			outEnabled = true;
			break;
		}
		case cmd_FullScreenOn:
		{
			outEnabled = true;
			break;
		}
		case cmd_FullScreenOff:
		{
			outEnabled = true;
			break;
		}
		case cmd_LoadScriptFile:
		{
			outEnabled = true;
			break;
		}
		default: 
		{
			LApplication::FindCommandStatus(inCommand, outEnabled,
											outUsesMark, outMark, outName);
			break;
		}
	}
}


// ---------------------------------------------------------------------------
//	¥ RegisterClasses								[protected]
// ---------------------------------------------------------------------------
//	To reduce clutter within the Application object's constructor, class
//	registrations appear here in this seperate function for ease of use.

void
CBasicApp::RegisterClasses()
{
	if (UEnvironment::HasFeature(env_HasAppearance)) 
	{
		::RegisterAppearanceClient();
	}
	
	// Register the Appearance Manager/GA classes
	//UControlRegistry::RegisterClasses(); // Save about 200K of ap.
	UControlRegistry::RegisterAMClasses();
	
	//RegisterClass_(LScroller);
	RegisterClass_(LTextEditView);
	RegisterClass_(LScroller);
	//RegisterClass_(LStdButton); // Not used w/ AM
	RegisterClass_(LCaption);
	//RegisterClass_(LStdCheckBox);
	//RegisterClass_(LEditField);
	//RegisterClass_(LBorderAttachment);
	
	RegisterClass_(LWindow);
	RegisterClass_(CCapturePane);
	RegisterClass_(CStatusPane);
	RegisterClass_(CEditFieldNum);
	RegisterClass_(CTextEditViewForScript);
	
	//RegisterClassID_(CCaptureWind, LWindow::class_ID);	
	RegisterClassID_(CCaptureWind, kCaptureWindowID);	
	
	RegisterClassID_(CControlWind, kControlWindowID);	
	
	RegisterClassID_(CNoteWind, kNoteWindowID);	
	
	RegisterClassID_(CScriptTextWind, kScriptTextWindowID);	

	RegisterClassID_(COutlineWind, kWorkspaceWindowID);	

	RegisterClass_(COutlineTable);

}

// ---------------------------------------------------------------------------
void
CBasicApp::SetupScriptMenu()
{
#if 0
	
	// ------- handle scripts menu ---------
	// get spec for the Scripts Folder
	FSSpec	appSpec;
	long		folderID;
	
	UFinder::GetAppSpec(appSpec);	
	folderID = UFinder::GetFolderID(appSpec,"\pScript Menu Items");
		
	// attach a new handler for the scripts menu
	AddAttachment(new SCScriptsMenuHandler(kScriptsMenuID,appSpec.vRefNum,folderID));
#endif
}

// ---------------------------------------------------------------------------
//		¥ HandleCreateElementEvent()
// ---------------------------------------------------------------------------
//

LModelObject*	
CBasicApp::HandleCreateElementEvent(
									DescType			inElemClass,
									DescType			inInsertPosition,
									LModelObject*		inTargetObject,
									const AppleEvent	&inAppleEvent,
									AppleEvent			&outAEReply)
{
	LModelObject	*result = nil;
	
	switch (inElemClass) 
	{
		case cWindow:
			//LWindow	*window = LWindow::CreateWindow(windID,this);
			LWindow	*window = LWindow::CreateWindow(PPob_SampleWindow, this);
			window->Show(); 
			result = window;
			break;

		default:
			result = LApplication::HandleCreateElementEvent(
					inElemClass,inInsertPosition,inTargetObject,inAppleEvent,outAEReply);
	
	}
 
	return result;
}
									

// ---------------------------------------------------------------------------

