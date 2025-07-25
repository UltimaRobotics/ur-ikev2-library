#!/bin/bash

set -e

echo "Setting up OpenIKEv2 Integration Layer..."

# Clone libopenikev2 if not exists
if [ ! -d "libopenikev2" ]; then
    echo "Cloning libopenikev2..."
    git clone https://github.com/openikev2/libopenikev2.git
fi

# Build libopenikev2 manually since autotools has issues
cd libopenikev2/src

echo "Building libopenikev2 source files..."

# Create object files from all cpp files
g++ -c -fPIC -I. *.cpp -DHAVE_CONFIG_H -O2

# Create static library
ar rcs libopenikev2.a *.o

# Create build directory and move library
mkdir -p ../build/lib
mkdir -p ../build/include
mv libopenikev2.a ../build/lib/
cp *.h ../build/include/

echo "libopenikev2 built successfully"

cd ../..

# Create build directory for integration layer
mkdir -p build
cd build

# Configure and build integration layer
echo "Building integration layer..."
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

echo "Build complete! Executable is in build/OpenIKEv2Integration"
echo "Configuration file: config.json"
echo ""
echo "To run the application:"
echo "  cd build"
echo "  ./OpenIKEv2Integration"
echo ""
echo "The HTTP API will be available on http://localhost:5000"
