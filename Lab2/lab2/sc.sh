#!/bin/bash

for i in {1..100}
    do
        ./$1 >> "output$i"
    done