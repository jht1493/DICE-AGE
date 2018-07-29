/*Pioneer LD-V4200 interface.*/#include <MacTypes.h>#include <MemoryMgr.h>#include <ToolboxUtil.h>#include <QuickDraw.h>#include <SerialDvr.h>#include <pascal.h>#include "HyperXCmd.h"#define NOVAL	(-1L)#define Nullp(a) ((a) == NOVAL)static int		chan = AoutRefNum;#define PPort	sPortAlong			arg1num;Byte			*arg1;long			vdwait();/* ------------------------ main -------------------------------------- */pascal void main(pp)	XCmdBlockPtr	pp;	{	Byte		*op;	Handle		h;		SetUpA4();#if 0	h = RecoverHandle(* (Ptr *) 0x9CE);	if (((long) *h) & 0x40000000) SysBeep(2);#endif	vdflush();		if (pp->paramCount < 1)		{		SysBeep(5);		goto End;		}			op = * (pp->params[0]);		if (pp->paramCount > 1)		{		arg1 = * (pp->params[1]);		if (stcd_i(arg1, &arg1num) > 0) ;		else if (strcmp(arg1, "on") == 0)			arg1num = 1;		else if (strcmp(arg1, "off") == 0)			arg1num = 0;		}		#define Is(a) (strcmp(op, a) == 0)	if (Is("port"))		SelectPort( (int) arg1num);			else if (Is("play"))		Vd1('PL');			else if (Is("search"))		Vdn('SE', (int)arg1num);			else if (Is("stopat"))		{		Vd('SM', (int)arg1num );		Vd1('PL');		}			else if (Is("setmark"))		Vd('SM', (int)arg1num );			else if (Is("waitfor"))		vdwait( arg1num );	else if (Is("chargen"))		Vd('DS', (int)arg1num);			else if (Is("pos"))		ReturnPos(pp);			else if (Is("frame"))		Vd('RA', (int)arg1num? 5: 4);			else if (Is("video"))		Vd('VD', (int)arg1num);			else if (Is("audio"))		Vd('AD', (int)arg1num);			else if (Is("stop"))		Vd1('ST');			else if (Is("slowfwd"))		SlowForward( (int)arg1num );			else if (Is("slowrev"))		SlowRev( (int)arg1num );		else if (Is("stepfwd"))		Vd1('SF');			else if (Is("steprev"))		Vd1('SR');			else if (Is("rev"))		RevPlay();			else if (Is("ff"))		FastForward( (int)arg1num );			else if (Is("rew"))		Rewind( (int)arg1num );			else if (Is("audiosq"))		Vd('RB', (int)arg1num? 0: 64);			else if (Is("text"))		DoText(pp);			else if (Is("cleartext"))		Vd1('CS');			else if (Is("init"))		VdOpen();			else SysBeep(2);End:	RestoreA4();	}	/* -------------------------------------------------------------------- */SlowRev(n)	{	if (n < 1) n = 60;	Vd('SP', 60/n);	Vd1('MR');	}	SlowForward(n)	{	if (n < 1) n = 60;	Vd('SP', 60/n);	Vd1('MF');	}	RevPlay()	{	Vd('SP', 60);	Vd1('MR');	}	FastForward(n)	{	if (n < 1) n = 1;	Vd('SP', 60*n);	Vd1('MF');	}Rewind(n)	{	if (n < 1) n = 1;	Vd('SP', 60*n);	Vd1('MR');	}	DelayBy(n)	{	long t;	Delay((long) n, &t);	}	/* ------------------------ SelectPort -------------------------------- */SelectPort(n)	int	n;	{	switch (n)		{		case 1:			n = 0x40;			break;		case 2:			n = 0xCF;			break;		case 3:			n = 0x8F;			break;		case 4:			n = 0x60;		default:			n = 0x40;			break;		}	MacEnhancer(n);	}	/* ------------------------ MacEnchancer ------------------------------ */MacEnhancer(op)	int	op;	{	int		val;	Handle	macen_h;	void	(*macen_f)();			macen_h = GetResource('PACK', 128);	if (! macen_h)		{		OhShit("\pGetResource macenh failed");		}			HLock(macen_h);		macen_f = (void (*)()) (*macen_h);		val = CallPascalW(18, -1, 0L, 0L, 0L, (int)op, 1/*-Modem-*/, macen_f);		VdOpen();		return (val);	}	/* ------------------------ ReturnPos ---------------------------------- */Handle	MakeNum();Handle	MakeString();long	vdreadpos();ReturnPos(pp)	XCmdBlockPtr	pp;	{	pp->returnValue = MakeNum(vdreadpos());	}	/* -------------------- MakeNum ---------------------------------------- */Handle MakeNum(n)	long	n;	{	Byte	str[16];		stci_d(str, n, 16);		return ( MakeString(str) );	}/* ----------------------- MakeString --------------------------------- */Handle MakeString(str)	Byte *str;	{	Handle hd;		hd = NewHandle((long) strlen(str)+1);	if (! hd)		{		OhShit("\pNewString no room");		return (hd);		}	strcpy(*hd, str);	return (hd);	}	/* -------------------- vdreadpos -------------------------------------- */long vdread();long vdreadpos()	{	long	it;		Vd1n('?F');		while ((it = vdread()) == NOVAL && ! Button()) ;		return (it);	}/* ------------------ vdread ------------------------------------------- */long vdread()	{	long count, n;	Byte buf[32];	int	er;		SerGetBuf(chan + 1, &count);	if (count >= 6)		{		er = FSRead(chan + 1, &count, buf);		if (er) OhShit("\pFSRead er=%d on count=%d\n", er, count);		buf[count] = 0;		stcd_i(buf, &n);		/* sscanf(buf, "%lx", &n); */#if 0		printf("(@ %s n=%ld c=%ld) ", buf, n, count);#endif		return ( n );		}			return (NOVAL);	}/* -------------------------- vdwait ------------------------------------ */long vdwait(target)	long		target;	{	long	vl;	long	to = 0x10000;		for (; !Button();)		{		Vd1n('?F');		for (; !Button();)			{			vl = vdread();			if (Nullp (vl))				continue;			if (vl != target)				break;			return (NOVAL);			}		}	}	/* --------------------- OhShit --------------------------------------- */OhShit(a,b)	Ptr	a, b;	{	DebugStr(a);	}	/* --------------- VdOpen --------------------------------------------- */static VdOpen()	{	int serConfig = baud4800 + stop10 + noParity + data8;	int er;		er = RAMSDOpen(PPort);	if (er) OhShit("\pRAMSopen");	er = SerReset(chan, serConfig);	if (er) OhShit("\pserreset of output");	er = SerReset(chan + 1, serConfig);	if (er) OhShit("\pserreset of input");	}	/* -------------------------------------------------------------------- */Vd1(op)	int	op;	{	Vd1n(op);	vdgetack();	}	Vd1n(op)	int	op;	{	int		er;	long	count;	Byte	buf[16];		buf[0] = op >> 8;	buf[1] = op;	buf[2] = 0x0D;	count = 3;	er = FSWrite(chan, &count, buf);	if (er) OhShit("\p vd1 write");	}	Vd(op, n)	{	Vdn(op, n);	vdgetack();	}	Vdn(op, n)	int			op;	unsigned	n;	{	Byte	buf[32];	long	count;	int		er;	count = stci_d(buf, (long)n, 16);	buf[count] = op >> 8;	buf[count+1] = op;	buf[count+2] = 0x0D;	count += 3;	er = FSWrite(chan, &count, buf);	if (er) OhShit("\p vd write");	}	vdflush()	{	long	count;	Byte	buf[128];	SerGetBuf(chan + 1, &count);		if (count < 1) return (0);	if (count > 128) count = 128;	FSRead(chan + 1, &count, buf);		return ( (int) count );	}	vdgetack()	{	long	count;	Byte	buf[32];	int		i;	long	timeOut = TickCount() + 5*60;		while (TickCount() < timeOut)		{		if (Button()) return;		SerGetBuf(chan + 1, &count);			if (count >= 2)			{			FSRead(chan + 1, &count, buf);			return;			}		}	SysBeep(2);	DelayBy(20);	SysBeep(2);	}/* ---------------- DoText -------------------------------------------- */DoText(pp)	XCmdBlockPtr	pp;	{	int		i, line, len;	Byte	buf[32];	Byte	*sp;	long	count;		line = arg1num;	for (i = 2; i < pp->paramCount; i++, line++)		{		Vd('PR', line);				sp = (Byte *) * (pp->params[i]);				len = (20 - strlen(sp))/2;		if (len < 1) len = 0;		strcpy(buf, "            ");		buf[len] = 0;		strcat(buf, sp);				count = strlen(buf);		buf[count] = 0x0D;		count += 1;		FSWrite(chan, &count, buf);				vdgetack();		}	}	/* --------------------------------------------------------------------- */	