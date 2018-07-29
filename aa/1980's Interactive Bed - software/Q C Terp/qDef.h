/**/#ifndef _qDef_#define _qDef_#ifndef _MacTypes_#include "MacTypes.h"#endif#ifndef NULL#define NULL ((long)0)#endif#define MaxStringLen	255typedef long qItem;typedef struct qList	{	qItem			a;	struct qList	*d;	}	qList;	typedef struct qSymbol	{	qItem	val;	qItem	func;	qList	*plist;	Byte	*name;	short	hashId;	short	index;	}	qSymbol;	typedef struct qIfunc	{	qItem	(*func)();	char	*name;	long	argDesc;#define 		qNargsMask		0x000F#define			qNargsBitLen	4#define			qCint1			0x0010#define			qCint2			0x0020#define			qCint3			0x0040#define			qCint4			0x0080#define			qCint5			0x0100#define			qCint6			0x0200#define			qCint7			0x0400#define			qCint8			0x0800#define			qCintLen		8#define			qRetInt			0x1000#define			qRetLong		0x2000#define			qRetQItem		0x4000#define			qConvNum		0x8000#define			qNoEval		   0x10000#define			qVarArg		   0x20000	Byte	*doc;	}	qIfunc;	typedef struct qSpace	{	struct qSpaceHdr	*first;	struct qSpaceHdr	*last;	long	isize;			/* Item size */	long	allocUnit;		/* Number of items to allocate in one block */	}	qSpace;	typedef struct qSpaceHdr	{	struct qSpaceHdr	*next;	long				nalloced;	Byte				s[];	}	qSpaceHdr;	typedef struct Env	{	qList	*freeList;	qList	*eStack;	long	gensym;		qSpace	syms;	qSpace	strs;	qSpace	cons;	int		tabSize;	qList	*symTab[];	}	Env;	#define qSymbolBit		0x80000000#define qListBit		0x40000000#define qOtherBit		0x20000000#define qOtherMask		0x1F000000#define	qIfuncType		(0x01000000 + qOtherBit)#define qIstructType	(0x02000000 + qOtherBit)#define qTypeBitsMask 	0xE0000000#define qTypeBitsLen	3#define qPtr(p, t) 	(( ((long)(p)) & 0x00FFFFFF) | t )#define Numify(x) ((qItem) ( ((long)(x)) & ~qTypeBitsMask) )#define Cnum(x) ((((long)(x)) << qTypeBitsLen) >> qTypeBitsLen)#define NIL		((qItem)(qOtherMask + qOtherBit))#define Type(x) ((long)(x) & 0xFF000000)#define Nump(x) (! ((long)(x) & qTypeBitsMask))#define Nullp(x) ((qItem)(x) == NIL)#define Symbolp(x) ((long)(x) & qSymbolBit)#define Listp(x) ((long)(x) & qListBit)#define Otherp(x) ((long)(x) & qOtherBit)#define Funcp(x) (Listp(x) || (Type(x) == qIfuncType))#define Car(x) (((qList *)(x))->a)#define Cdr(x) (((qList *)(x))->d)#define Cadr(x) (Car(Cdr(x)))#define Rplaca(x, a) Car(x) = (qItem)(a)#define Rplacd(x, d) Cdr(x) = (qList *)(d)Env		*NewEnv();qList *	Cons();qList *	Nreverse();qItem	InternSymbol();qItem	ReadStream(struct Stream *);qItem	ReadStreamList(struct Stream *);qItem	ReadConsole();qItem	ReadFile();qList *	Last();long	Length();qItem	Eval();qItem	Apply();qItem	ApplyIfunc();qItem	ApplyDefun();qList *	List2();qList * Copy();#endif