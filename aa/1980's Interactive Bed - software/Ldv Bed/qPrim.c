/**/#include "qDef.h"extern char qtrace;#define TRACE(a,b) if (qtrace>=a) printf bqItem	trueSym;qItem	quoteSym;qItem	novalueSym;Env *	NewEnv(tabSize, symsUnit, strsUnit, consUnit)	{	register Env	*ev;	int				i;		TRACE(2,("NewEnv: tabSize=%d sys=%d strs=%d cons=%d\n",				tabSize, symsUnit, strsUnit, consUnit));		ev = (Env *) NewPtr( (long) sizeof(Env) + sizeof(qItem) * tabSize);	if (ev == 0) OhShit(1, "Env alloc failed\n");		ev->freeList = (qList *)NIL;	ev->eStack = (qList *)NIL;	ev->gensym = 0;	InitSpace(&ev->syms, symsUnit, sizeof(qSymbol) );	InitSpace(&ev->strs, strsUnit, sizeof(Byte) );	InitSpace(&ev->cons, consUnit, sizeof(qList) );		ev->tabSize = tabSize;	for (i = 0; i < tabSize; i++)		{		ev->symTab[i] = (qList *)NIL;		}	return (ev);	}	InitSyms(ev)	Env		*ev;	{			novalueSym = InternSymbol(ev, "\07novalue");	((qSymbol *)novalueSym)->func = novalueSym;	((qSymbol *)novalueSym)->val = novalueSym;					trueSym = InternSymbol(ev, "\01t");	((qSymbol *)trueSym)->val = trueSym;		quoteSym = InternSymbol(ev, "\05quote");	}	long NewSpace();qList *	Cons(ev, a, b)	register Env	*ev;	qItem			a, b;	{	qList			*p;	TRACE(2,("Cons: ev=%lx a=%lx b=%lx\n",ev,a,b));	if (! Listp(ev->freeList))		{		register long	i;		/* Allocate new cons block */		p = (qList *) NewSpace(ev, &ev->cons, ev->cons.allocUnit);				Rplacd(p, NIL);		for (i = 1; i < ev->cons.allocUnit; i++)			{			p++;			Rplacd(p, (qList *)qPtr(p-1, qListBit));			}		ev->freeList = (qList *)qPtr(p, qListBit);		}				p = ev->freeList;	ev->freeList = Cdr(p);	Rplaca(p, a);	Rplacd(p, b);	return ( p);	}	qItem	InternSymbol(ev, str)	Env					*ev;	Byte				*str;	{	register qList		*p;	register qSymbol	*s;	long				hashId;	long				len;		TRACE(2,("InternSymbol: ev=%lx str=%s\n", ev, str+1));		hashId = HashStr(str) % ev->tabSize;	p = ev->symTab[hashId];	for (; Listp(p); p = Cdr(p))		{		s = (qSymbol *) Car(p);		if (strcmp(s->name + 1, str + 1) == 0)			{			return ((qItem)s);			}		}	TRACE(2,("\t new sym=%s hash=%ld\n", str+1, hashId));	/* Alloc symbol */	if (!ev->syms.last 			|| ev->syms.last->nalloced >= ev->syms.allocUnit)		{		NewSpace(ev, &ev->syms, ev->syms.allocUnit);		}		s = (qSymbol *) &ev->syms.last->s;	s += ev->syms.last->nalloced;	ev->syms.last->nalloced += 1;		/* Alloc symbol name string */	len = str[0] + 2;	if (!ev->strs.last 			|| ev->strs.last->nalloced + len >= ev->strs.allocUnit)		{		NewSpace(ev, &ev->strs, ev->strs.allocUnit);		}	s->name = &ev->strs.last->s;	s->name += ev->strs.last->nalloced;	ev->strs.last->nalloced += len;		BlockMove(str, s->name, len);			s->val = novalueSym;	s->plist = (qList *) NIL;	s->func = novalueSym;	s->index = 0;	s->hashId = hashId;	s = (qSymbol *) qPtr(s, qSymbolBit);	ev->symTab[hashId] = (qList *) Cons(ev, s, ev->symTab[hashId]);		return ((qItem) s);	}	qItem	Gensym(ev)	Env		*ev;	{	Byte	buf[MaxStringLen+2];		sprintf(buf+1,"_%ld", ev->gensym);	buf[0] = strlen(buf+1);	ev->gensym += 1;	return (InternSymbol(ev, buf) );	}	qItem	Mapatoms(ev, fp)	Env		*ev;	qItem	fp;	{	register qList	*arg;	register qList	*lp;	register long	i;		arg = Cons(ev, NIL, NIL);	for (i = 0; i < ev->tabSize; i++)		{		for (lp = ev->symTab[i]; Listp(lp); lp = Cdr(lp) )			{			Rplaca(arg, Car(lp));			Apply(ev, fp, arg);			}		}	FreeList(ev, lp);	return (NIL);	}	static InitSpace(spc, allocUnit, isize)	register qSpace	*spc;	int				allocUnit;	int				isize;	{	spc->first = NULL;	spc->last = NULL;	spc->isize = isize;	spc->allocUnit = allocUnit;	}	long	NewSpace(ev, spc, allocUnit)	register Env		*ev;	register qSpace		*spc;	long				allocUnit;	{	qSpaceHdr	*hd;		TRACE(1,("NewSpace: ev=%lx spc=%lx %ld %ld ", ev, spc, spc->isize, allocUnit));		hd = (qSpaceHdr *) NewPtr(allocUnit * spc->isize 								+ sizeof(qSpaceHdr));	TRACE(1,(" @ %lx\n",hd));		if (! hd) OhShit(1,"NewSpace failed %ld %ld\n", 							spc->allocUnit, spc->isize);		hd->next = NULL;	hd->nalloced = 0;		if (spc->last)		spc->last->next = hd;	else		spc->first = hd;			spc->last = hd;		return ((long) &hd->s);	}	int		HashStr(str)	register Byte	*str;	{	register int h = 0;		str++;		for (; *str; str++)		{		h += *str;		}			return (h & ~0x8000);	}	qList *	Nreverse(a)	register qList *a;	{	register qList *b;	register qItem c;		if (! Listp(a))		return (a);			b = Cdr(a);	Rplacd(a, NIL);	while (Listp(b))		{		c = (qItem)Cdr(b);		Rplacd(b, a);		a = b;		b = (qList *)c;		}	return ( a);	}	void	InternIfuncs(ev, fp)	Env				*ev;	register qIfunc	*fp;	{	Byte 	str[MaxStringLen+2];	qSymbol	*sp;		TRACE(2,("InternIfuncs: ev=%lx fp=%lx\n", ev,fp));		fp = (qIfunc *) qPtr(fp, qIfuncType);		for (; fp->func; fp++)		{		strcpy(str+1, fp->name);		str[0] = strlen(str+1);		sp = (qSymbol *)InternSymbol(ev, str);		sp->func = (qItem) fp;		}	}	qList *	Last(lp)	register qList	*lp;	{	register qList	*q;		if (! Listp(lp))		return (lp);			for (; Listp(q = Cdr(lp)) ; lp = q);		return (lp);	}	long	Length(lp)	register qList	*lp;	{	register long	n;		for (n = 0; Listp(lp); lp = Cdr(lp), n++);		return (n);	}	FreeList(ev, lp)	register Env	*ev;	qList			*lp;	{	register qList	*q;		TRACE(2,("FreeList: ev=%lx lp=%lx\n",ev,lp));	if (! Listp(lp))		return;	for (q = lp; Listp(Cdr(q)); q = Cdr(q));	Rplacd(q, ev->freeList);		ev->freeList = lp;	}	void	FreeAll(ev, lp)	register Env	*ev;	register qList	*lp;	{	TRACE(2,("FreeAll: ev=%lx lp=%lx\n",ev,lp));		for (; Listp(lp); lp = Cdr(lp))		{		FreeList(ev, Car(lp));		}	}qList *	List2(ev, a, b)	Env		*ev;	qItem	a;	qItem	b;	{	return ( Cons(ev, a, Cons(ev, b, NIL)) );	}	qList *	Copy(ev, lp)	Env				*ev;	register qList	*lp;	{	register qList	*q;		if (! Listp(lp))		return (lp);			for (q = (qList *)NIL; Listp(lp); lp = Cdr(lp))		{		q = Cons(ev, Copy(ev, Car(lp)), q);		}	return (Nreverse(q));	}	qList *	Nconc(al, bl)	qList	*al;	qList	*bl;	{	register qList	*lp;		lp = Last(al);	if (! Listp(lp))		return (bl);			Rplacd(lp, bl);	return (al);	}	qList	*Getl(sym, tag)	register qSymbol	*sym;	qItem				tag;	{	register qList	*lp;		for (lp = sym->plist; Listp(lp); lp = Cdr(Cdr(lp)) )		{		if (Car(lp) == tag)			return (Cdr (lp));		}			return (lp);	}	qItem	Get(sym, tag)	qSymbol	*sym;	qItem	tag;	{	register qItem it;	if (! Symbolp(sym))		{		printf("Get not symbol %lx\n",sym);		return (NIL);		}					it = (qItem) Getl(sym, tag);		if (Listp(it))		return (Car(it));		return (it);	}	qItem	Put(ev, sym, tag, val)	Env					*ev;	register qSymbol	*sym;	qItem				tag;	qItem				val;	{	register qList	*lp;		TRACE(2,("Put: ev=%lx sym=%lx tag=%lx val=%lx\n",ev,sym,tag,val));	if (! Symbolp(sym))		{		printf("Put not symbol %lx\n",sym);		return (NIL);		}			lp = Getl(sym, tag);		if (! Listp(lp))		{		sym->plist = Cons(ev, tag, 							Cons(ev, val, sym->plist));		}	else {		Rplaca(lp, val);		}	return (val);	}	qItem	ConsCount(ev)	Env		*ev;	{	register qSpaceHdr	*hd;	register long		n;		for (n = 0, hd = ev->cons.first; hd; hd = hd->next, n++);	n = n * ev->cons.allocUnit - Length(ev->freeList);		return ( Numify(n) );	}	qItem	Nth(n, lp)	register long	n;	register qList	*lp;	{	for (; n > 0 && Listp(lp); lp = Cdr(lp), n--);		if (Listp(lp))		return (Car(lp));			return (NIL);	}void	SetList(lp, arg1)	register qList	*lp;	qItem	*arg1;	{	register qItem	**ap = &arg1;		for (; Listp(lp) && *ap != NULL; lp = Cdr(lp), ap++)		{		**ap = Car(lp);		}	}		