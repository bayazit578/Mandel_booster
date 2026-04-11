#!/usr/bin/bash

FREQUENCY="2.4GHz"
AFFINITY=3
TS_INTERVAL=0.5
XCUTABLE=compiled/prog
OUT_DIR=statistics

mkdir -p $OUT_DIR

sudo cpupower frequency-set -d $FREQUENCY
sudo cpupower frequency-set -u $FREQUENCY
sudo cpupower frequency-set -g performance

benchmark() {
     touch $OUT_DIR/bench_$2.ticks

    ./build.sh $1 -DBENCHMARK bench_$2.ticks
    
    sudo turbostat --quiet --cpu $AFFINITY --show CPU,frequency,CoreThr,CoreTmp,Busy% \
        --interval $TS_INTERVAL --Summary --out $OUT_DIR/bench_$2.statistics & disown
    
    TURBOSTAT_PID=$!
    
    taskset --cpu-list $AFFINITY ./$XCUTABLE bench_$2.ticks
    
    kill $TURBOSTAT_PID
}

benchmark "-DSCALAR_MODE" "scalar"

sudo cpupower frequency-set -d 500MHz
sudo cpupower frequency-set -u 3GHz
sudo cpupower frequency-set -g performance
