#!/bin/sh

for (( M_in = 1; M_in <= 50; M_in +=1 ))
do
    for (( K_in = 1; K_in <= 50; K_in += 1 ))
    do
        for (( np_in = 1; np_in < 10; np_in += 1 ))
        do
            printf "M = %7d; K = %7d; np = %2d -> " $M_in $K_in $np_in>> data
            make run np=$np_in M=$M_in K=$K_in >> data
            sleep 1
        done
    done
done

