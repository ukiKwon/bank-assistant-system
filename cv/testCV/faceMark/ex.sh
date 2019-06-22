#!/bin/bash
#argument[0] : file target
#argument[x > 0] : file target's arguments

declare -a error_msg;
error_msg=''
target_src=''
target_out=''
target_compile='g++ '$target_src' -o '$target_out' $(pkg-config opencv4 --libs --cflags) -std=c++11'

function execSRC() {
	$target_src=$1
	$target_out=$1 + ".out"
	$target_compile
	msg=$?
	echo $msg

	
}

execSRC $1 $2 $3
