#!/bin/tcsh

make
set execs="pm_dreamer"
set execdir="../../../bin"

cd ../../doc

foreach exec ($execs)
  $execdir/$exec -h > $exec.manpage
  eqn $exec.manpage > $exec.1
  man -t -p eqn ./$exec.manpage > $exec.ps
  ps2pdf $exec.ps $exec.pdf
end

cd ../


