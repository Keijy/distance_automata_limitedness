TESTS=test_automate

CPPFLAGS=-g -O0 -Wall -Werror
CFLAGS=
LDFLAGS= -lm

all: $(TESTS) 

test_automate: test_automate.o libautomate.a

libautomate.a: libautomate.a(automate.o table.o ensemble.o avl.o fifo.o outils.o Matrice.o arbresyntaxique.o)

clean:
	-rm -rf *.o
	-rm -rf *.a
	-rm -rf $(TESTS)
	-rm -rf *~

.PHONY: all clean
