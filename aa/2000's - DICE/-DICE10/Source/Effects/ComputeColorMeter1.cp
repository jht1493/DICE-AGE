/* History:� 2001-2002 John Henry Thompson. All rights reserved2002.02.12	Color Meter first attempt.2002.01.07	Making a living.2001.05.22	jht	Split off.*/#include "CCapturePane.h"#include "Math.h"// ---------------------------------------------------------------------------------/* Show color diff, max color clamp */void	CCapturePane::ComputeColorMeter1(){	int					hcount;	int					vcount;	int					width;	int					height;	int					rowBytes;		UInt32*				sourcePtr; 	UInt32*				monitorPtr; 	UInt32*				previous2Ptr; 	UInt32*				compositePtr; 		UInt32*				sourceLinePtr; 	UInt32*				monitorLinePtr; 	UInt32*				previous2LinePtr; 	UInt32*				compositeLinePtr; 		int	capPix;	int diffwPix;	#if 0	int	tolerance = diffTolerance*3;	#else	int	tolerance = diffTolerance*diffTolerance;	#endif		//sourcePtr = (UInt32*)GetPixBaseAddr( GetGWorldPixMap(gwSourcePtr) );	sourcePtr = (UInt32*)GetPixBaseAddr( GetGWorldPixMap(gwAvgPtr) );		monitorPtr = (UInt32*)GetPixBaseAddr( GetGWorldPixMap(gwMonitorPtr) );		previous2Ptr = (UInt32*)GetPixBaseAddr( GetGWorldPixMap(gwPrevious2Ptr) );	compositePtr = (UInt32*)GetPixBaseAddr( GetGWorldPixMap(gwCompositePtr) );	/* Assume all gworlds have the same rowBytes */		rowBytes = (**GetGWorldPixMap(gwCompositePtr)).rowBytes & 0x3FFF;	rowBytes = rowBytes / 4;		width = captureRect.right - captureRect.left;	height = captureRect.bottom - captureRect.top;	for (vcount = height; 		vcount > 0; 		vcount--, 			sourcePtr += rowBytes, 			monitorPtr += rowBytes,			previous2Ptr += rowBytes,			compositePtr += rowBytes)	{		sourceLinePtr = sourcePtr ;		previous2LinePtr = previous2Ptr ;		monitorLinePtr = monitorPtr;		compositeLinePtr = compositePtr;				for (hcount = width; 				hcount > 0; 				hcount--, 			sourceLinePtr++,  			previous2LinePtr++,			monitorLinePtr++, 			compositeLinePtr++)		{			#if 0			capPix = ((UInt8*)sourceLinePtr)[1] + ((UInt8*)sourceLinePtr)[2] + ((UInt8*)sourceLinePtr)[3];   			diffwPix = ((UInt8*)previous2LinePtr)[1] + ((UInt8*)previous2LinePtr)[2] + ((UInt8*)previous2LinePtr)[3] - capPix; 			if (diffwPix < -tolerance || diffwPix > tolerance) 			#else 			/* Compute distance between pixels */			capPix = ((UInt8*)sourceLinePtr)[1] - ((UInt8*)previous2LinePtr)[1];			diffwPix = capPix*capPix;			capPix   = ((UInt8*)sourceLinePtr)[2] - ((UInt8*)previous2LinePtr)[2];			diffwPix += capPix*capPix;			capPix   = ((UInt8*)sourceLinePtr)[3] - ((UInt8*)previous2LinePtr)[3];			diffwPix += capPix*capPix;			if (diffwPix > tolerance)			#endif			{ 				/* There is  difference  */ 				#if 0				// show trans layer				capPix = ( ((UInt8*)sourceLinePtr)[1] + ((UInt8*)sourceLinePtr)[2] + ((UInt8*)sourceLinePtr)[3] ) /3;				capPix = 128 + capPix/2;				((UInt8*)monitorLinePtr)[1] = ( ((UInt8*)sourceLinePtr)[1] + ((UInt8*)compositeLinePtr)[1] )/2;				((UInt8*)monitorLinePtr)[2] = ( ((UInt8*)sourceLinePtr)[2] + ((UInt8*)compositeLinePtr)[2] )/2;				((UInt8*)monitorLinePtr)[3] = ( ((UInt8*)sourceLinePtr)[3] + ((UInt8*)compositeLinePtr)[3] )/2;				#endif				#if 1				// show trans image				((UInt8*)monitorLinePtr)[1] =  ((UInt8*)sourceLinePtr)[1] ;				((UInt8*)monitorLinePtr)[2] =  ((UInt8*)sourceLinePtr)[2] ;				((UInt8*)monitorLinePtr)[3] =  ((UInt8*)sourceLinePtr)[3] ;				#endif				#if 0				((UInt8*)monitorLinePtr)[1] =  255 ;				((UInt8*)monitorLinePtr)[2] =  255 ;				((UInt8*)monitorLinePtr)[3] =  255 ;				#endif 							} 			else 			{ 				/* No difference */ 				#if 0				*monitorLinePtr = *compositeLinePtr;				#endif				#if 0				diffwPix = sqrt( diffwPix );				((UInt8*)monitorLinePtr)[1] = (UInt8)diffwPix;				((UInt8*)monitorLinePtr)[2] = (UInt8)diffwPix;				((UInt8*)monitorLinePtr)[3] = (UInt8)diffwPix;				#endif				#if 1				diffwPix = 0;				((UInt8*)monitorLinePtr)[1] = (UInt8)diffwPix;				((UInt8*)monitorLinePtr)[2] = (UInt8)diffwPix;				((UInt8*)monitorLinePtr)[3] = (UInt8)diffwPix;				#endif			} 		}	}		{		const BitMap	*pSource; 		const BitMap	*pDest;		GrafPtr	myPort = GetMacPort();		pSource = GetPortBitMapForCopyBits( gwMonitorPtr );		pDest = GetPortBitMapForCopyBits( myPort );		CopyBits( pSource, pDest, &captureRect, &drawRect, inkMode, 0);	}}