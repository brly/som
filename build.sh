if [ $# -ne 1 ]; then
    echo "usage : ./build.sh [C++ Source File]" 1>&2
    exit 1
fi

g++ $1 -O3 -o $1.out `pkg-config opencv --cflags --libs` -std=c++0x
