#!/bin/bash

if [ ! -f ../CMakeLists.txt ]; then
	echo "cleaning .gitignore files from ./data"
	find . -type f -name '.gitignore' -delete 
else
	echo 'error: clean called in source folder.'
	echo 'only call this script in the build folder.'
	echo 'ignoring action.'
fi
