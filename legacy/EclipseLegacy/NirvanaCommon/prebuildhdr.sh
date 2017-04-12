#!/bin/bash

cd PCH
mv PCH.h PCH.h.fake
mv PCH.h.real PCH.h
g++ -std=c++0x -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -O0 -g3 -c -x c++-header -fmessage-length=0 -H -MMD -MP  PCH.h -o "PCH.h.gch"
mv PCH.h PCH.h.real
mv PCH.h.fake PCH.h
cd ..

