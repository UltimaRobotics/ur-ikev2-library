#!/bin/bash

set -e

echo "Setting up OpenIKEv2 Integration Layer..."

# Clone libopenikev2 if not exists
if [ ! -d "libopenikev2" ]; then
    echo "Cloning libopenikev2..."
    git clone https://github.com/openikev2/libopenikev2.git
fi

# libopenikev2 will be built as subdirectory

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
echo "The application will output JSON status to terminal"
