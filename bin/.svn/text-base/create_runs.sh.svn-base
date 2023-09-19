#!/bin/tcsh

if ( $#argv != 6 ) then
  echo "USAGE: create_runs.sh header_name register_name register_value start inc end"
  exit(1)
endif

set header_name=$argv[1]
set register_name=$argv[2]
set register_value=$argv[3]
@ start=$argv[4]
@ inc=$argv[5]
@ end=$argv[6]
set awkval1 = "    <Entry key=\042$register_name\042>"
set awkval2 = "</Entry>"
set awkval = "$awkval1$register_value$awkval2"
echo "Register looks like:\n$awkval\n"

echo -n "Range of values will be: "
@ v = $start
while ( $v <= $end )
  echo -n "$v "
  @ v = $v + $inc
end
echo "\nPress enter to continue: "
set waitp = $<

set confs = `echo *.conf`
echo "First replacement example: "
awk -F, -v token="$awkval" '$1==token{print $0}' $confs[1]
awk -F, -v token="$awkval" -v head="$awkval1" -v val=$start '$1==token{print head val "</Entry>"}' $confs[1]

echo "\nPress enter to continue: "
set waitp = $<

set rand="39412762 50301449 72197985 30620855 11216372 44328996"
set rand=( $rand "46676256 1466888 66405187 72406171 28163361 26181868")
set rand=( $rand "70847141 78385903 98615781 47334271 90281884 45105877")
set rand=( $rand "80451681 82886448 16627012 39390601 52075749 71812398")
set rand=( $rand "56918952 46080618 44530706 8774461 44348322 36629985")
set rand=( $rand "30253382 85184470 5947939 94975929 55793851 1423302")
set rand=( $rand "59617709 81620571 97709236 22190808 70368367 52206092")
set rand=( $rand "93289706 71335445 22803891 44964211 17219970 96882015")
set rand=( $rand "35571611 4904683")

rm -rf $header_name
mkdir $header_name
cd $header_name
foreach conf ( $confs )
  set headfile=$conf:r
  	  	
  @ v = $start
  while ( $v <= $end )
    set conf_file=$conf:r.$v
    awk -F, -v token="$awkval" -v head="$awkval1" -v val=$v '$1==token{print head val "</Entry>"}$1!=token{print $0}' ../$conf > $conf_file

    foreach rand_num ( $rand )
      set gp_head = $conf_file.$rand_num
      echo "mpiexec -np 32 gp_force ../lj2.data $conf_file -f $gp_head -r $rand_num >& $gp_head.log2" >> $conf_file.sh
    end
    echo "#\041/bin/bash" > $conf_file.brs
    echo "#PBS -l nodes=8:ppn=4:vis" >> $conf_file.brs  
    echo "#PBS -l walltime=2:00:00" >> $conf_file.brs  
    echo "#PBS -A 127740/1" >> $conf_file.brs
    echo "tcsh $conf_file.sh" >> $conf_file.brs  

    @ v = $v + $inc
  end

end
