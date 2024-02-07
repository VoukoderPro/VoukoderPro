# Build instructions
## Msys2
Start up Msys2 from a Visual Studio command prompt:
```
cd c:\Msys64\
msys2_shell.cmd -use-full-path
```
## Directory structure
Assuming you are currently in a working directory like /home/Daniel/intelav1
```
build/include/vpl/*
build/include/vpl/preview/*
build/lib/vpl.lib
build/lib/pkgconfig/vpl.pc
cartwheel-ffmpeg
oneVPL/*
```
Make sure latest AV1 patches are installed to
```
cartwheel-ffmpeg/patches-av1
```
## Build oneVPL
```
git clone https://github.com/oneapi-src/oneVPL.git
cd oneVPL
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=install-dir -DBUILD_TOOLS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_SHARED_LIBS=OFF
# cmake --build . --config Release --target install (Replace with proper MSBuild command)

cp build/Release/vpl.lib ../build/lib/
# Place vpl.pc file in ../build/lib/pkgconfig/
cp -r ../api/vpl ../build/include/
cd ..
```
# Build FFmpeg
```
git clone https://github.com/intel-media-ci/cartwheel-ffmpeg --recursive
cd cartwheel-ffmpeg
git submodule update --init --recursive
cd ffmpeg
git am ../patches/*.patch
git am ../patches-av1/*.patch

# Full build
PKG_CONFIG_PATH=$(realpath ../../build/lib/pkgconfig):$PKG_CONFIG_PATH ./configure --toolchain=msvc --cc=cl --extra-cflags="-MD" --extra-libs=Ole32.lib --extra-libs=Advapi32.lib --prefix=../../build --build-suffix=-intelav1 --enable-shared --disable-static --arch=x86_64 --disable-doc --enable-libvpl --enable-encoder=av1_qsv

# Slim build
PKG_CONFIG_PATH=$(realpath ../../build/lib/pkgconfig):$PKG_CONFIG_PATH ./configure --toolchain=msvc --cc=cl --extra-cflags="-MD" --extra-libs=Ole32.lib --extra-libs=Advapi32.lib --prefix=../../build --build-suffix=-intelav1 --enable-shared --disable-static --arch=x86_64 --disable-doc --disable-network --disable-encoders --disable-decoders --disable-demuxers --disable-parsers --disable-protocols --enable-protocol=file --disable-indevs --disable-outdevs --disable-devices --disable-muxers --enable-muxer=mp4 --enable-muxer=matroska --enable-encoder=aac --enable-libvpl --enable-encoder=av1_qsv

make -j$(nproc)
make install
```
