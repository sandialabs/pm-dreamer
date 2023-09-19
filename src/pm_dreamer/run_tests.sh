#!/bin/tcsh

set BIND = `awk '$1=="BIN_DIR"{print $3}' ../Makefile.vars`

cd test
rm -f *.obm
rm -f *.valgrind
rm -f *.log

set mdiff = "../$BIND/mdiff"

if ( $#argv == 1 ) then
  if ( $argv[1] == "valgrind" ) then
    set bin = "valgrind ../$BIND/pm_dreamer"
    set binsix = "valgrind ../$BIND/pm_dreamer -p 1"
  else
    if ( $argv[1] == "mpi" ) then
      set bin = "mpirun -np 3 ../$BIND/pm_dreamer"
      set binsix = "mpirun -np 6 ../$BIND/pm_dreamer -p 2"
    else
      if ( $argv[1] == "mpivalgrind" ) then
        set bin = "mpirun -np 3 valgrind ../$BIND/pm_dreamer"
        set binsix = "mpirun -np 6 valgrind ../$BIND/pm_dreamer -p 2"
      else
        set bin = "../$BIND/pm_dreamer"
        set binsix = "../$BIND/pm_dreamer -p 1"
      endif
    endif
  endif
else
  set bin = "../$BIND/pm_dreamer"
  set binsix = "../$BIND/pm_dreamer -p 1"
endif

echo "Running mdiff from $mdiff..."
echo "Running pm_dreamer from $bin..."

set tests = `echo *.conf`

foreach test ( $tests )
  if ( $test == "pair-hfc0.conf" ) then
    $binsix -z data.in $test -f $test:r >>& $test:r.valgrind
  else
    $bin -z data.in $test -f $test:r >>& $test:r.valgrind
  endif
  
  echo "#############################################"
  echo "      $test"
  echo "#############################################"
  if ( $test == "pair-hfc10.conf" ) then
    echo "\n$binsix -z data.in $test -f $test:r >>& $test:r.valgrind" >> results.log
  else
    echo "\n$bin -z data.in $test -f $test:r >>& $test:r.valgrind" >> results.log
  endif
  if ( -e $mdiff ) then
    set lfiles = `echo $test:r*.obm`
    foreach lfile ( $lfiles )
      $mdiff $lfile ../compare/$lfile >> results.log
      tail -n 1 results.log
    end
  else
    echo "ERROR mdiff did not compile" >> results.log
  endif
end

set tests = "paird pairf pairf1 twothree twothreed twothreef twothreef1"
set data = (ljd.in ljd.in ljd.in twothree.in twothreed.in twothreed.in twothreed.in)
set conf = (pair-simple1.conf pair-simple1.conf pair-simple1.conf twothree.confd twothree.confd twothree.confd twothree.confd)
@ index = 1

foreach test ( $tests )
  $bin -z $data[$index] $conf[$index] -t $test -f $test >>& $test:r.valgrind
  $bin -z $data[$index] $conf[$index] -t $test -f $test"_corr" -s CORR >>& $test:r_corr.valgrind
  
  echo "#############################################"
  echo "      Potential template: $test"
  echo "#############################################"
  echo "\n$bin -z $data[$index] $conf[$index] -t $test -f $test >>& $test:r.valgrind" >> results.log
  echo "\n$bin -z $data[$index] $conf[$index] -t $test -f $test"_corr" -s CORR >>& $test:r_corr.valgrind" >> results.log
  if ( -e $mdiff ) then
    set lfiles = `echo $test:r*.obm`
    foreach lfile ( $lfiles )
      $mdiff $lfile ../compare/$lfile >> results.log
      tail -n 1 results.log
    end
  else
    echo "ERROR mdiff did not compile" >> results.log
  endif
  @ index = $index + 1
end

set tests = "pair paird pairf pairf1 twothree twothreed twothreef twothreef1"
set data = (data.in ljd.in ljd.in ljd.in twothree.in twothreed.in twothreed.in twothreed.in)
set conf = (pair-simple1.conf pair-simple1.conf-hs pair-simple1.conf pair-simple1.conf twothree.confd-hs twothree.confd twothree.confd twothree.confd)
@ index = 1

foreach test ( $tests )
  $bin $data[$index] $conf[$index] -t $test -f $test"z" >>& $test:r"z".valgrind
  $bin $data[$index] $conf[$index] -t $test -f $test"z_corr" -s CORR >>& $test:rz_corr.valgrind
  
  echo "#############################################"
  echo "      Potential template (vectorized): $test"
  echo "#############################################"
  echo "\n$bin $data[$index] $conf[$index] -t $test -f $test"z" >>& $test:r"z".valgrind" >> results.log
  echo "\n$bin $data[$index] $conf[$index] -t $test -f $test"z_corr" -s CORR >>& $test:rz_corr.valgrind" >> results.log
  if ( -e $mdiff ) then
    set lfiles = `echo $test:r"z"*.obm`
    foreach lfile ( $lfiles )
      $mdiff $lfile ../compare/$lfile >> results.log
      tail -n 1 results.log
    end
  else
    echo "ERROR mdiff did not compile" >> results.log
  endif
  @ index = $index + 1
end

set tests = "paird pairf twothreed twothreef"
set data = (lj_periodic.data lj_periodic.data sid5.data sid5.data)
set conf = (pair-simple1.conf pair-simple1.conf twothree.confd twothree.confd)
@ index = 1

foreach test ( $tests )
  $bin -z $data[$index] $conf[$index] -t $test -f $test"_per" >>& $test:r_per.valgrind
  $bin $data[$index] $conf[$index] -t $test -f $test"_perz" >>& $test:r_perz.valgrind
  
  echo "#############################################"
  echo "      Potential template (periodic): $test"
  echo "#############################################"
  echo "\n$bin -z $data[$index] $conf[$index] -t $test -f $test"_per" >>& $test:r_per.valgrind" >> results.log
  echo "\n$bin $data[$index] $conf[$index] -t $test -f $test"_perz" >>& $test:r_perz.valgrind" >> results.log
  if ( -e $mdiff ) then
    set lfiles = `echo $test:r_per*.obm`
    foreach lfile ( $lfiles )
      $mdiff $lfile ../compare/$lfile >> results.log
      tail -n 1 results.log
    end
  else
    echo "ERROR mdiff did not compile" >> results.log
  endif
  @ index = $index + 1
end

set tests = "pair paird pairf pairf1 twothree twothreed twothreef twothreef1"
set data = (data.in ljd.in ljd.in ljd.in twothree.in twothreed.in twothreed.in twothreed.in)
set conf = (pair-simple1.conf-s pair-simple1.conf-s pair-simple1.conf-s pair-simple1.conf-s twothree.confd-s twothree.confd-s twothree.confd-s twothree.confd-s)
@ index = 1

foreach test ( $tests )
  $bin $data[$index] $conf[$index] -t $test -f $test"s_z" >>& $test:r"s_z".valgrind
  
  echo "########################################################"
  echo "      Potential template (vectorized simplify): $test"
  echo "########################################################"
  echo "\n$bin $data[$index] $conf[$index] -t $test -f $test"s_z" >>& $test:r"s_z".valgrind" >> results.log
  if ( -e $mdiff ) then
    set lfiles = `echo $test:r"s_z"*.obm`
    foreach lfile ( $lfiles )
      $mdiff $lfile ../compare/$lfile >> results.log
      tail -n 1 results.log
    end
  else
    echo "ERROR mdiff did not compile" >> results.log
  endif
  @ index = $index + 1
end

set tests = "pair paird pairf pairf1 twothree twothreed twothreef twothreef1"
set data = (pair-mtype.in pair-mtype.in pair-mtype.in pair-mtype.in three-mtype.in three-mtype.in three-mtype.in three-mtype.in)
set conf = (pair-mtype.conft pair-mtype.conft pair-mtype.conft pair-mtype.conft three-mtype.conft three-mtype.conft three-mtype.conft three-mtype.conft)
@ index = 1

foreach test ( $tests )
  $bin $data[$index] $conf[$index] -t $test -f $test"m_t0_z" >>& $test:r"m_t0_z".valgrind
  
  echo "########################################################"
  echo "      Potential template (multiple types - all): $test"
  echo "########################################################"
  echo "\n$bin $data[$index] $conf[$index] -t $test -f $test"m_t0_z" >>& $test:r"m_t0_z".valgrind" >> results.log
  if ( -e $mdiff ) then
    set lfiles = `echo $test:r"m_t0_z"*.obm`
    foreach lfile ( $lfiles )
      $mdiff $lfile ../compare/$lfile >> results.log
      tail -n 1 results.log
    end
  else
    echo "ERROR mdiff did not compile" >> results.log
  endif
  @ index = $index + 1
end

set tests = "pair paird pairf pairf1 twothree twothreed twothreef twothreef1"
set data = (pair-mtype.in pair-mtype.in pair-mtype.in pair-mtype.in three-mtype.in three-mtype.in three-mtype.in three-mtype.in)
set conf = (pair-mtype1.conft pair-mtype1.conft pair-mtype1.conft pair-mtype1.conft three-mtype1.conft three-mtype1.conft three-mtype1.conft three-mtype1.conft)
@ index = 1

foreach test ( $tests )
  $bin $data[$index] $conf[$index] -t $test -f $test"m_t1_z" >>& $test:r"m_t1_z".valgrind
  
  echo "########################################################"
  echo "      Potential template (multiple types - one): $test"
  echo "########################################################"
  echo "\n$bin $data[$index] $conf[$index] -t $test -f $test"m_t1_z" >>& $test:r"m_t1_z".valgrind" >> results.log
  if ( -e $mdiff ) then
    set lfiles = `echo $test:r"m_t1_z"*.obm`
    foreach lfile ( $lfiles )
      $mdiff $lfile ../compare/$lfile >> results.log
      tail -n 1 results.log
    end
  else
    echo "ERROR mdiff did not compile" >> results.log
  endif
  @ index = $index + 1
end

set tests = "pair paird pairf pairf1 twothreed twothreef"
set data = (data.in ljd.in ljd.in ljd.in twothreed.in twothreed.in)
set conf = (pair-simple1.conf.2 pair-simple1.conf.2 pair-simple1.conf.2 pair-simple1.conf.2 twothree.confd.2 twothree.confd.2)
set obm = (pairf1z pairf1z pairf1z pairf1z twothreefz twothreefz)
@ index = 1

foreach test ( $tests )
  set files = `/bin/ls $obm[$index]*.obm | grep -v corr`
  foreach file ( $files )
    cp $file $test"__"$file:r.obm
    cp $file $test"__"$file:r"z".obm
  end

  $bin $data[$index] $conf[$index] -t $test -f $test"__"$obm[$index] -z -c >>& $test"__"$obm[$index]:r.valgrind
  $bin $data[$index] $conf[$index] -t $test -f $test"__"$obm[$index]"z" -c >>& $test"__"$obm[$index]:r"z".valgrind
  
  echo "#############################################"
  echo "      Potential template (Restart): $test"
  echo "#############################################"
  echo "\n$bin $data[$index] $conf[$index] -t $test -f $test"__"$obm[$index] -z -c >>& $test"__"$obm[$index]:r.valgrind" >> results.log
  echo "$bin $data[$index] $conf[$index] -t $test -f $test"__"$obm[$index]"z" -c >>& $test"__"$obm[$index]:r"z".valgrind" >> results.log
  if ( -e $mdiff ) then
    set lfiles = `echo $test"__"$obm[$index]*.obm`
    foreach lfile ( $lfiles )
      $mdiff $lfile ../compare/$lfile >> results.log
      tail -n 1 results.log
    end
  else
    echo "ERROR mdiff did not compile" >> results.log
  endif
  @ index = $index + 1
end

grep 'ERROR SUMM' *.valgrind | awk -F: '{print $1,$3}' | sed 's/(suppressed//g' >> valgrind.log
grep 'No such file' *.valgrind | awk -F: '{print $1,$4}' >> valgrind.log

cd ../
