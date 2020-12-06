#!/bin/sh

BUILD_DIR=build

SIZE=10000000

$BUILD_DIR/workload ./data/10M/a_10M_load.dat ./data/10M/a_10M_txns.dat $SIZE $SIZE
$BUILD_DIR/workload ./data/10M/c_10M_load.dat ./data/10M/c_10M_txns.dat $SIZE $SIZE
$BUILD_DIR/workload ./data/10M/e_10M_load.dat ./data/10M/e_10M_txns.dat $SIZE $SIZE
