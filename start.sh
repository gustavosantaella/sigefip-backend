#!/bin/bash

cmake --preset x64-debug
cmake --build --preset x64-debug --clean-first
./out/build/x64-debug/sigefip-back.exe