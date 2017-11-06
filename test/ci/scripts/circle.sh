#!/bin/bash

function build_lugbench() {
    cd ~/LugBench

    mkdir build && cd build
    (cmake .. -DBUILD_SHADERS=false -DLUG_THIRDPARTY_DIR=$HOME/.local/thirdparty) || return 1
    (make all) || return 1

    return 0
}

case $CIRCLE_NODE_INDEX in
    0)
        export CXX=clang++
        build_lugbench
    ;;

    1)
        export CXX=g++
        build_lugbench
    ;;
esac

