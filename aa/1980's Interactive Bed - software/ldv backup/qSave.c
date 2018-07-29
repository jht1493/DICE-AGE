/**/#include "qDef.h"#include "FileMgr.h"extern char	qtrace;#define TRACE(a,b) if (qtrace>=a) printf btypedef unsigned short ushort;#define bufSize	512#define creator		'????'#define fileType	'yeah'#define qVersion	'yeah'#define	qSymbolq		((int) (qSymbolBit >> 16))#define qStartListq		((int) (qListBit >> 16))#define qEndListq		((int) (qStartListq + 1))#define qOtherq			((int) (qOtherBit >> 16))#define vRef			0qItem	SaveEnv(ev, sy)	Env		*ev;	qSymbol	*sy;	{	int		er;	Byte	*fn;	int		fd;		if (! Symbolp(sy))		{		printf("SaveEnv: not a symbol %lx\n",sy);		return (NIL);		}	fn = sy->name;		er = FSDelete(fn, 0);	er = Create(fn, 0, creator, fileType);	if (er)		{		printf("Create: er=%d on %s\n", er, fn+1);		return (NIL);		}			er = FSOpen(fn, vRef, &fd);	if (er)		{		printf("FSOpen: er=%d on %s\n", er, fn+1);		return (NIL);		}		SaveToFile(ev, fd);		FSClose(fd);		FlushVol(NULL, vRef);		return ((qItem) sy);	}	/* ------------------------------------------------------------------- */struct fileHeader		{		long	version;		long	gensym;		long	nsyms;		long	nstrs;		long	ncons;		};static ushort	*bp;static ushort	*bstop;static ushort	*buf0;static int		fd;static long		consCount;static SaveToFile(ev, fd1)	Env		*ev;	int		fd1;	{	long			nsyms;	long			nstrs;	register qSpaceHdr	*hd;	register qSymbol	*sy;	register int		i;	int					index;	struct fileHeader	fh;	ushort buf[bufSize];		fd = fd1;	consCount = 0;	/* Determine index for all symbols */		index = 0;	for (hd = ev->syms.first; hd; hd = hd->next)		{		for (i = 0, sy = (qSymbol *)&hd->s; 				i < hd->nalloced; i++, sy++, index++)			{			sy->index = index;			}		}		/* Total symbols */	for (nsyms = 0, hd = ev->syms.first;			hd ; nsyms += hd->nalloced, hd = hd->next);				for (nstrs = 0, hd = ev->strs.first;			hd ; nstrs += hd->nalloced, hd = hd->next);	/* Round up to next word boundary */	if (nstrs & 0x01)		{		nstrs = nstrs + 1;		ev->strs.last->nalloced += 1; /* Use up one extra byte */		}		/* Write Header */	fh.version = qVersion;	fh.gensym = ev->gensym;	fh.nsyms = nsyms;	fh.nstrs = nstrs;	fh.ncons = 0;	WriteBuf( &fh, (long) sizeof(fh));	/* Write symbol name strings */	for (hd = ev->strs.first; hd; hd = hd->next)		WriteBuf( &hd->s, hd->nalloced);		/* Write the symbol values etc. */			buf0 = buf;	bstop = buf + bufSize;	bp = buf;	for (hd = ev->syms.first; hd; hd = hd->next)		{		for (sy = (qSymbol *) &hd->s, i = 0; 				i < hd->nalloced; i++, sy++)			{			WriteObj( sy->val);			WriteObj( sy->plist);			WriteObj( sy->func);			WriteWord( sy->hashId);			}		}			CloseBuf();		/* Set cons count in header */	fh.ncons = consCount + nsyms;	Lseek(fd, (long) 0);	WriteBuf( &fh, (long) sizeof(fh));				TRACE(1,(" nsyms=%ld nstrs=%ld ncons=%ld\n", nsyms, nstrs, fh.ncons));			}	static Lseek(fd, pos)	int		fd;	long	pos;	{	int		er;		er = SetFPos(fd, fsFromStart, pos);	if (er) printf("Lseek: er=%d\n", er);	}	static WriteObj( it)	register qItem	it;	{	if (Symbolp(it))		{		WriteWord( qSymbolq | ((qSymbol *)it)->index );		}	else if (Listp(it))		{		WriteList( it);		}	else if (Otherp(it))		{		/* Nil and Other objects */		WriteWord( (ushort) ( ((unsigned long)it) >> 16) );		}	else		{		/* Number */		WriteWord( (ushort) ( ((unsigned long)it) >> 16) );		WriteWord( (ushort)it );		}	}	static WriteList(lp)	register qList	*lp;	{		TRACE(1,("\nWriteList: lp=%lx\n",lp));		WriteWord( qStartListq );		for (; Listp(lp); lp = Cdr(lp))		{		consCount += 1;		WriteObj( Car(lp));		}			WriteWord( qEndListq );	}	static WriteWord(w)	{	TRACE(1,(" %x ", w));		if (bp >= (ushort *)bstop)		{		WriteBuf(buf0, ((long) bufSize) * sizeof(*bp));		bp = buf0;		}	*bp++ = w;	}	static WriteBuf( buf, count)	ushort	*buf;	long	count;	{	int		er;	er = FSWrite(fd, &count, buf);	if (er)		{		printf("WriteBuf: er=%d\n", er);		}	}	static CloseBuf()	{	long	count = ((long)bp) - ((long)buf0);	if (count > 0)		{		WriteBuf( buf0, count);		}	}	/* --------------------------------------------------------------------- */FreeEnv(ev) 	Env			*ev;	{	int		i;	ev->freeList = (qList *)NIL;	ev->eStack = (qList *)NIL;			for (i = 0; i < ev->tabSize; i++)		ev->symTab[i] = (qList *)NIL;			FreeSpace(&ev->syms);	FreeSpace(&ev->strs);	FreeSpace(&ev->cons);		}static FreeSpace(sp)	qSpace				*sp;	{	register qSpaceHdr	*hd;	register qSpaceHdr	*nh;		for (hd = sp->first; hd; hd = nh)		{		nh = hd->next;		DisposPtr(hd);		}			sp->first = NULL;	sp->last = NULL;	}	/* -------------------------------------------------------------------- */qItem			LoadObj();unsigned int	ReadWord();static qSymbol	*symBase;static qList	*consp;LoadEnv(ev, fd1)	Env			*ev;	int			fd1;	{	struct fileHeader	fh;	Byte				*names;	qSymbol				*sy;	register int		n;	ushort 				buf[bufSize];	long				NewSpace();		fd = fd1;		ReadBuf(&fh, (long) sizeof(fh));	TRACE(1,("LoadEnv: nsyms=%ld nstrs=%ld ncons=%ld\n",				fh.nsyms,fh.nstrs,fh.ncons));					if (fh.version != qVersion)		{		printf("Load bad file\n");		FSClose(fd);		return;		}			ev->gensym = fh.gensym;		consCount = fh.ncons;		/* Allocate symbol names space */	names = (Byte *) NewSpace(ev, &ev->strs, fh.nstrs);	ev->strs.last->nalloced = fh.nstrs;	NewSpace(ev, &ev->strs, ev->strs.allocUnit);		/* Allocate symbol space */	sy = (qSymbol *) NewSpace(ev, &ev->syms, fh.nsyms);	ev->syms.last->nalloced = fh.nsyms;	NewSpace(ev, &ev->syms, ev->syms.allocUnit);	symBase = sy;		/* Allocate cons space */	consp = (qList *) NewSpace(ev, &ev->cons, fh.ncons);	consp = (qList *) qPtr(consp, qListBit);	ev->cons.last->nalloced = fh.ncons;	TRACE(1,(" names=%lx sy=%lx consp=%lx\n",names,sy,consp));		/* Setup file read pointers */		buf0 = buf;	bstop = buf + bufSize;	bp = buf + bufSize;			/* Read in symbol names */	ReadBuf(names, fh.nstrs);		sy = (qSymbol *) qPtr(sy, qSymbolBit);	for (n = fh.nsyms; n > 0; n--, sy++, names += names[0] + 2)		{		register long	hi;						sy->name = names;		sy->val = LoadObj(ReadWord());		sy->plist = (qList *) LoadObj(ReadWord());		sy->func = LoadObj(ReadWord());				/* Link into symbol table */		hi = ReadWord();		sy->hashId = hi;		Rplacd(consp, ev->symTab[hi]);		Rplaca(consp, sy);		ev->symTab[hi] = consp;		consp++;		consCount--;		}			if (consCount != 0) 		printf("LoadEnv: bad conscount=%ld!\n",consCount);		FSClose(fd);	}	static qItem	LoadObj(x)	register ushort	x;	{	qItem			LoadList();			if (x & qSymbolq)		{		return ((qItem) qPtr(symBase + (x & ~qSymbolq), qSymbolBit ) );		}	if (x == qStartListq)		{		return (LoadList());		}	if (x & qOtherq)		{		return (NIL);		}			return ((qItem) ( ((long)x) << 16) | ReadWord());	}	static qItem	LoadList()	{	register ushort	x;	register qList	*ocp = (qList *) NIL;	qList			*top;		TRACE(1,("\nLoadList "));		top = consp;	for (;;)		{				x = ReadWord();		if (x == qEndListq)			break;					ocp = consp;		consp++;		consCount--;					Rplaca(ocp, LoadObj(x));		Rplacd(ocp, consp);		}			if (Nullp(ocp))		{		printf("LoadList: null!\n");		return (NIL);		}			Rplacd(ocp, NIL);	return ((qItem) top);	}	static ushort	ReadWord()	{		if (bp >= bstop)		{		bp = buf0;				ReadBuf(bp, (long) bufSize * sizeof(*bp));		}	TRACE(1,(" %x ", *bp));		return (*bp++);	}	static	ReadBuf(bp, count)	ushort	*bp;	long	count;	{	int		er;		TRACE(1,("ReadBuf: bp=%lx co=%ld\n", bp,count));		er = FSRead(fd, &count, bp);	if (er && er != -39) printf("ReadBuf: er=%d\n", er);		TRACE(1,(" (%x %x %x %x %x %x) \n",bp[0],bp[1],bp[2],bp[3],bp[4],bp[5]));	}		