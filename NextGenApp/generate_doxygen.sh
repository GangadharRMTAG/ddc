#!/bin/bash
# Script to generate Doxygen documentation for NextGenApp

set -e

echo "=========================================="
echo "NextGenApp - Doxygen Documentation Generator"
echo "=========================================="
echo ""

# Check if Doxygen is installed
if ! command -v doxygen &> /dev/null; then
    echo "Doxygen is not installed. Installing..."
    sudo apt-get update
    sudo apt-get install -y doxygen graphviz
    echo "Doxygen installed successfully."
    echo ""
fi

# Check if Graphviz is installed (for diagrams)
if ! command -v dot &> /dev/null; then
    echo "Graphviz (dot) is not installed. Installing..."
    sudo apt-get install -y graphviz
    echo "Graphviz installed successfully."
    echo ""
fi

# Verify versions
echo "Doxygen version:"
doxygen --version
echo ""
echo "Graphviz (dot) version:"
dot -V
echo ""

# Create docs directory if it doesn't exist
mkdir -p docs

# Generate documentation
echo "Generating Doxygen documentation..."
echo "This may take a few moments..."
echo ""

doxygen Doxyfile

if [ $? -eq 0 ]; then
    echo ""
    echo "=========================================="
    echo "Documentation generated successfully!"
    echo "=========================================="
    echo ""
    echo "HTML documentation is available at:"
    echo "  file://$(pwd)/docs/html/index.html"
    echo ""
    echo "To view the documentation:"
    echo "  1. Open docs/html/index.html in a web browser"
    echo "  2. Or run: xdg-open docs/html/index.html"
    echo ""
else
    echo ""
    echo "=========================================="
    echo "Error: Documentation generation failed!"
    echo "=========================================="
    echo ""
    exit 1
fi

