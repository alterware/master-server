#!/bin/bash
git submodule update --init --recursive
if [ "$(uname)" == "Darwin" ]; then
	./premake5 gmake2
else
	./premake5 gmake2
fi
