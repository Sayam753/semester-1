#!/bin/bash
COUNTER=20
while ((COUNTER<10))
do
  echo COUNTER $COUNTER
  let COUNTER=$[$COUNTER-1]
done
