#! /bin/bash

make
python3 test/system/system_test.py ./shell
