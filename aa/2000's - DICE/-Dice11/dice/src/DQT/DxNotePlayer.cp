/* History:

© 2003 John Henry Thompson. All rights reserved.

2003.03.10	jht Extracted from QTNote player Xtra. Bye bye Macromedia Director.

*/

/*

- Get from NoteRequest
  Str31               synthesizerName;        /- name of instantiation of synth -/
  Str31               instrumentName;         /- preferred name for human use -/

*/

#include "DxNotePlayer.h"

/* -----------------------------------------------------  */
DxNotePlayer::DxNotePlayer(int instrumentNum, int polyphonyNum, double polyphonyHint)
{
	NoteChannel				myNoteChannel;
	NoteRequest				myNoteRequest;
	
	this->noteAllocator = 0;
	this->noteChannel = 0;
	
	if (! this->noteAllocator)
		this->noteAllocator = OpenDefaultComponent(kNoteAllocatorComponentType, 0);

	myNoteChannel = NULL;

	// fill out a note request, using NAStuffToneDescription
	myNoteRequest.info.flags = 0;
	//myNoteRequest.info.reserved = 0;
	*(short *)(&myNoteRequest.info.polyphony) = EndianS16_NtoB( polyphonyNum );				// simultaneous tones
	*(Fixed *)(&myNoteRequest.info.typicalPolyphony) = EndianU32_NtoB( (long)(polyphonyHint*0x00010000) );

	this->myErr = NAStuffToneDescription( this->noteAllocator, instrumentNum, &myNoteRequest.tone);
	
	this->myErr = NANewNoteChannel( this->noteAllocator, &myNoteRequest, &myNoteChannel);
	
	if (myNoteChannel)
	{
		this->noteChannel = myNoteChannel;
		this->noteRequest = myNoteRequest;
	}
	this->instrumentNum = instrumentNum;
	this->npoly = polyphonyNum;
}

/* -----------------------------------------------------  */
DxNotePlayer::~DxNotePlayer()
{
	if (this->noteAllocator) 
		CloseComponent( this->noteAllocator );
		
	// !!@ noteChannel
}

/* -----------------------------------------------------  */
void DxNotePlayer::Task(void)
{
	NATask( this->noteAllocator );
}

/* -----------------------------------------------------  */
void DxNotePlayer::Play(int pitchNum, int velocityNum )
{
	// middle C at velocity 80
#if 0

	The NAPlayNote function plays a note with a specified pitch and velocity on the
	specified note channel.
	
pascal ComponentResult NAPlayNote(
	NoteAllocator na,
	NoteChannel noteChannel,
	long pitch,
	long velocity);

	na 		You obtain the note allocator identifier from the Component ManagerÕs OpenComponent function. 
		See the chapter ÒComponent ManagerÓ in QuickTime 4 Reference for details.

	noteChannel 	The note channel to play the note. You obtain the note channel
		identifier from the NANewNoteChannel or the
		NANewNoteChannelFromAtomicInstrument function.

	pitch 		The pitch at which to play the note. You can specify values as
		integer pitch values (0Ð127 where 60 is middle C) or fractional
		pitch values (256 (0x1.00) through 32767 (0x7F.FF)).

	velocity 	The velocity with which the key is struck. A value of 0 is silence;
		a value of 127 is maximum force.

#endif

	NAPlayNote( this->noteAllocator, this->noteChannel, pitchNum, velocityNum);					
}

/* -----------------------------------------------------  */
void DxNotePlayer::SetInstrument(int instrNum)
{
	NASetInstrumentNumber(this->noteAllocator, this->noteChannel, instrNum);
	this->instrumentNum = instrNum;
}

/* -----------------------------------------------------  */
int DxNotePlayer::GetInstrument()
{
	return this->instrumentNum;
}

/* -----------------------------------------------------  */
void DxNotePlayer::SetNPoly(int npoly)
{
	//NASetInstrumentNumber(this->noteAllocator, this->noteChannel, instrNum);
	// !!@ Need to deallocate and re-allocate noteChannel
	this->npoly = npoly;
}

/* -----------------------------------------------------  */
int DxNotePlayer::GetNPoly()
{
	return this->npoly;
}


/* -----------------------------------------------------  */
void DxNotePlayer::GetInstrumentName( Str255 instrName)
{
	NoteRequest		nr;
	
	NAGetNoteRequest(this->noteAllocator, this->noteChannel, &nr);
	
	BlockMoveData( nr.tone.instrumentName, instrName, nr.tone.instrumentName[0]+1);
}

/* -----------------------------------------------------  */
int DxNotePlayer::PickInstrument( void)
{
	#if 0
	NAPickEditInstrument( this->noteAllocator, 0/*filterPoc*/, "\pPick instrument"/*prompt*/, 
		0/*refcon*/, this->noteChannel, 0 /*AtomicInstrument*/, kPickEditAllowPick/*flags*/ );

	NoteRequest		nr;
		
	NAGetNoteRequest(this->noteAllocator, this->noteChannel, &nr);
	return nr.tone.gmNumber;
	
	#endif
	
	#if 1
	ToneDescription	tone;
	unsigned char *prompt = (unsigned char *)"\pPick instrument";
	
	NAPickInstrument( this->noteAllocator,  0/*filterPoc*/, prompt, 
		&tone, kPickEditAllowPick/*flags*/, 0/*refCon*/, 0/*res1*/, 0/*res2*/ );
	
	return CFSwapInt32BigToHost( *(long*) & tone.instrumentNumber);
	//return tone.gmNumber;
	#endif
}

/* -----------------------------------------------------  */
int DxNotePlayer::GetController(int itemNum )
{
	long		newValue;
	
	NAGetController(this->noteAllocator, this->noteChannel, itemNum, &newValue);
	
	return newValue;
}

/* -----------------------------------------------------  */
// itemNum The controller to set; see Music Controllers.
// newValue Value for controller setting; typically 0 (0x00.00) to 32767 (0x7F.FF).
//
void DxNotePlayer::SetController(int itemNum, int newValue)
{
	
	NASetController(this->noteAllocator, this->noteChannel, itemNum, newValue);
}

//
#if 0
kControllerModulationWheel =1
kControllerBreath =2
kControllerFoot =4
kControllerPortamentoTime =5
kControllerVolume =7
kControllerBalance =8
kControllerPan =10
kControllerExpression =11
kControllerLever1 =16
kControllerLever2 =17
kControllerLever3 =18
kControllerLever4 =19
kControllerLever5 =80
kControllerLever6 =81
kControllerLever7 =82
kControllerLever8 =83
kControllerPitchBend =32
kControllerAfterTouch =33
kControllerSustain =64
kControllerSostenuto =66
kControllerSoftPedal =67
kControllerReverb =91
kControllerTremolo =92
kControllerChorus =93
kControllerCeleste =94
kControllerPhaser =95
kControllerEditPart =113
kControllerMasterTune =114
#endif
//


#if 0
/* -----------------------------------------------------  */
MoaError CScript_IMoaMmXScript::XScrpGetController(PMoaDrCallInfo callPtr)
{
	MoaMmValue		item;
	long			itemNum, valueNum;
	MoaError err = kMoaErr_NoErr;

	AccessArgByIndex(2, &item );
	
	this->pObj->pMmValue->ValueToInteger( &item, &itemNum);

	NAGetController(this->pObj->noteAllocator, this->noteChannel, itemNum, &valueNum);

	this->pObj->pMmValue->IntegerToValue( valueNum, &callPtr->resultValue);

	return(err);
}

/* ----------------------------------------------------- XScrpSetController */
MoaError CScript_IMoaMmXScript::XScrpSetController(PMoaDrCallInfo callPtr)
{
	MoaMmValue		item;
	MoaMmValue		value;
	long			itemNum, valueNum;
	MoaError err = kMoaErr_NoErr;

	AccessArgByIndex(2, &item );
	AccessArgByIndex(3, &value );
	
	this->pObj->pMmValue->ValueToInteger( &item, &itemNum);
	this->pObj->pMmValue->ValueToInteger( &value, &valueNum);

	NASetController(this->pObj->noteAllocator, this->noteChannel, itemNum, valueNum);

	return(err);
}

/* ----------------------------------------------------- XScrpGetKnob */
MoaError CScript_IMoaMmXScript::XScrpGetKnob(PMoaDrCallInfo callPtr)
{
	MoaMmValue		item;
	long			itemNum, valueNum;
	MoaError err = kMoaErr_NoErr;

	AccessArgByIndex(2, &item );
	
	this->pObj->pMmValue->ValueToInteger( &item, &itemNum);

	NAGetKnob(this->pObj->noteAllocator, this->noteChannel, itemNum, &valueNum);

	this->pObj->pMmValue->IntegerToValue( valueNum, &callPtr->resultValue);

	return(err);
}

/* ----------------------------------------------------- XScrpSetKnob */
MoaError CScript_IMoaMmXScript::XScrpSetKnob(PMoaDrCallInfo callPtr)
{
	MoaMmValue		item;
	MoaMmValue		value;
	long			itemNum, valueNum;
	MoaError err = kMoaErr_NoErr;

	AccessArgByIndex(2, &item );
	AccessArgByIndex(3, &value );
	
	this->pObj->pMmValue->ValueToInteger( &item, &itemNum);
	this->pObj->pMmValue->ValueToInteger( &value, &valueNum);

	NASetKnob(this->pObj->noteAllocator, this->noteChannel, itemNum, valueNum);
	
	return err;
}


#endif


