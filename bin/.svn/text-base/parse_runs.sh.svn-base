#!/bin/tcsh
set files = `echo *.log2`
rm -f *.tab
foreach file ( $files )
  set file1=$file:r:r.fit.tab
  set file2=$file:r:r.time.tab
  awk -v f1="$file1" -v f2="$file2" '$1=="Best"{printf "%f ",$3 >> f1}$1=="Time:"{printf "%f ",$2 >> f2}' $file
  echo " " >> $file1
  echo " " >> $file2
end

set files = `echo *.time.tab`
foreach file ( $files )
  awk 'NF>maxf{maxf=NF}{for (i=1; i<=NF; i++) { sum[i]+=$i; t[i]+=1 }}END{for (i=1; i<=maxf; i++) {printf "%f,",sum[i]/t[i]}}' $file > $file:r:r.csv
  echo " " >> $file:r:r.csv
  awk 'NF>maxf{maxf=NF}{for (i=1; i<=NF; i++) { sum[i]+=$i; t[i]+=1 }}END{for (i=1; i<=maxf; i++) {printf "%f,",sum[i]/t[i]}}' $file:r:r.fit.tab >> $file:r:r.csv
  echo " " >> $file:r:r.csv
end
