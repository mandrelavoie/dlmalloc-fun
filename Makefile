.PHONY: all run cutter clean

DLMALLOC=dlmalloc

MAKE=make
CC=gcc
LDFLAGS=-L ${DLMALLOC}/lib -l:libmalloc.a -Wl,-z,norelro

CUTTER=cutter
CUTTERARGS=-A 1

PYTHON=python3
PAYLOAD=payload.py

OUTDIR=out
OUTFILE=${OUTDIR}/main

${OUTDIR}:
	mkdir -p $@

${DLMALLOC}/lib/libmalloc.a:
	${MAKE} -C ${DLMALLOC} all

${OUTFILE}: main.c ${DLMALLOC}/lib/libmalloc.a ${OUTDIR}
	${CC} $< -o ${OUTFILE} ${LDFLAGS}

all: ${OUTFILE}

# Disable aslr for that process
run: ${OUTFILE}
	@setarch $(uname -m) -R ${OUTFILE} $(shell ${PYTHON} ${PAYLOAD})

cutter: ${OUTFILE}
	${CUTTER} $< ${CUTTERARGS}

clean:
	${MAKE} -C ${DLMALLOC} clean
	rm -rf ${OUTDIR}