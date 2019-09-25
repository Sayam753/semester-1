#!/bin/bash
a=2334;let "a += 1";echo "a = $a"; b=${a/23/BB};echo "b = $b";
let "b += 1";echo "b = $b"; c=BB34;echo "c = $c"; d=${c/BB/23};echo "d = $d";
let "d += 1";echo "d = $d"; e='';echo "e = $e";
let "e += 1";echo "e = $e"; echo "f = $f";
let "f += 1";echo "f = $f";
