# Copyright (c) 2012 Hajime Nakagami<nakagami@gmail.com>
# All rights reserved. Licensed under the standard PIL license, see README.rst

import unittest
from PIL import Image
import PIL.ArgImagePlugin
import PIL.BmpImagePlugin           # test_bmp
import PIL.BufrStubImagePlugin
import PIL.CurImagePlugin
import PIL.DcxImagePlugin           # test_dcx
import PIL.EpsImagePlugin           # test_eps
import PIL.FitsStubImagePlugin      # test_fits
import PIL.FliImagePlugin
import PIL.FpxImagePlugin
import PIL.GbrImagePlugin
import PIL.GifImagePlugin           # test_gif
import PIL.GribStubImagePlugin
import PIL.Hdf5StubImagePlugin
import PIL.IcnsImagePlugin
import PIL.IcoImagePlugin           # test_ico
import PIL.ImImagePlugin
import PIL.ImtImagePlugin
import PIL.IptcImagePlugin
import PIL.JpegImagePlugin          # test_jpeg
import PIL.McIdasImagePlugin
import PIL.MicImagePlugin
import PIL.MpegImagePlugin
import PIL.MspImagePlugin
import PIL.PcdImagePlugin           # test_pcd
import PIL.PcxImagePlugin           # test_pcx
import PIL.PixarImagePlugin
import PIL.PngImagePlugin           # test_png
import PIL.PpmImagePlugin           # test_ppm
import PIL.PsdImagePlugin           # test_psd
import PIL.SgiImagePlugin           # test_sgi
import PIL.SpiderImagePlugin
import PIL.SunImagePlugin           # test_sun
import PIL.TgaImagePlugin
import PIL.TiffImagePlugin          # test_tiff
import PIL.WmfImagePlugin
import PIL.XVThumbImagePlugin
import PIL.XbmImagePlugin           # test_xbm
import PIL.XpmImagePlugin           # test_xpm


class PluginTest(unittest.TestCase):
    def _test_open(self, factory, accept, filename):
        fp = open(filename, 'rb')
        if accept:
            self.assertTrue(accept(fp.read(16)))
        fp.seek(0)
        im = factory(fp, filename)
        self.assertTrue(isinstance(im, factory))
#        im.load()
        fp.close()

    def test_bmp(self):
        self._test_open(PIL.BmpImagePlugin.BmpImageFile,
                        PIL.BmpImagePlugin._accept, './Images/lena.bmp')

    def test_dcx(self):
        self._test_open(PIL.DcxImagePlugin.DcxImageFile,
                        PIL.DcxImagePlugin._accept, './Images/lena.dcx')

    def test_eps(self):
        self._test_open(PIL.EpsImagePlugin.EpsImageFile,
                        PIL.EpsImagePlugin._accept, './Images/lena.eps')

    def test_fits(self):
        self._test_open(PIL.FitsStubImagePlugin.FITSStubImageFile,
                        PIL.FitsStubImagePlugin._accept, './Images/lena.fits')

    def test_gif(self):
        self._test_open(PIL.GifImagePlugin.GifImageFile,
                        PIL.GifImagePlugin._accept, './Images/lena.gif')

    def test_ico(self):
        self._test_open(PIL.IcoImagePlugin.IcoImageFile,
                        PIL.IcoImagePlugin._accept, './Images/lena.ico')

    def test_jpeg(self):
        self._test_open(PIL.JpegImagePlugin.JpegImageFile,
                        PIL.JpegImagePlugin._accept, './Images/lena.jpg')

    def test_pcd(self):
        self._test_open(PIL.PcdImagePlugin.PcdImageFile,
                        None, './Images/lena.pcd')

    def test_pcx(self):
        self._test_open(PIL.PcxImagePlugin.PcxImageFile,
                        PIL.PcxImagePlugin._accept, './Images/lena.pcx')

    def test_png(self):
        self._test_open(PIL.PngImagePlugin.PngImageFile,
                        PIL.PngImagePlugin._accept, './Images/lena.png')
        self._test_open(PIL.PngImagePlugin.PngImageFile,
                    PIL.PngImagePlugin._accept, './Images/solid_red_alpha.png')

    def test_ppm(self):
        self._test_open(PIL.PpmImagePlugin.PpmImageFile,
                        PIL.PpmImagePlugin._accept, './Images/lena.ppm')

    def test_psd(self):
        self._test_open(PIL.PsdImagePlugin.PsdImageFile,
                        PIL.PsdImagePlugin._accept, './Images/lena.psd')

    def test_sgi(self):
        self._test_open(PIL.SgiImagePlugin.SgiImageFile,
                        PIL.SgiImagePlugin._accept, './Images/lena.sgi')

    def test_sun(self):
        self._test_open(PIL.SunImagePlugin.SunImageFile,
                        PIL.SunImagePlugin._accept, './Images/lena.sun')

    def test_tiff(self):
        self._test_open(PIL.TiffImagePlugin.TiffImageFile,
                        PIL.TiffImagePlugin._accept, './Images/lena.tiff')

    def test_xbm(self):
        self._test_open(PIL.XbmImagePlugin.XbmImageFile,
                        PIL.XbmImagePlugin._accept, './Images/lena.xbm')

    def test_xpm(self):
        self._test_open(PIL.XpmImagePlugin.XpmImageFile,
                        PIL.XpmImagePlugin._accept, './Images/simple.xpm')
        # still not support
        #self._test_open(PIL.XpmImagePlugin.XpmImageFile,
        #                PIL.XpmImagePlugin._accept, './Images/lena.xpm')

if __name__ == '__main__':
    unittest.main()
