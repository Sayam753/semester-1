#!/bin/bash
echo "Please enter a number"
read s
sum=0
for ((i=1;i<s;i++))
do
  if [ $[$s%$i] -eq 0 ]
  then
    sum=$[$sum+i]
  fi
done
if [ $sum -eq $s ]
then
  echo "$s is a perfect number."
else
  echo "$s is not a perfect number."
fi
