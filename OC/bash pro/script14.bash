#!/bin/bash
echo "Please enter a string"
read s
reverse=`echo $s | rev`
if [ "$reverse" = "$s" ]
then
  echo "$s is a palindrome."
else
  echo "$s is not a palindrome."
fi
