#!/bin/bash

cmd="r\nq"

for file in $@; do
	make loader > /dev/null
	make > /dev/null
	echo "testfile: $file"
	echo -e $cmd | ./nemu $file | grep 'nemu:'
	echo ""
done

