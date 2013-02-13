# Our robot, current position, blue empty square
set style line 1 lt 2 pt 4 linecolor rgb "blue"
# Our robot, future position, blue filled square
set style line 2 lt 1 pt 5 linecolor rgb "blue"
set style line 3 lt 1 pt 2 linecolor rgb "green"
# Ball, current position, red empty circle
set style line 4 lt 2 pt 6 linecolor rgb "red"
# Ball, future position, red filled circle
set style line 5 lt 1 pt 7 linecolor rgb "red"

plot "../test/astar_path.dat" with lines, \
"../test/our_current.dat" with linespoints ls 1, \
"../test/our_future.dat" with points ls 2, \
"../test/ball_current.dat" with linespoints ls 4, \
"../test/ball_future.dat" with points ls 5

unset key
set xrange[0:244]
set yrange[0:122]
set size ratio 0.5

load 'save.plt'
!mv my-plot.ps astar_path.ps
!lpr astar_path.ps
