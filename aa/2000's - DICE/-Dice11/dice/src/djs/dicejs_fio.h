/* History:

Copyright � 2003 John Henry Thompson. All rights reserved.

2003.03.15	jht	Created. Integrating JavaScript.

*/

#pragma once

#include <stdio.h>

#include "jstypes.h"

JS_BEGIN_EXTERN_C

#if 0
#include "CTextEditViewForScript.h"
void dicejs_setTextEditView( CTextEditViewForScript* tev);
#endif

int dicejs_fprintf(FILE *aFile, const char *aStr, ...);

int dicejs_fputs(const char *aStr, FILE *aFile);

size_t dicejs_fwrite(const void * aBuf , size_t memb_size, size_t num_memb, FILE * aFile );

int dicejs_fputc(int aChar, FILE *  aFile);

JS_END_EXTERN_C
