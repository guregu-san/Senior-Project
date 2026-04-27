all: serial_count_sort parallel_count_sort

serial_count_sort: count_sort.c
	gcc count_sort.c -o serial_count_sort

parallel_count_sort: count_sort.c
	gcc -fopenmp count_sort.c -o parallel_count_sort

run:
	echo 'Serial:' && ./serial_count_sort && echo '\nParallel:' && ./parallel_count_sort

clean:
	rm serial_count_sort parallel_count_sort