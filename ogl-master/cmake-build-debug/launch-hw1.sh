#!/bin/sh
bindir=$(pwd)
cd /home/fanastasiiaf/Projects/OpenGLTutorials/ogl-master/hw1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/fanastasiiaf/Projects/OpenGLTutorials/ogl-master/cmake-build-debug/hw1 
	else
		"/home/fanastasiiaf/Projects/OpenGLTutorials/ogl-master/cmake-build-debug/hw1"  
	fi
else
	"/home/fanastasiiaf/Projects/OpenGLTutorials/ogl-master/cmake-build-debug/hw1"  
fi
