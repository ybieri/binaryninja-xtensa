# Binary Ninja Xtensa Architecture Plugin

Xtensa machine code decoder & Binary Ninja architecture plugin.

**Build (macOS)**
```
$ git clone https://github.com/PetoWorks/binaryninja-xtensa
$ git clone https://github.com/Vector35/binaryninja-api
$ cd binaryninja-xtensa
$ mkdir build
$ cmake -S build -B build -D BN_API_PATH="$(realpath ../binaryninja-api)"
$ cmake --build build
$ ls build/lib/xtensa_arch/libarch_xtensa.dylib
```

**Build (Windows)**
```
$ git clone https://github.com/PetoWorks/binaryninja-xtensa
$ git clone https://github.com/Vector35/binaryninja-api
$ cd binaryninja-xtensa
$ mkdir build
$ cmake -G "Visual Studio 18 2026" -A x64 -S . -B build -D BN_API_PATH="..\binaryninja-api"
$ cmake --build build --config Release
$ ls build/lib/xtensa_arch/Release/arch_xtensa.dll
```

Big thanks to @emesare to help write this plugin
