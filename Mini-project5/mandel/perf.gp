set terminal postscript color
set output "perf.ps"
set style line 1 linetype 2
set style line 2 linetype 3
set style line 3 linetype 1

set boxwidth 0.5
set style fill solid

set nokey
set xrange [-0.5:15.5]
set xtics 0,1,15
    
set title "Performance of MPI Mandelbrot set computation"
set xlabel "MPI Process (rank)"
set ylabel "Local computation time (ms)"

set key on
plot	'perf.data' index 0 using 2:3 title "MPI Size = 1 process" with boxes, \
	'perf.data' index 1 using 2:3 title "MPI Size = 2 processes" with boxes, \
	'perf.data' index 2 using 2:3 title "MPI Size = 4 processes" with boxes, \
	'perf.data' index 3 using 2:3 title "MPI Size = 8 processes" with boxes,  \
	'perf.data' index 4 using 2:3 title "MPI Size = 16 processes" with boxes
