CFLAGS=-Wall -Wextra

BIN=diamond-square

.PHONY: all render clean

all: ${BIN}

render: ${BIN}
	mkdir -p result
	./$^
	./render_steps.sh
	mv *.pgm *.png result/

clean:
	-rm -f ${BIN} result/*
	-rm -fd result/
