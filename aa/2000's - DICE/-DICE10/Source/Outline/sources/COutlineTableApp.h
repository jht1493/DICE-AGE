// ===========================================================================//	COutlineTableApp.h	 	   �1997-2002 Metrowerks Inc. All rights reserved.// ===========================================================================//	Original Author: John C. Daub////	Demonstration application for the PowerPlant "Outline Table" classes (part//	of the Constructor Additions).#pragma once#include <LApplication.h>class	COutlineTableApp : public LApplication {public:						COutlineTableApp();		// constructor registers all PPobs	virtual 			~COutlineTableApp();		// stub destructor			// this overriding function performs application functions			virtual Boolean		ObeyCommand(CommandT inCommand, void* ioParam);				// this overriding function returns the status of menu items			virtual void		FindCommandStatus(CommandT inCommand,							Boolean &outEnabled, Boolean &outUsesMark,							UInt16 &outMark, Str255 outName);protected:	virtual void		StartUp();		// overriding startup functions		virtual	void		DoVolumeTable();	virtual	void		DoEditTable();};