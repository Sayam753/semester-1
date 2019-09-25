#!/bin/bash
for ((i=1;i<=10000;i++))
do
  s=$i
  sum=0
  for ((;s>0;))
  do
    rem=$[$s%10]
    sum=$[$sum+(rem**3)]
    s=$[$s/10]
  done
  if [ $sum -eq $i ]
  then echo -n "$i "
  fi
done
echo -n "are the armstrong numbers"
echo
