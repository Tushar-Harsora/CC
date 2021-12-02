#!/bin/bash

g++ Practical2.cpp -o Practical2
echo Testing string a + b - 10
echo "a + b - 10" | ./Practical2 && echo
echo Testing string axdf / bsfa + 10.5
echo "axdf / bsfa + 10.5" | ./Practical2 && echo
echo "Testing string (a +     b)-10"
echo "(a +     b)-10" | ./Practical2 && echo
echo "Testing string ((a) + 1000"
echo "((a) + 1000" | ./Practical2 && echo
echo "Testing string ((1)+5/(2))"
echo "((1)+5/(2))" | ./Practical2 && echo
echo "Testing string 1 -- 8"
echo "1 -- 8" | ./Practical2 && echo

rm Practical2