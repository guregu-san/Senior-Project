all: serial_count_sort parallel_count_sort

run: serial_count_sort parallel_count_sort
	echo 'Serial:' && ./serial_count_sort && echo -e '\nParallel:' && ./parallel_count_sort

debug_serial: serial_count_sort_debug
	gdb serial_count_sort_debug

debug_parallel: parallel_count_sort_debug
	gdb parallel_count_sort_debug

serial_count_sort: count_sort.c
	gcc count_sort.c -o serial_count_sort && gcc count_sort.c -o serial_count_sort_debug -g

parallel_count_sort: count_sort.c
	gcc -fopenmp count_sort.c -o parallel_count_sort && gcc -fopenmp count_sort.c -o parallel_count_sort_debug -g

clean:
	rm *_count*