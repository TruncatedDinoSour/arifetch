# This repository has been migrated to the self-hosted ari-web Forgejo instance: <https://git.ari.lt/ari/arifetch>
# Arifetch

A small and fast system information fetching tool written and configured in C++

# Requirements

- [libstatgrab](https://libstatgrab.org/)
- [clang (if CXX=clang++ or CXX is unset)](https://clang.llvm.org/)

# Credits

- [jgs, a:f on ascii.co.uk](https://ascii.co.uk/art/linux)

# Installation

```bash
$ ./build.sh -install
```

# Removal

```bash
$ ./build.sh -uninstall
```

# Building with GCC

```bash
$ CXX=g++ ./build.sh
```

# Configuration

Configure though the [src/config.hh](/src/config.hh) header.
Maybe even edit the source and add colours with [rang.hpp](https://github.com/agauniyal/rang)?
