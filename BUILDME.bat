rem # $Id$
rem # quick windows build (for lazy programmers).  for more
rem # information on the build process, see the README file.
if not "%1" == "clean" goto :build
  python setup.py clean
  erase PIL\*.pyd
:build
python setup.py build_ext -i
rem # upx --best PIL\*.pyd
