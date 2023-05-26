#!/bin/bash
git submodule update --init --recursive
if [ "$(uname)" == "Darwin" ]; then
	./tools/premake5-mac gmake2
else
	./tools/premake5-linux gmake2
fi
