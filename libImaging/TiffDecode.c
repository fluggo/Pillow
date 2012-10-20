/*
 * The Python Imaging Library.
 * $Id: //modules/pil/libImaging/TiffDecode.c#1 $
 *
 * LibTiff-based Group3 and Group4 decoder
 *
 */

#include "Imaging.h"

#ifdef HAVE_LIBTIFF

#undef INT32
#undef UINT32

#include "Tiff.h"

#include <stddef.h>

#include <tiffio.h>
#include <tiffiop.h>

extern void _TIFFSetDefaultCompressionState(TIFF* tif);

typedef    void (*TIFFFaxFillFunc)(unsigned char*, uint32*, uint32*, uint32);

typedef struct {
    int     rw_mode;                /* O_RDONLY for decode, else encode */
    int    mode;            /* operating mode */
    uint32    rowbytes;        /* bytes in a decoded scanline */
    uint32    rowpixels;        /* pixels in a scanline */

    uint16    cleanfaxdata;        /* CleanFaxData tag */
    uint32    badfaxrun;        /* BadFaxRun tag */
    uint32    badfaxlines;        /* BadFaxLines tag */
    uint32    groupoptions;        /* Group 3/4 options tag */
    uint32    recvparams;        /* encoded Class 2 session params */
    char*    subaddress;        /* subaddress string */
    uint32    recvtime;        /* time spent receiving (secs) */
    TIFFVGetMethod vgetparent;    /* super-class method */
    TIFFVSetMethod vsetparent;    /* super-class method */
} Fax3BaseState;

typedef struct {
    Fax3BaseState b;
    const unsigned char* bitmap;        /* bit reversal table */
    uint32    data;            /* current i/o byte/word */
    int    bit;            /* current i/o bit in byte */
    int    EOLcnt;            /* count of EOL codes recognized */
    TIFFFaxFillFunc fill;        /* fill routine */
    uint32*    runs;            /* b&w runs for current/previous row */
    uint32*    refruns;        /* runs for reference line */
    uint32*    curruns;        /* runs for current line */
} Fax3DecodeState;


#define DECODER_STATE(tiff) (UINT8 *)(tiff->tif_data + offsetof(Fax3DecodeState, bitmap))
#define DECODER_STATE_SIZE  (sizeof(Fax3DecodeState) - offsetof(Fax3DecodeState, bitmap))


static void
_errorHandler(const char* module, const char* fmt, va_list ap)
{
    /* be quiet */
}

static void
_cleanupLibTiff( TIFF *tiff )
{
    if (tiff->tif_cleanup)
    tiff->tif_cleanup(tiff);

    if (tiff->tif_fields)
    free(tiff->tif_fields);
}

int
ImagingLibTiffInit(ImagingCodecState state, int compression, int fillorder, int count)
{
    TIFF *tiff = &((TIFFSTATE *) state->context)->tiff;
    const TIFFCodec *codec;

    TIFFSetErrorHandler(_errorHandler);
    TIFFSetWarningHandler(_errorHandler);

    codec = TIFFFindCODEC((uint16) compression);
    if (codec == NULL)
    return 0;

    _TIFFmemset(tiff, 0, sizeof (*tiff));
    _TIFFSetDefaultCompressionState(tiff);
    TIFFDefaultDirectory(tiff);

    tiff->tif_mode = O_RDONLY;
    tiff->tif_dir.td_fillorder = (uint16) fillorder;
    tiff->tif_dir.td_compression = (uint16) compression;

    ((TIFFSTATE *) state->context)->count = count;

    if (! codec->init(tiff, 0)) {
    _cleanupLibTiff(tiff);
    return 0;
    }

    return 1;
}

void
ImagingLibTiffCleanup(ImagingCodecState state)
{
    _cleanupLibTiff( &((TIFFSTATE *) state->context)->tiff );
}

int
ImagingLibTiffDecode(Imaging im, ImagingCodecState state, UINT8* buffer, int bytes)
{
    TIFF *tiff = &((TIFFSTATE *) state->context)->tiff;
    int count  = ((TIFFSTATE *) state->context)->count;
    int savecc;
    UINT8 saveds[DECODER_STATE_SIZE];

    if (bytes < count)
    return 0;

    if ((tiff->tif_flags & TIFF_CODERSETUP) == 0) {
    tiff->tif_dir.td_bitspersample = 1;
    tiff->tif_dir.td_imagewidth = state->xsize;
    tiff->tif_scanlinesize = TIFFScanlineSize(tiff);

    if (! tiff->tif_setupdecode(tiff) || ! tiff->tif_predecode(tiff, 0)) {
        state->errcode = IMAGING_CODEC_BROKEN;
        return -1;
    }

    tiff->tif_flags |= TIFF_CODERSETUP;
    }

    tiff->tif_row   = state->y + state->yoff;
    tiff->tif_rawcp = buffer;
    tiff->tif_rawcc = bytes;

    TRACE(("decoding %d bytes, row %d\n", bytes, tiff->tif_row));
    for (;;) {
    if (count < 0) {
        savecc = tiff->tif_rawcc;
        memcpy(saveds, DECODER_STATE(tiff), DECODER_STATE_SIZE);
    }

    if (tiff->tif_decoderow(tiff, (tidata_t) state->buffer,
                tiff->tif_scanlinesize, 0) < 0) {
        TRACE(("decode error, %d bytes left\n", tiff->tif_rawcc));
        if (count < 0)
        break;
        state->errcode = IMAGING_CODEC_BROKEN;
        return -1;
    }

    if (count < 0 && tiff->tif_rawcc <= 0) {
        TRACE(("not enough data\n"));
        break;
    }

    tiff->tif_postdecode(tiff, (tidata_t) buffer, tiff->tif_scanlinesize);

    state->shuffle((UINT8*) im->image[state->y + state->yoff] +
               state->xoff * im->pixelsize, state->buffer,
               state->xsize);

    TRACE(("decoded row %d, %d bytes left\n", tiff->tif_row, tiff->tif_rawcc));
    tiff->tif_row++;
    state->y++;

    if (state->y >= state->ysize) {
        TRACE(("decoded %d rows\n", state->y));
        return -1; /* errcode = 0 */
    }
    }

    memcpy(DECODER_STATE(tiff), saveds, DECODER_STATE_SIZE);

    TRACE(("decoded %d rows, %d bytes left\n", state->y, savecc));
    return bytes - savecc;
}

#endif