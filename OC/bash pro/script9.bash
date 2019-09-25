#!/bin/bash
for ((i=0;i<=9;i++))
do
    k=$i
    for ((j=0;j<=i;j++))
    do 
        echo -n $k
        k=$[$k-1]
    done
    echo ""
done
    
