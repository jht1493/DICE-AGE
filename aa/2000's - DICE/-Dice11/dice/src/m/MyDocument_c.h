/*
 *  MyDocument_c.h
 *  ddoc
 *
 *  Created by jht1 on 8/27/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "jsapi.h"

// ------------------------------------------------------------------------------------------------------
void MyDocument_writeUTF8String( const void * aBuf, size_t len);

// ------------------------------------------------------------------------------------------------------
void MyDocument_GetDocPath( void *docInst, jschar **chars, size_t *length);

// ------------------------------------------------------------------------------------------------------
