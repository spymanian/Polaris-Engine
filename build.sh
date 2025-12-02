#!/bin/bash
# Build script for Polaris Engine

echo "🔨 Building Polaris Engine..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Run CMake
echo "📋 Running CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo "❌ CMake configuration failed!"
    exit 1
fi

# Build
echo "⚙️  Compiling..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo ""
    echo "🚀 Run with: ./build/polaris"
    echo "📊 Results will be saved to stats.csv"
else
    echo "❌ Build failed!"
    exit 1
fi
