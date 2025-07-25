#!/bin/bash

set -e

echo "Setting up OpenIKEv2 Integration Layer..."

# Clone libopenikev2 if not exists
if [ ! -d "libopenikev2" ]; then
    echo "Cloning libopenikev2..."
    git clone https://github.com/openikev2/libopenikev2.git
fi

cd libopenikev2

# Create build directory
mkdir -p build
cd build

# Configure and build libopenikev2 using CMake
echo "Building libopenikev2..."
cmake .. -DCMAKE_BUILD_TYPE=Release -DLIBOPENIKE_VERSION="0.7"
make -j$(nproc)

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
