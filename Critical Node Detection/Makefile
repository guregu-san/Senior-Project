all: serial_cndp

serial_cndp: main.c graph.c cnd.c
	gcc main.c graph.c cnd.c -o serial_cndp

run:
	echo 'Serial:' && ./serial_cndp

clean:
	rm serial_cndp log.csv