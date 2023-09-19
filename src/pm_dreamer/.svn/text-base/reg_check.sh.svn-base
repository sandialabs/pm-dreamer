set mail_list="wmbrown@sandia.gov"
set mail_sub="GP-Force Test Report"
set log_dir="/scratch2/regression/gpforce_reg_logs"
/bin/rm -f temp.mail

if ( $#argv != 0 ) then
        set valgrind = ""
        set mpivalgrind = "mpi"
else
        set valgrind = "valgrind"
        set mpivalgrind = "mpivalgrind"
endif

svn checkout svn+ssh://development.sandia.gov/usr/local/svn/gp_force/src ./src > changes.txt
if ( $status != 0 ) then
	echo "Error in SVN checkout!" > temp.mail
	echo "Error in SVN checkout!"
	cat changes.txt >> temp.mail
	unix2dos temp.mail
	mail -s "$mail_sub" $mail_list < temp.mail
	/bin/rm -f last.complete.ok
	exit
endif
@ changes = `cat changes.txt | wc -l`
if ( $changes == 5 ) then
    if ( -e last.complete.ok ) then
	set last_revision = `cat last_revision.txt`
	echo "No changes since last checked revision: $last_revision" > temp.mail
	echo "No changes since last checked revision: $last_revision"
	unix2dos temp.mail
#	mail -s "$mail_sub" $mail_list < temp.mail
	exit
    endif
endif
/bin/rm -f last.complete.ok
svn checkout svn+ssh://development.sandia.gov/usr/local/svn/gp_force/obj ./obj |& awk '$1=="Checked"{print $4}' > last_revision.txt

set revision = `cat last_revision.txt`

echo "Regression testing log for PM-Dreamer revision $revision" > log
date >> log
set compiles = `awk '$1=="#REGC"{print $4}' ./src/Makefile.vars`
echo $compiles

rm -rf testdir
rm -f compile_test_*.sh
mkdir testdir
foreach compile ( $compiles )
 mkdir testdir/$compile
 mkdir testdir/$compile/bin
 cp -R src testdir/$compile
 cp -R obj testdir/$compile
 awk -v comp=$compile '$1=="#REGC" && $4==comp{$1=""}$1=="#REGI"{$1=""}{print $0}' \
  ./src/Makefile.vars > ./testdir/$compile/src/Makefile.vars
  
 echo "cd testdir/$compile/src/pm_dreamer" >> compile_test_$compile.sh
 echo "make clean >& log.make" >> compile_test_$compile.sh
 echo "make  >>& log.make" >> compile_test_$compile.sh
 set mpi = `echo $compile | grep mpi | wc -l`
 if ($mpi == "0") then
   echo "source run_tests.sh $valgrind" >> compile_test_$compile.sh
 else
   echo "source run_tests.sh $mpivalgrind" >> compile_test_$compile.sh
 endif
 echo "cd ../../../../" >> compile_test_$compile.sh
 echo "/bin/tcsh compile_test_$compile.sh" >> compile_test_script.sh
end

source compile_test_script.sh

rm -f logtwo

####################### Write the Results errors

echo "\n-----------------OUTPUT ERROR SUMMARY----------------" >> log
foreach compile ( $compiles )
 set tdir = ./testdir/$compile/src/pm_dreamer/test
 awk -v name=$compile 'NF!=0 && $1!="OK" && $1!="valgrind" && $1!="../../../bin//pm_dreamer" &&$1!="mpirun"{print name,$1,$2}' $tdir/results.log >> log
 echo "\n------------------- $compile -----------------" >> logtwo
 cat $tdir/results.log >> logtwo
  echo "------------------------------------------------" >> logtwo
end
echo "\n--------------------- END SUMMARY -------------------" >> log

####################### Write the Valgrind errors

echo "\n----------------VALGRIND ERROR SUMMARY---------------" >> log
foreach compile ( $compiles )
 set tdir = ./testdir/$compile/src/pm_dreamer/test
 awk -v name=$compile '$2!=0{print name,$1,$2,$3}' $tdir/valgrind.log >> log
 echo "\n------------------- $compile -----------------" >> logtwo
 cat $tdir/valgrind.log >> logtwo
  echo "------------------------------------------------" >> logtwo
end
echo "\n--------------------- END SUMMARY -------------------" >> log
echo "\n\n\n\n" >> log
echo "\n----------------------- DETAILS ---------------------" >> log

echo "\n\n----------------------- CHANGES ---------------------" >> logtwo
cat changes.txt >> logtwo


cat log logtwo > temp.mail

####################### Copy the logs
rm -rf $log_dir/rev$revision
mkdir $log_dir/rev$revision
foreach compile ( $compiles )
 mkdir $log_dir/rev$revision/$compile
 set tdir = ./testdir/$compile/src/pm_dreamer/test
 /bin/cp $tdir/*.valgrind $tdir/*.results $tdir/../*.make $log_dir/rev$revision/$compile
 /bin/cp changes.txt temp.mail $log_dir/rev$revision
end

sed G temp.mail > t; /bin/mv -f t temp.mail
mail -s "$mail_sub" $mail_list < temp.mail
date > last.complete.ok

cd $log_dir/rev$revision/
gzip `find ./`
