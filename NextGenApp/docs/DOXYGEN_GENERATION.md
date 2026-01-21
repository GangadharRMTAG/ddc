# Doxygen Documentation Generation Guide

## Overview

This project uses Doxygen to generate comprehensive API documentation from source code comments.

## Prerequisites

Before generating documentation, ensure the following tools are installed:

- **Doxygen** (version 1.8.13 or later)
- **Graphviz** (for class diagrams and graphs)

### Installation

#### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install doxygen graphviz
```

#### macOS (using Homebrew):
```bash
brew install doxygen graphviz
```

#### Windows:
Download installers from:
- Doxygen: https://www.doxygen.nl/download.html
- Graphviz: https://graphviz.org/download/

## Generating Documentation

### Method 1: Using the Generation Script (Recommended)

```bash
cd /home/ubuntu/NextGen/git/NextGenDisplay/NextGenApp
./generate_doxygen.sh
```

This script will:
1. Check if Doxygen and Graphviz are installed
2. Install them if missing (requires sudo)
3. Generate the documentation
4. Display the location of the generated HTML files

### Method 2: Manual Generation

```bash
cd /home/ubuntu/NextGen/git/NextGenDisplay/NextGenApp
doxygen Doxyfile
```

## Viewing Documentation

After generation, open the HTML documentation:

```bash
# Linux
xdg-open docs/html/index.html

# macOS
open docs/html/index.html

# Windows
start docs/html/index.html
```

Or navigate to `docs/html/index.html` in your web browser.

## Documentation Structure

The generated documentation includes:

### Main Pages
- **Main Page:** Project overview from README.md
- **Classes:** All documented classes
- **Files:** All source and header files
- **Namespaces:** (if any)

### Class Documentation
- **AppInterface:** Main application interface class
  - Properties (Q_PROPERTY)
  - Public methods
  - Signals
  - Private methods and members
  - Enumerations

- **cLogger:** Logging system singleton
  - Static methods
  - Properties
  - Signals and slots

- **LogMessageContext:** Log message context helper

### Diagrams
- **Class Diagrams:** Inheritance and composition relationships
- **Collaboration Diagrams:** Class interactions
- **Include Graphs:** File dependency graphs
- **Directory Graphs:** Directory structure

### Additional Features
- **Search:** Full-text search across documentation
- **Index:** Alphabetical index of all symbols
- **File List:** All documented files
- **Class Hierarchy:** Inheritance tree
- **Class Members:** Detailed member documentation

## Configuration

The Doxygen configuration is stored in `Doxyfile`. Key settings:

- **Input:** Source files in `include/`, `src/`, and `main.cpp`
- **Output:** HTML documentation in `docs/html/`
- **Extract:** All members, including private
- **Diagrams:** Enabled with Graphviz
- **Qt Support:** Enabled for Q_PROPERTY, Q_INVOKABLE, etc.

## Documentation Standards

### File Headers
All source files should include:
```cpp
/**
 * @file filename.h
 * @brief Brief description
 * @date Date
 * @author Author name
 */
```

### Class Documentation
```cpp
/**
 * @brief Brief class description
 *
 * Detailed description of the class purpose and usage.
 */
class MyClass {
    // ...
};
```

### Method Documentation
```cpp
/**
 * @brief Brief method description
 *
 * Detailed description of what the method does.
 *
 * @param param1 Description of parameter 1
 * @param param2 Description of parameter 2
 * @return Description of return value
 * @note Additional notes
 * @warning Warnings if any
 */
```

### Property Documentation (Qt)
```cpp
/**
 * @property propertyName
 * @brief Brief property description
 *
 * Detailed description of the property.
 */
Q_PROPERTY(type propertyName READ getter NOTIFY signal)
```

## Troubleshooting

### Doxygen Not Found
```bash
# Check if installed
which doxygen

# Install if missing
sudo apt-get install doxygen
```

### Graphviz Not Found (Diagrams won't generate)
```bash
# Check if installed
which dot

# Install if missing
sudo apt-get install graphviz
```

### Documentation Not Updating
- Delete `docs/html/` directory
- Regenerate with `doxygen Doxyfile`
- Clear browser cache if viewing old documentation

### Missing Documentation
- Check that source files have proper Doxygen comments
- Verify `EXTRACT_ALL = YES` in Doxyfile
- Check `EXCLUDE` patterns in Doxyfile

## Continuous Integration

To generate documentation in CI/CD:

```yaml
# Example GitHub Actions
- name: Install Doxygen
  run: sudo apt-get install -y doxygen graphviz

- name: Generate Documentation
  run: doxygen Doxyfile

- name: Deploy Documentation
  run: |
    # Deploy docs/html/ to hosting service
```

## Additional Resources

- [Doxygen Manual](https://www.doxygen.nl/manual/index.html)
- [Doxygen Qt Support](https://www.doxygen.nl/manual/autolink.html)
- [Graphviz Documentation](https://graphviz.org/documentation/)

