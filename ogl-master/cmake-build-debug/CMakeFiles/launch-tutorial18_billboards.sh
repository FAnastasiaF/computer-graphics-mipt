#!/bin/sh
bindir=$(pwd)
cd /home/fanastasiiaf/Projects/task1/ogl-master/tutorial18_billboards_and_particles/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/fanastasiiaf/Projects/task1/ogl-master/cmake-build-debug/tutorial18_billboards 
	else
		"/home/fanastasiiaf/Projects/task1/ogl-master/cmake-build-debug/tutorial18_billboards"  
	fi
else
	"/home/fanastasiiaf/Projects/task1/ogl-master/cmake-build-debug/tutorial18_billboards"  
fi
