#!/bin/bash

FILES=benchmark/*
for f in $FILES
do
	echo "Processing $f file..."
	IFS='/' read -ra f_delim <<< "$f"
    word=${f_delim[1]}
    length=${#word}
    cname=${word:0:($length-3)}
	./bin/SFA $f > logs/$cname.log
done
