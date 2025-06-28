# 🚀 CI/CD Release Workflow for Zappy

This document explains how to use the GitHub Actions workflow to create automatic releases of the Zappy project.

## 📋 Overview

The release workflow allows you to:
- Automatically generate semantic versions
- Compile all project components
- Create release packages
- Publish on GitHub Releases
- Generate automatic changelog

## 🎯 Versioning Types

The system follows **semantic versioning** (SemVer) with the format `v{MAJOR}.{MINOR}.{PATCH}`:

### 🆕 New version (MAJOR)
- **Option**: `nouvelle-version`
- **Effect**: `v1.2.3` → `v2.0.0`
- **Usage**: Major changes, breaking changes
- **Example**: Complete architecture refactor, incompatible API changes

### ✨ New feature (MINOR)
- **Option**: `nouvelle-feature`
- **Effect**: `v1.2.3` → `v1.3.0`
- **Usage**: New compatible features
- **Example**: New game mode, new AI commands, GUI improvements

### 🐛 Bug resolution (PATCH)
- **Option**: `resolution-bug`
- **Effect**: `v1.2.3` → `v1.2.4`
- **Usage**: Bug fixes, minor improvements
- **Example**: Crash fixes, performance optimizations

## 🚀 How to create a release

### 1. Access the workflow
1. Go to GitHub → **Actions** tab
2. Select the **🚀 Release Zappy** workflow
3. Click on **Run workflow**

### 2. Configure the release
- **Branch**: Select the branch (usually `main`)
- **Version type**: Choose from:
  - `nouvelle-version` (MAJOR)
  - `nouvelle-feature` (MINOR)
  - `resolution-bug` (PATCH)
- **Pre-release**: Check if it's a test version

### 3. Trigger the release
Click on **Run workflow** to start the process.

## 📦 Release Contents

Each release contains:

### 🗂️ Archive `zappy-vX.Y.Z.tar.gz`
```
zappy-vX.Y.Z/
├── zappy_server          # Game server (C)
├── zappy_gui            # Graphical interface (C++)
├── zappy_ai             # AI client (Python)
├── lib/                 # Static libraries
│   ├── libgui.a
│   ├── libserver.a
│   ├── libraylib_cpp.a
│   └── libraygui_cpp.a
├── plugins/             # Dynamic libraries
│   ├── libraygui.so
│   └── libraylibcpp.so
├── README.md           # Documentation
├── install.sh          # Installation script
└── VERSION             # Version number
```

### 📋 Installation script
```bash
# Make the script executable
chmod +x install.sh

# Install Zappy on the system
sudo ./install.sh
```

## 🔧 Technical Operation

### 1. 🔐 Permission verification
- Release creation rights control
- User and repository validation

### 2. 📊 Version calculation
- Latest semantic tag retrieval
- Automatic new version calculation
- First release case handling (`v0.0.0` → `v1.0.0`)

### 3. 🏗️ Compilation
- System dependencies installation
- Compilation of all components:
  - C server with Make
  - C++ GUI with raylib
  - Python AI client
- Binary integrity verification

### 4. 📦 Packaging
- Complete archive creation
- Inclusion of all binaries and libraries
- Installation script generation
- Automatic changelog creation

### 5. 🚀 Publication
- Git tag creation
- GitHub Releases publication
- Archive upload
- Release notes generation

## 📝 Automatic Changelog

The changelog is automatically generated and contains:
- Release type and description
- List of commits since last version
- Release content
- Installation instructions

### Changelog example
```markdown
## 🎮 Zappy v1.2.0

### 📅 Release date: 12/06/2025 at 14:30

### ✨ New feature
This version adds new features while maintaining compatibility.

### 📋 Changes
- Added spectator mode in GUI
- Improved AI with new algorithms
- Network performance optimization
- Minor bug fixes

### 📦 Release content
- **Binaries**: zappy_server, zappy_gui, zappy_ai
- **Libraries**: libgui.a, libserver.a, libraylib_cpp.a, libraygui_cpp.a
- **Plugins**: libraygui.so, libraylibcpp.so
- **Documentation**: Usage guide and API
```

## ⚙️ Advanced Configuration

### Environment variables
The workflow uses several configurable variables:

```yaml
env:
  BINARIES: "zappy_gui zappy_server zappy_ai"
  LIBRARIES: "libgui.a libserver.a libraylib_cpp.a libraygui_cpp.a"
  DYNAMIC_LIBS: "libraygui.so libraylibcpp.so"
```

### Timeouts
- Total build: 20 minutes
- Dependencies installation: 10 minutes
- Compilation: 15 minutes

### Artifacts
- Retention: 90 days
- Format: compressed tar.gz
- Automatic upload to GitHub

## 🐛 Troubleshooting

### Common errors

#### ❌ Binary not found
```
❌ Binary zappy_server not found
```
**Solution**: Check that compilation went well in the Makefile

#### ❌ Missing library
```
⚠️ Static library libgui.a not found
```
**Solution**: Ensure that `make all` compiles all libraries

#### ❌ Insufficient permissions
```
Error: Resource not accessible by integration
```
**Solution**: Check GitHub token permissions in repository settings

### Debug logs
Each workflow step produces detailed logs:
- 🔍 Permission verification
- 📊 Version calculation with details
- 🏗️ Complete compilation logs
- ✅ Binary and library verification
- 📦 Release archive content

## 🎯 Best Practices

### Before creating a release
1. **Test locally**: `make all && make tests_run`
2. **Check changes**: Review commits since last version
3. **Choose the right type**: Evaluate the impact of changes
4. **Document**: Update README.md if necessary

### Version management
- **PATCH**: For urgent hotfixes
- **MINOR**: For normal sprints/iterations
- **MAJOR**: For important milestones

### Timing
- Avoid releases at end of week
- Prefer business hours for support
- Test pre-releases before final releases

## 📞 Support

In case of issues with the release workflow:
1. Check logs in the Actions tab
2. Verify configuration in `.github/workflows/release.yml`
3. Contact the development team

---

*This workflow was designed to automate and standardize the release process of the Zappy project while maintaining quality and traceability.*
