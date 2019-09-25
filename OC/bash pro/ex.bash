#!/bin/bash
echo Enter a number
read b


#CHECKING IF PRIME
fun()
{
for ((i=2; i<j; i++))
do
  if [ $[$j%$i] -eq  0 ]
  then
    flag=1
    break
else
  flag=0
fi
done
}

for ((j=b; j>1; j--))
do
  fun
if [ $flag -eq 1 ]
then
  continue
#ALL PRIMES ARE IN  $J ONE BY ONE`
else
  for ((;;))
  do
    if [ $[b%j] -eq 0 ]
    then
      echo "*$j"
      b=$[b/j]
    else
      break;
    fi
  done
fi
done
