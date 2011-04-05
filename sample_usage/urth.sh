#!/bin/sh
cd "/home/user/UrbanTerror"
export LD_PRELOAD=$HOME/path/to/urth/build/libURTH.so

# don't change anything below this line
./ioUrbanTerror.i386 $*
unset LD_PRELOAD
exit 0
