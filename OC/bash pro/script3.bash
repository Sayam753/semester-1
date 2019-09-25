#!/bin/bash
echo "Enter two dates in dd/mm/yyyy format"
read date1
read date2
d1=${date1:0:2}
d2=${date2:0:2}
m1=${date1:3:2}
m2=${date2:3:2}
y1=${date1:6}
y2=${date2:6}
echo $d1 $d2 $m1 
then
  echo "Wrong insertion"
fi
