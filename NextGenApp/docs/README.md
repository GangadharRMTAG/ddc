# NextGen Display Application - API Documentation

This directory contains the auto-generated API documentation using Doxygen.

## Generating Documentation

To generate the documentation, ensure Doxygen is installed:

```bash
# Install Doxygen (Ubuntu/Debian)
sudo apt-get install doxygen graphviz

# Generate documentation
cd /home/ubuntu/NextGen/git/NextGenDisplay/NextGenApp
doxygen Doxyfile
```

The generated HTML documentation will be available in the `docs/html/` directory.

## Viewing Documentation

Open `docs/html/index.html` in a web browser to view the documentation.

## Documentation Structure

The documentation includes:
- **Classes:** AppInterface, cLogger, LogMessageContext
- **Files:** All header and source files
- **Namespaces:** (if any)
- **Pages:** Main page from README.md
- **Class Hierarchy:** Inheritance diagrams
- **File List:** All documented files
- **Class Members:** All methods, properties, and signals

## Additional Information

For more details, see:
- TECHNICAL_DOCUMENTATION.md - High-level technical overview
- LOW_LEVEL_DESIGN.md - Detailed design specifications
- CODE_REVIEW.md - Code review and recommendations

