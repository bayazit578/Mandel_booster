#!/usr/bin/bash

FREQUENCY="2.4GHz"
AFFINITY=3
TS_INTERVAL=0.5
XCUTABLE="compiled/prog"
CLOCK_DIR="clock"
STAT_DIR="statistics"

MODE=
case "$1" in
    "-DSCALAR_MODE")
        MODE="scalar"
        ;;
    "-DARRAY_MODE")
        MODE="array"
        ;;
    "-DINTRIN_MODE")
        MODE="intrin"
        ;;
    *)
        echo "Error: unknown mode '$1'"
        exit 1
        ;;
esac

mkdir -p $CLOCK_DIR
mkdir -p $STAT_DIR
PROG_ARG="$CLOCK_DIR/$MODE"
touch $PROG_ARG

sudo cpupower frequency-set -d $FREQUENCY
sudo cpupower frequency-set -u $FREQUENCY
sudo cpupower frequency-set -g performance

benchmark() {
     touch "statistics/$2"

    ./build.sh $1 -DBENCHMARK 
    
    sudo turbostat --quiet --cpu $AFFINITY --show \
        CPU,frequency,CoreThr,CoreTmp,Busy% --interval $TS_INTERVAL --Summary \
        --out $STAT_DIR/$2 & disown

    TS_PID=$!
    
    taskset --cpu-list $AFFINITY ./$XCUTABLE $PROG_ARG 
    
    kill $TS_PID
}

benchmark $1 $MODE

sudo cpupower frequency-set -d 500MHz
sudo cpupower frequency-set -u 3GHz
sudo cpupower frequency-set -g performance
