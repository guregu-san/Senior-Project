SRC = main.c graph.c cnd.c

SERIAL_BIN = serial_cndp.bin
SERIAL_DBG = serial_cndp_dbg.bin

PARALLEL_BIN = parallel_cndp.bin
PARALLEL_DBG = parallel_cndp_dbg.bin

all: serial_cndp parallel_cndp

serial_cndp: $(SRC)
	gcc $^ -o $(SERIAL_BIN)
	gcc -g $^ -o $(SERIAL_DBG)

parallel_cndp: main.c graph.c threaded_cnd.c
	gcc -fopenmp $^ -o $(PARALLEL_BIN)
	gcc -g -fopenmp $^ -o $(PARALLEL_DBG)

run: $(SERIAL_BIN) $(PARALLEL_BIN)
	@echo 'Serial:' && ./$(SERIAL_BIN) && \
	echo -e '\nParallel:' && ./$(PARALLEL_BIN)

debug_serial: $(SERIAL_DBG)
	gdb $(SERIAL_DBG)

debug_parallel: $(PARALLEL_DBG)
	gdb $(PARALLEL_DBG)

draw: draw.py
	python3 ./draw.py

clean:
	rm *.bin log.csv