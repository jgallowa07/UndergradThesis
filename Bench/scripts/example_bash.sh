#!/bin/bash

div ()  # Arguments: dividend and divisor
{
        if [ $2 -eq 0 ]; then echo division by 0; exit; fi
        local p=12                            # precision
        local c=${c:-0}                       # precision counter
        local d=.                             # decimal separator
        local r=$(($1/$2)); echo -n $r        # result of division
        local m=$(($r*$2))
        [ $c -eq 0 ] && [ $m -ne $1 ] && echo -n $d
        [ $1 -eq $m ] || [ $c -eq $p ] && return
        local e=$(($1-$m))
        let c=c+1
        div $(($e*10)) $2
}

N=10000
size=10000

#x=$("scale=3; 6.5 / 2.7"|bc)  
x=$(echo "scale=10; ($size / (4.0 * $N)) / 100000000.0"|bc)

echo $x

#echo "scale=3 $x * 10.0"|bc

#M=$((4*$N))
#R=$(div $size $M)

#MR=$(div r 100000000)

#R=$(($N*$size))
#echo $MR

#res=  $(div $N $size)
#echo $res

#let "MUTRATE=$SIZE/4/*$N/100000000"

#echo $MUTRATE
