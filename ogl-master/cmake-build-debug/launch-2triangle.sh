#!/bin/sh
bindir=$(pwd)
cd /home/fanastasiiaf/Projects/task1/ogl-master/2triangle/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/fanastasiiaf/Projects/task1/ogl-master/cmake-build-debug/2triangle 
	else
		"/home/fanastasiiaf/Projects/task1/ogl-master/cmake-build-debug/2triangle"  
	fi
else
	"/home/fanastasiiaf/Projects/task1/ogl-master/cmake-build-debug/2triangle"  
fi
