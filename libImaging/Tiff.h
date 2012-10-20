/*
 * The Python Imaging Library.
 * $Id: //modules/pil/libImaging/Tiff.h#1 $
 *
 * declarations for the LibTiff-based Group3 and Group4 decoder
 *
 */

#include <tiffiop.h>

typedef struct {

    /* PRIVATE CONTEXT (set by decoder) */
    TIFF tiff;
    int count;

} TIFFSTATE;

extern int  ImagingLibTiffInit(ImagingCodecState state, int compression, int fillorder, int count);
extern void ImagingLibTiffCleanup(ImagingCodecState state);

/*
#define VA_ARGS(...)	__VA_ARGS__
#define TRACE(args)    fprintf(stderr, VA_ARGS args)
*/

#define TRACE(args)