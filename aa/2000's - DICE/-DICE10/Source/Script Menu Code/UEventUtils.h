//	===========================================================================//	UEventUtils.h				�1995-2000 Metrowerks Inc. All rights reserved.//	===========================================================================#pragma	once#include	<PP_Prefix.h>class	UEventUtils {public:	static	Boolean		KeyDownInMap(UInt16 inScanCode, const KeyMap inKeys);	static	short		GetModifiers(void);	static	void		FleshOutEvent(Boolean inOldButtonWasDown, EventRecord *ioEvent);};//	===========================================================================//	Modifier key scan codesenum	{	cmdKeyCode = 0x37,	shiftKeyCode,	alphaLockKeyCode,	optionKeyCode,	controlKeyCode};	