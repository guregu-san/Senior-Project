SRC = main.c graph.c cnd.c

SERIAL_BIN = serial_cndp.bin
SERIAL_DBG = serial_cndp_dbg.bin

all: serial_cndp

serial_cndp: $(SRC)
	gcc $^ -o $(SERIAL_BIN)
	gcc -g $^ -o $(SERIAL_DBG)

run: $(SERIAL_BIN)
	@echo 'Serial:' && ./$(SERIAL_BIN)

debug_serial: $(SERIAL_DBG)
	gdb $(SERIAL_DBG)

draw: draw.py
	python3 ./draw.py

clean:
	rm *.bin log.csv