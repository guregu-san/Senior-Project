SRC = count_sort.c

SERIAL_BIN = serial_count_sort.bin
SERIAL_DBG = serial_count_sort_dbg.bin

PARALLEL_BIN = parallel_count_sort.bin
PARALLEL_DBG = parallel_count_sort_dbg.bin

all: serial_count_sort parallel_count_sort

serial_count_sort: $(SRC)
	gcc $^ -o $(SERIAL_BIN)
	gcc -g $^ -o $(SERIAL_DBG)

parallel_count_sort: $(SRC)
	gcc -fopenmp $^ -o $(PARALLEL_BIN)
	gcc -g -fopenmp $^ -o $(PARALLEL_DBG)

run: $(SERIAL_BIN) $(PARALLEL_BIN)
	@echo 'Serial:' && ./$(SERIAL_BIN) && \
	echo -e '\nParallel:' && ./$(PARALLEL_BIN)

debug_serial: (SERIAL_DBG)
	gdb $^

debug_parallel: $(PARALLEL_DBG)
	gdb $^

clean:
	rm *.bin