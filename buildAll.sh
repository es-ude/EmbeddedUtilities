#!/bin/bash

function build_and_zip {
    bazel build :pkg --config=$2
    if [ ! -z "$OUTPUT_DIR" ]; then
        mkdir -p "$OUTPUT_DIR"/"$1"/
        cp -f bazel-bin/"$1".zip "$OUTPUT_DIR"/"$1"/"$1"-"$2".zip
    fi
}

build_and_zip $1 native
build_and_zip $1 avr
