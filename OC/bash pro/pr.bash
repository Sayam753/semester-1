#!/bin/bash
echo "Enter a num";read a
for ((i=2;i<a;i++))
do
while [ $[a%i] -eq 0 ]
do
  echo $i
  let a=$a/$i
done

done
