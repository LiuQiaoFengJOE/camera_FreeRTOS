/*------------------------------------------------------------------------
 *  for platfrom port 
 *------------------------------------------------------------------------*/
#ifndef _ZBAR_CFG_H_
#define _ZBAR_CFG_H_


/* whether to build support for Code 128 symbology */
#define ENABLE_CODE128 1
/* whether to build support for Code 39 symbology */
#define ENABLE_CODE39 1
/* whether to build support for EAN symbologies */
#define ENABLE_EAN 1
/* whether to build support for Interleaved 2 of 5 symbology */
#define ENABLE_I25 1
/* whether to build support for PDF417 symbology */
#define ENABLE_PDF417	0
/* whether to build support for QR Code */
#define ENABLE_QRCODE 1



/* Program major version (before the '.') as a number */
#define ZBAR_VERSION_MAJOR 0
/* Program minor version (after '.') as a number */
#define ZBAR_VERSION_MINOR 10


#include "typedef.h"
//#include "../../../hal/inc/hal.h"
#include "platform_if.h"

#endif


