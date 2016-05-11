#!/bin/bash
echo PSP-Size by Robert Shilling
echo Please enter .o file to check analyse.
read NAME
psp-objdump -h $NAME | grep text | tee "text.PSF"
psp-objdump -h $NAME | grep data | tee "data.PSF"
./OsizeApp.exe -d
