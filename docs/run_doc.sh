#!/bin/bash

if [ -d docBin ]; then
    rm -rf docBin
fi

sed -e 's/#define.*DLL_PUBLIC.*//g' -e 's/DLL_PUBLIC //g' ../library/include/rocfft.h > rocfft.h
doxygen Doxyfile

cd source
make clean
make html
cd ..

rm rocfft.h

