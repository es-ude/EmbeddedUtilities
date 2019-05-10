#!/bin/bash

function build_and_zip {
    bazel build :pkg --config=$2
    if [ ! -z "$OUTPUT_DIR" ]; then
        mkdir -p "$OUTPUT_DIR"/"$1"/
        cp -f bazel-bin/pkg.tar.gz "$OUTPUT_DIR"/"$1"/"$1"-"$2".tar.gz
    fi
}

build_and_zip $1 $2
# build_and_zip $1 avr
