all: serial_cndp

run: serial_cndp draw.py
	echo 'Serial:' && ./serial_cndp && python3 ./draw.py

debug_serial: serial_cndp_debug
	gdb serial_cndp_debug

serial_cndp: main.c graph.c cnd.c
	gcc main.c graph.c cnd.c -o serial_cndp && gcc main.c graph.c cnd.c -o serial_cndp_debug -g

clean:
	rm serial* log.csv