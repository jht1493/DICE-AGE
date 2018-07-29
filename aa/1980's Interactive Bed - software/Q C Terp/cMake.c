/**/#include "cGlob.h"/* ---------------------------------------------------------------------- */extern cEnv			ce;char *AddSpaceBlock();/* ------------------ cMakeTerm ----------------------------------------- */Ent	*cMakeTerm(term)	short		term;	{	Bool		new;	uchar		tm[1];		TRACE(1,("MakeTerm: %c\n", term));		tm[0] = term;	return (cIntern(ce.idTab, 1, &tm, sizeof(IdEnt), &new) );	}/* ------------------ cMakeSymbolOrKeyWord ----------------------------------------- */Ent	*cMakeSymbolOrKeyWord(n, str)	short		n;	char		*str;	{	Ent			*en;	Bool		new;		str[n] = 0;	TRACE(1,("MakeSymbol: %s\n", str));		en = cIntern(ce.idTab, n, str, sizeof(IdEnt), &new);	if (new)		{		en->idEnt.et = IdEntType;		en->idEnt.def = NULL;		}			return (en);	}/* --------------------------------------------------------------------- *//* ------------------ cMakeStringConst ----------------------------------------- */Ent *cMakeStringConst(n, str)	short		n;	char		*str;	{	extern PtrTypeEnt	stypeString;		str[n] = 0;	TRACE(1,("MakeString: %s\n", str));	return ( cInternConst( (Ent *)&stypeString, n+1, str) );	}/* ------------------ cMakeCharConst ----------------------------------------- */Ent *cMakeCharConst(val)	char		val;	{	extern SimpleTypeEnt	stypeChar;		TRACE(1,("MakeCharConst: %d\n", val));	return ( cInternConst(&stypeChar, 1, (Name)&val) );	}	/* ------------------ cMakeShortConst ----------------------------------------- */Ent *cMakeShortConst(val)	short		val;	{	extern SimpleTypeEnt	stypeShortInt;		TRACE(1,("MakeShortConst: %d\n", val));	return ( cInternConst(&stypeShortInt, 2, (Name)&val) );	}	/* ------------------ cMakeUnsignedShortConst ----------------------------------------- */Ent *cMakeUnsignedShortConst(val)	short		val;	{	extern SimpleTypeEnt	stypeUnsignedShortInt;		TRACE(1,("MakeUnShortConst: %d\n", val));	return ( cInternConst(&stypeUnsignedShortInt, 2, (Name)&val) );	}/* ------------------ cMakeLongConst ----------------------------------------- */	Ent *cMakeLongConst(val)	long	val;	{	extern SimpleTypeEnt	stypeLongInt;		TRACE(1,("MakeLongConst: %ld\n", val));		return ( cInternConst(&stypeLongInt, 4, (Name)&val) );	}	/* ------------------ cMakeUnsignedLongConst ----------------------------------------- */	Ent *cMakeUnsignedLongConst(val)	long	val;	{	extern SimpleTypeEnt	stypeUnsignedLongInt;		TRACE(1,("MakeUnsignedLongConst: %ld\n", val));		return ( cInternConst(&stypeUnsignedLongInt, 4, (Name)&val) );	}/* ------------------ cMakeDoubleConst ----------------------------------------- */	Ent *cMakeDoubleConst(n, dstr)	short		n;	char		*dstr;	{	extern SimpleTypeEnt	stypeDouble;		dstr[n] = 0;	TRACE(1,("MakeDoubleConst: %s\n", dstr));	return ( cInternConst(&stypeLongInt, n, (Name)dstr) );	}/* ------------------ HashStr ------------------------------------------ */static short HashStr(len, str)	short		len;	Name		str;	{	short		h = 0;		for (; len > 0; str++, len--)		{		h += *str;		}			return (h & 0x7FFF);	}	/* ------------------ StrEq ------------------------------------------ */static Bool StrEq(len, str1, str2)	short		len;	Name		str1;	Name		str2;	{	for (; len > 0; len--, str1++, str2++)		{		if (*str1 != *str2) return (0);		}	return (1);	}/* -------------------- cIntern -------------------------------------------- */Ent *cIntern(htab, len, name, entSize, new)	HashTab		*htab;	short		len;	Name		name;			short		entSize;	Bool		*new;	{	short		hi;	cList		*cl;	Ent			*en;	Name		n2;	TRACE(2,("cIntern: htab=%lx len=%d name=%lx entSize=%d new=%lx\n",				htab,len,name,entSize,new));			*new = 0;	hi = HashStr(len, name) % htab->tabSize;	cl = htab->tab[hi];	for (; cl ; cl = cl->next)		{		en = cl->ent;		n2 = en->tabEnt.name;		if (n2[0] == len && StrEq(len, name, n2+1) )			return (en);		}			/* Allocate name string */			*new = 1;	n2 = (Name) cAllocInSpace(&ce.objSpace, (long) len + 2);	if (! n2) return (NULL);		n2[0] = len;	BlockMove(name, n2+1, (long) len);	n2[len+1] = 0;		/* Place ent in obj space */	en = (Ent *) cAllocInSpace(&ce.objSpace, (long) entSize);	if (! en) return (NULL);		en->tabEnt.name = n2;		/* Link into hash table */	cl = cNewObj(cList);	if (! cl) return (en);		cl->ent = en;	cl->next = htab->tab[hi];	htab->tab[hi] = cl;		return (en);	}/* -------------------- cInternConst --------------------------------------- */Ent *cInternConst(type, len, name)	TypeEnt		*type;	short		len;	Name		name;	{	HashTab		*htab = ce.constTab;	short		hi;	cList		*cl;	Ent			*en;	Name		n2;	hi = HashStr(len, name) % htab->tabSize;	cl = htab->tab[hi];	for (; cl ; cl = cl->next)		{		en = cl->ent;		n2 = en->constEnt.name;		if (en->constEnt.type == type 			&& en->constEnt.size == len && StrEq(len, name, en->constEnt.name) )			return (en);		}			/* Allocate constant string */	n2 = (Name) cAllocInSpace(&ce.objSpace, (long) len);	if (! n2) return (NULL);		BlockMove(name, n2, (long) len);		/* Place const in obj space */	en = (Ent *) cAllocInSpace(&ce.objSpace, (long) sizeof(ConstEnt));	if (! en) return (NULL);		en->constEnt.et = ConstEntType;	en->constEnt.name = n2;	en->constEnt.type = type;	en->constEnt.size = len;		/* Link into hash table */	cl = cNewObj(cList);	if (! cl) return (en);		cl->ent = en;	cl->next = htab->tab[hi];	htab->tab[hi] = cl;		return (en);	}/* ------------------ cInternByEt ------------------------------------------ */Ent *cInternByEt(htab, et, name, entSize, new)	HashTab		*htab;	EntType		et;	Ent			*name;	short		entSize;	Bool		*new;	{	short		hi;	cList		*cl;	Ent			*en;		TRACE(2,("cInternByEt: htab=%lx et=%d name=%lx entSize=%d new=%lx\n",				htab,et,name,entSize,new));		*new = 0;	hi = ((short) name) % htab->tabSize;	cl = htab->tab[hi];	for (; cl ; cl = cl->next)		{		en = cl->ent;		if (en->tabEnt.name == name && en->tabEnt.et == et)			return (en);		}		/* Dont alloc if entsize zero */	if (entSize < 1) return (NULL);			/* Allocate ent object */	*new = 1;	en = (Ent *) cAllocInSpace(&ce.objSpace, (long) entSize);	if (! en) return (NULL);		en->tabEnt.name = name;	en->tabEnt.et = et;		/* Link into hash table */	cl = cNewObj(cList);	if (! cl) return (NULL);		cl->ent = en;	cl->next = htab->tab[hi];	htab->tab[hi] = cl;		return (en);	}	/* ---------------------- cSpaceInit ----------------------------------------- */cSpaceInit(cspace, allocSize)	cSpace		*cspace;	long		allocSize;	{	cspace->first = NULL;	cspace->last = NULL;	cspace->allocSize = allocSize;	AddSpaceBlock(cspace);	}/* ----------------------- AddSpaceBlock----------------------------------- */static char *AddSpaceBlock(cspace)	cSpace		*cspace;	{	cSpaceHdr	*hd;	long		sz;		hd = (cSpaceHdr *) NewPtr(sz = cspace->allocSize + sizeof(cSpaceHdr));		if (! hd)		{		cMemError("AddSpaceBock", sz);		return (NULL);		}			hd->next = NULL;	hd->nalloc = 0;	hd->allocSize = cspace->allocSize;		if (cspace->last)		cspace->last->next = hd;	else		cspace->first = hd;			cspace->last = hd;		return (&hd->s[0]);	}	/* ----------------------- cAllocInSpace ----------------------------------- */char *cAllocInSpace(cspace, len)	cSpace		*cspace;	long		len;	{	cSpaceHdr	*hd = cspace->last;	char		*cp;		TRACE(2,("cAllocInSpace: cspace=%lx len=%ld\n",cspace,len));		len = (len + 1) & ~1;	/* Make even, for minimal alignment */		if (hd->nalloc + len > hd->allocSize)		{		if (len > cspace->allocSize)			cFatalError("cAllocInSpace: len=%ld is bigger than allocSize\n",len);					if (! AddSpaceBlock(cspace))			return (NULL);		hd = cspace->last;		}			cp = hd->s + hd->nalloc;	hd->nalloc += len;		return (cp);	}	/* ----------------------- cTabInit ---------------------------------------- */HashTab *cTabInit(tabSize)	long		tabSize;	{	HashTab		*ht;	long		i, sz;		ht = (HashTab *) NewPtr( sz = (long) sizeof(HashTab) 								+ (long)sizeof(cList *) * tabSize);									if (! ht)		{		cMemError("cTabInit", sz);		return (NULL);		}			ht->tabSize = tabSize;	for (i = 0; i < tabSize; i++)		ht->tab[i] = NULL;			return (ht);	}/* ---------------------------------------------------------------------------- */#define IdTabSize		((long)1009)#define AuxTabSize		((long)1009)#define ConstTabSize	((long)1009)#define ObjSpaceSize	((long)4000)/* ------------------------ cEnvInit ---------------------------------------- */static int initDone = 0;cEnvInit()	{	if (initDone) return;	initDone = 1;	ce.idTab = cTabInit(IdTabSize);	ce.auxTab = cTabInit(AuxTabSize);	ce.constTab = cTabInit(ConstTabSize);	cSpaceInit(&ce.objSpace, ObjSpaceSize);		cKeyInit();	}/* ---------------------------------------------------------------------------- */cEnvClose()	{	cSpaceHdr	*sp, *nsp;		if (! initDone)		return;	DisposPtr(ce.idTab);	DisposPtr(ce.auxTab);	DisposPtr(ce.constTab);		for (sp = ce.objSpace.first; sp; sp = nsp)		{		nsp = sp->next;		DisposPtr(sp);		}		initDone = 0;	}	/* ---------------------------------------------------------------------------- */		