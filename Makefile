

CFLAGS = -g -I/usr/pkg/include -I.. -pg 
LDFLAGS = -L/usr/pkg/lib -Wl,-rpath,/usr/pkg/lib -lnetpbm -lm -pg

OBJ = compile.o node.o dict.o symbol.o list.o exec.o env.o alloc.o misc.o math.o func.o imgf.o array.o

all: ppmscript

test: test.o $(OBJ)
	cc -o test test.o $(OBJ) -lm

ppmscript: ppmscript.o $(OBJ)
	cc -o ppmscript ppmscript.o $(OBJ) $(LDFLAGS)

pixsh: pixsh.o $(OBJ)
	cc -o pixsh pixsh.o $(OBJ) -lm


clean:
	rm -f $(OBJ) *.o ppmscript pixsh test


math.c: mkmath.pl
	perl mkmath.pl > math.c

install: ppmscript
	cp ppmscript /usr/local/bin

