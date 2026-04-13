#!/bin/bash

./benchmark.sh -DSCALAR_MODE -O2
./benchmark.sh -DARRAY_MODE -O2
./benchmark.sh -DINTRIN_MODE -O2
./benchmark.sh -DSCALAR_MODE -O3
./benchmark.sh -DARRAY_MODE -O3
./benchmark.sh -DINTRIN_MODE -O3
