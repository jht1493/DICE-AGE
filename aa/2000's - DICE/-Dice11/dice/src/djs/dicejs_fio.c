/* History:

Copyright © 2003 John Henry Thompson. All rights reserved.

2003.03.20	jht	Created. Integrating JavaScript.

*/

#include "dicejs_fio.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "jsapi.h"

//#import <Carbon/Carbon.h>
//#import <Foundation/Foundation.h>
#import "MyDocument_c.h"
//extern void MyDocument_writeUTF8String( const void * aBuf, size_t len);

#if 0
static CTextEditViewForScript*	textEditForIO = 0;

// --------------------------------------------------------------------------------
// Careful here. Is called before JS inited.
//
void dicejs_setTextEditView( CTextEditViewForScript* tev)
{
	textEditForIO = tev;
}
#endif

#if 1

// --------------------------------------------------------------------------------
// !!@ 2003.03.21 jht: is this needed??
static void translateLFtoCR(char *str, int length)
{
    char *limit = str + length;
    while (str != limit) {
        if (*str == '\n')
            *str = '\r';
        str++;
    }
}

// --------------------------------------------------------------------------------
int dicejs_fprintf(FILE *file, const char *format, ...)
{
	// 2003.03.20 jht: Copped from js.c 
    va_list args;
    char smallBuffer[4096];
    int n;
    int bufferSize = sizeof smallBuffer;
    char *buffer = smallBuffer;
    int result;

    va_start(args, format);
    n = vsnprintf(buffer, bufferSize, format, args);
    va_end(args);
    while (n < 0) {
        if (buffer != smallBuffer)
            free(buffer);
        bufferSize <<= 1;
        buffer = (char *)malloc(bufferSize);
        if (!buffer) {
            //JS_ASSERT(JS_FALSE);
            return 0;
        }
        va_start(args, format);
        n = vsnprintf(buffer, bufferSize, format, args);
        va_end(args);
    }
    translateLFtoCR(buffer, n);
    result = dicejs_fwrite(buffer, 1, n, file);	// 2003.03.20 jht
    if (buffer != smallBuffer)
        free(buffer);
    return result;
}

// --------------------------------------------------------------------------------
int dicejs_fputs(const char *aStr, FILE *aFile)
{
	dicejs_fwrite( aStr, 1, strlen( aStr), aFile );
	return 0;
}

// --------------------------------------------------------------------------------
size_t dicejs_fwrite(const void * aBuf , size_t memb_size, size_t num_memb, FILE * aFile )
{
	MyDocument_writeUTF8String( aBuf, memb_size*num_memb );
	return 0;
}

// --------------------------------------------------------------------------------
int dicejs_fputc(int aChar, FILE *  aFile)
{
	dicejs_fprintf(aFile, "%c", aChar);
	
	return 0;
}
#endif

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
