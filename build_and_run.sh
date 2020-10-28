#!/bin/bash\
cmake -DCMAKE_INSTALL_PREFIX=${PWD} ../ && make install && ./snake
