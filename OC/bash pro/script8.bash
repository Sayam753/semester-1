#!/bin/bash
echo -n "Please enter a number:  "
read n
echo $n
temp=$n
b=0
for ((i=2;i<=n;i++))
do
  c=1
  for ((j=2;j<i;j++))
  do
      if ((i%j==0))
      then
        c=$[$c+1]
        break
      fi
  done
  if ((c==1))
  then
    if ((n==0))
    then
        break
    fi
    while [ TRUE ]
    do
      if ((n%i==0))
      then
        num[$b]=$i
        n=$[$n/$i]
        b=$[$b+1]
      else
        break
      fi
    done
  fi
done
for ((i=0;i<b-1;i++))
do
  echo -n "${num[$i]} x "
done
echo -n "${num[$[$b-1]]} = $temp"
echo
