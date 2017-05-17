#` pkg-config --cflags --libs gsl `
CC = g++
CFLAGS = -g #-Wall -Werror
GSL = -I/usr/local/include  -L/usr/local/lib -lgsl -lgslcblas -lm
BOOST = -lpthread -lboost_date_time -lboost_iostreams -lboost_thread-mt -lboost_system
VGRD = valgrind -v --tool=memcheck --leak-check=full #--track-origins=yes
TZ = ./bin/Tokenizer.o
RE = ./bin/RandomEngine.o
BS = ./bin/BlackScholes.o

fsmt:
	$(CC) $(CFLAGS) src/analysis/fsm-trng.cpp -o ./bin/fsm-trng $(BOOST) $(TZ) $(GSL)
	./bin/fsm-trng

fsmp:
	$(CC) $(CFLAGS) src/analysis/fsm-predict.cpp -o ./bin/fsm-predict $(BOOST) $(TZ)
	./bin/fsm-predict

hcor:
	$(CC) $(CFLAGS) src/analysis/hsi-correl.cpp -o ./bin/hsi-correl $(BOOST) $(TZ) $(GSL)
	./bin/hsi-correl

opt:
	$(CC) $(CFLAGS) src/analysis/optzn.cpp -o ./bin/optzn $(BOOST) $(TZ) $(GSL)
	#./bin/optzn

hi:
	$(CC) $(CFLAGS) src/analysis/hsi-intd.cpp -o ./bin/hsi-intd $(BOOST) $(TZ) $(GSL)
	./bin/hsi-intd

hd:
	$(CC) $(CFLAGS) src/analysis/hsi-day.cpp -o ./bin/hsi-day $(BOOST) $(TZ) $(GSL)
	./bin/hsi-day

bah:
	$(CC) $(CFLAGS) src/analysis/buyandhold.cpp -o ./bin/buyandhold $(GSL) $(TZ)
	./bin/buyandhold

dcab:
	$(CC) $(CFLAGS) src/analysis/dlrcstavgbt.cpp -o ./bin/dlrcstavgbt $(GSL) $(TZ) $(BOOST)
	./bin/dlrcstavgbt

fupe:
	$(CC) $(CFLAGS) src/analysis/fundlpe.cpp -o ./bin/fundlpe $(GSL) $(TZ)
	./bin/fundlpe

sp:
	$(CC) $(CFLAGS) src/analysis/sgnlprc.cpp -o ./bin/sgnlprc $(BOOST) $(TZ) $(GSL)
	./bin/sgnlprc

cb:
	$(CC) $(CFLAGS) src/analysis/com./binebars.cpp -o ./bin/com./binebars $(BOOST) $(TZ) $(GSL)
	./bin/com./binebars

tz:
	$(CC) $(CFLAGS) -o $(TZ) -c src/corelib/Tokenizer.cpp

slb:
	$(CC) $(CFLAGS) src/unittests/ut-slb.cpp -o ./bin/ut-slb
	$(VGRD) ./bin/ut-slb

re:
	$(CC) $(CFLAGS) -o $(RE) -c src/smath/RandomEngine.cpp
	$(CC) $(CFLAGS) src/unittests/ut-re.cpp $(RE) -o ./bin/ut-re
	#$(VGRD) ./bin/ut-re
	./bin/ut-re

hl:
	$(CC) $(CFLAGS) src/unittests/ut-hl.cpp -o ./bin/ut-hl
	$(VGRD) ./bin/ut-hl

sma:
	$(CC) $(CFLAGS) src/unittests/ut-sma.cpp -o ./bin/ut-sma
	$(VGRD) ./bin/ut-sma

ema:
	$(CC) $(CFLAGS) src/unittests/ut-ema.cpp -o ./bin/ut-ema
	$(VGRD) ./bin/ut-ema

wma:
	$(CC) $(CFLAGS) src/unittests/ut-wma.cpp -o ./bin/ut-wma
	$(VGRD) ./bin/ut-wma

atr:
	$(CC) $(CFLAGS) src/unittests/ut-atr.cpp -o ./bin/ut-atr
	$(VGRD) ./bin/ut-atr

sm1:
	$(CC) $(CFLAGS) src/unittests/ut-sm1.cpp $(RE) -o ./bin/ut-sm1 $(BOOST) $(TZ)
	$(VGRD) ./bin/ut-sm1

spraw:
	$(CC) $(CFLAGS) src/unittests/ut-sp.cpp -o ./bin/ut-sp $(BOOST) $(TZ)
	$(VGRD) ./bin/ut-sp

ts:
	$(CC) $(CFLAGS) src/unittests/ut-ts.cpp $(RE) -o ./bin/ut-ts
	$(VGRD) ./bin/ut-ts

wr:
	$(CC) $(CFLAGS) src/unittests/ut-wr.cpp -o ./bin/ut-wr
	$(VGRD) ./bin/ut-wr

lr:
	$(CC) $(CFLAGS) src/unittests/ut-lr.cpp -o ./bin/ut-lr $(GSL)
	$(VGRD) ./bin/ut-lr

snys:
	$(CC) $(CFLAGS) src/unittests/ut-snys.cpp $(RE) -o ./bin/ut-snys $(LBDT) $(GSL)
	$(VGRD) ./bin/ut-snys

cdq:
	$(CC) $(CFLAGS) src/unittests/ut-cdq.cpp $(RE) -o ./bin/ut-cdq $(BOOST)
	$(VGRD) ./bin/ut-cdq

dlyq:
	$(CC) $(CFLAGS) src/unittests/ut-dlyq.cpp $(RE) -o ./bin/ut-dlyq $(BOOST)
	$(VGRD) ./bin/ut-dlyq

hkfe:
	$(CC) $(CFLAGS) src/unittests/ut-hkfe.cpp $(TZ) -o ./bin/ut-hkfe
	$(VGRD) ./bin/ut-hkfe

vs:
	$(CC) $(CFLAGS) src/unittests/ut-vs.cpp $(TZ) -o ./bin/ut-vs
	$(VGRD) ./bin/ut-vs

st:
	$(CC) $(CFLAGS) src/unittests/ut-st.cpp $(TZ) -o ./bin/ut-st $(BOOST)
	$(VGRD) ./bin/ut-st

sdt:
	$(CC) $(CFLAGS) src/unittests/ut-sdt.cpp $(TZ) -o ./bin/ut-sdt
	$(VGRD) ./bin/ut-sdt

ac:
	$(CC) $(CFLAGS) src/unittests/ut-ac.cpp $(TZ) -o ./bin/ut-ac $(BOOST) $(BS) $(GSL)
	$(VGRD) ./bin/ut-ac

gs:
	$(CC) $(CFLAGS) src/unittests/ut-gs.cpp src/broker/PracticalSocket.cpp -o ./bin/ut-gs
	./bin/ut-gs

b:
	$(CC) $(CFLAGS) src/unittests/ut-b.cpp -o ./bin/ut-b
	$(VGRD) ./bin/ut-b

bp:
	$(CC) $(CFLAGS) src/unittests/ut-bp.cpp $(TZ) -o ./bin/ut-bp $(BOOST)
	$(VGRD) ./bin/ut-bp

bs:
	$(CC) $(CFLAGS) -o $(BS) -c src/smath/BlackScholes.cpp $(GSL)
	$(CC) $(CFLAGS) $(BS) src/unittests/ut-bs.cpp -o ./bin/ut-bs $(GSL)
	$(VGRD) ./bin/ut-bs

obs:
	$(CC) $(CFLAGS) src/unittests/ut-obs.cpp -o ./bin/ut-obs
	$(VGRD) ./bin/ut-obs

bisec:
	$(CC) $(CFLAGS) src/unittests/ut-bisec.cpp -o ./bin/ut-bisec
	$(VGRD) ./bin/ut-bisec

mrtg:
	$(CC) $(CFLAGS) src/unittests/ut-mrtg.cpp -o ./bin/ut-mrtg
	$(VGRD) ./bin/ut-mrtg

all:
	make sdt
	make tz
	make bp
	make slb
	make re
	make hl
	make sma
	make ema
	make atr
	make sm1
	make spraw
	make ts
	make wr
	make lr
	make snys
	make cdq
	make dlyq
	make hkfe
	make st
	make bs
	make ac
	make gs
	make bisec
	make mrtg


clean:
	rm -f ./bin/*
