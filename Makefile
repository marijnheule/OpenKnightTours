all: okt-trans okt-lfsr

okt-trans: okt-trans.c
	gcc okt-trans.c -O2 -o okt-trans

okt-lfsr: okt-lfsr.c
	gcc okt-lfsr.c -O2 -o okt-lfsr

clean:
	rm okt-trans okt-lfsr
