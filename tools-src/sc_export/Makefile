UNAME = $(shell uname)

DEP_INC = ../depends/include
DEP_LIB = ../depends/lib

ifeq ($(UNAME), Linux)
	SO_EXT = so
	EXE_EXT =
	CFLAG = -fPIC -L/usr/local/lib
	EFLAG = $(CFLAG)
else ifeq ($(UNAME), Darwin)
	SO_EXT = so
	EXE_EXT = 
	CFLAG = -g -fPIC -dynamiclib -Wl,-undefined,dynamic_lookup
	EFLAG = -g -fPIC
else
	SO_EXT = dll
	EXE_EXT = .exe
	CFLAG = -L/usr/local/bin -I$(DEP_INC) -L$(DEP_LIB) -llua52
	EFLAG = $(CFLAG)
endif

all : png pvr epconv scexport ppm lzma.$(SO_EXT) checkep

LZMADEC = \
7zCrc.c \
7zCrcOpt.c \
7zStream.c \
Alloc.c \
Bcj2.c \
Bra.c \
Bra86.c \
BraIA64.c \
CpuArch.c \
Delta.c \
LzFind.c \
Lzma2Dec.c \
Lzma86Dec.c \
LzmaDec.c \
Ppmd7.c \
Ppmd7Dec.c \
Sha256.c \
Xz.c \
XzCrc64.c \
XzDec.c \
XzIn.c

LZMAENC = \
Ppmd7Enc.c \
Lzma86Enc.c \
Lzma2Enc.c \
LzmaEnc.c \
XzEnc.c 

LZMA = $(LZMADEC) $(LZMAENC)
ST_LZMA = -D_7ZIP_ST

lzma.$(SO_EXT) : lzma.c $(addprefix lzma/,$(LZMA))
	gcc $(ST_LZMA) --shared -o $@ -O3 -Wall -Ilzma $^ -I/usr/local/include $(CFLAG)

checkep : checkep.c eploader.c $(addprefix lzma/,$(LZMA))  
	gcc $(ST_LZMA) -O3 -Wall -o $@ $^ -Ilzma

png: png.c
	gcc -g -Wall --shared -o $@.$(SO_EXT) $^ -I/usr/local/include $(CFLAG) -llibpng16-16 -lz

pvr: pvr.c
	gcc -g -Wall --shared -o $@.$(SO_EXT) $^ -I/usr/local/include $(CFLAG)

ktx: ktx.c
	gcc -g -Wall --shared -o $@.$(SO_EXT) $^ -I/usr/local/include $(CFLAG) 

pkm: pkm.c
	gcc -g -Wall --shared -o $@.$(SO_EXT) $^ -I/usr/local/include $(CFLAG) 

ppm : ppmloader.c
	gcc --shared -g -Wall -o $@.$(SO_EXT) $^ -I/usr/local/include $(CFLAG)

epconv : eploader.c
	gcc --shared -DEXPORT_EP -g -Wall -o $@.$(SO_EXT) $^ -I/usr/local/include $(CFLAG)

scexport : scexport.c $(addprefix lzma/,$(LZMADEC))  
	gcc -g -Wall -o $@ $^ $(EFLAG) -Ilzma

test : scexport checkep
	scexport /d/coc2/buildings.sc.byte
	lua epbin.lua /d/coc2/buildings.sc.byte.lua 0
	checkep /d/coc2/buildings.sc.byte.ep

clean :
	rm -f scexport$(EXE_EXT) checkep$(EXE_EXT) *.$(SO_EXT)

install :
	cp scexport$(EXE_EXT) checkep$(EXE_EXT) *.so ../scexport-bin/
