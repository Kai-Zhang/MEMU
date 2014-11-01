#!/bin/bash

cmd="r\nq"

for file in $@; do
	echo "testfile: $file"
	echo -e $cmd | ./memu $file | grep 'memu:'
	echo ""
done

