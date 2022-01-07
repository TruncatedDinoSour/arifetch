#!/usr/bin/env sh

set -xe

main() {
    cd src

    ${CXX:-clang++} -flto -Ofast -std=c++20 -lstatgrab -Wshadow -Wall -Wextra -Wpedantic -pedantic -march=native -pipe main.cc -o ../arifetch.elf

    cd ..

    if [ "$1" = "-r" ]; then
        ./arifetch.elf
    elif [ "$1" = "-install" ]; then
        mv arifetch.elf arifetch
        su -c 'install -Dm755 arifetch /usr/local/bin'
    elif [ "$1" = "-uninstall" ]; then
        su -c 'rm -fvi /usr/local/bin/arifetch'
    fi
}

main "$@"
