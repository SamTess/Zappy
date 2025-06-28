# 📖 Release Workflow Usage Examples

This document provides concrete examples of using the release workflow for different scenarios.

## 🎯 Release Scenarios

### Scenario 1: First Project Release

**Situation**: The Zappy project is completed and ready for its first public release.

**Action**:
- Version type: `nouvelle-version`
- Pre-release: No

**Result**:
- Generated version: `v1.0.0` (if no tag exists)
- Automatic changelog with complete project description
- Complete archive with all components

### Scenario 2: Adding a New Feature

**Situation**: The team has added a spectator mode in the GUI and new commands for the AI.

**Action**:
- Version type: `nouvelle-feature`
- Pre-release: No

**Result**:
- Generated version: `v1.0.0` → `v1.1.0`
- Changelog with list of new features
- Maintained compatibility with previous versions

### Scenario 3: Critical Bug Fixes

**Situation**: Crash bugs have been fixed in the server and GUI.

**Action**:
- Version type: `resolution-bug`
- Pre-release: No

**Result**:
- Generated version: `v1.1.0` → `v1.1.1`
- Changelog focused on fixes
- Quick release for affected users

### Scenario 4: Major Architecture Refactor

**Situation**: The team has rewritten the server with a new architecture and changed the API.

**Action**:
- Version type: `nouvelle-version`
- Pre-release: No

**Result**:
- Generated version: `v1.1.1` → `v2.0.0`
- Changelog with breaking changes warning
- Documentation of necessary migrations

### Scenario 5: Test Version Before Release

**Situation**: The team wants to test a new version before the official release.

**Action**:
- Version type: `nouvelle-feature`
- Pre-release: **Yes**

**Result**:
- Generated version: `v1.1.0` → `v1.2.0-pre`
- Marked as pre-release on GitHub
- Testing by early adopters

## 📋 Step-by-Step Guide

### Detailed Steps to Create a Release

1. **Preparation**
   ```bash
   # Check project status
   git status
   git pull origin main
   
   # Test compilation
   make clean && make all
   make tests_run
   ```

2. **Access Workflow**
   - Go to GitHub
   - Click on "Actions" tab
   - Find "🚀 Release Zappy"
   - Click "Run workflow"

3. **Configuration**
   - **Branch**: Select `main` (or release branch)
   - **Version type**: Choose according to scenario
   - **Pre-release**: Check if necessary

4. **Monitor Execution**
   - Watch logs in real-time
   - Verify each step:
     - ✅ Permissions
     - ✅ Version calculation
     - ✅ Compilation
     - ✅ Archive creation
     - ✅ Publication

5. **Post-release Verification**
   - Check releases page
   - Download and test archive
   - Validate changelog

## 🔢 Version Calculation Examples

| Current Version | Release Type | New Version | Use Case |
|-----------------|--------------|-------------|----------|
| `v0.0.0` | `nouvelle-version` | `v1.0.0` | First release |
| `v1.0.0` | `nouvelle-feature` | `v1.1.0` | New feature |
| `v1.1.0` | `resolution-bug` | `v1.1.1` | Bug fix |
| `v1.5.3` | `nouvelle-version` | `v2.0.0` | Breaking change |
| `v2.1.7` | `nouvelle-feature` | `v2.2.0` | Compatible feature |

## 📦 Typical Release Content

### Archive `zappy-v1.2.0.tar.gz`
```
📁 zappy-v1.2.0/
├── 🔧 zappy_server      # Game server (Linux x64)
├── 🎨 zappy_gui         # Graphical interface
├── 🤖 zappy_ai          # Python AI client
├── 📁 lib/              # Static libraries
│   ├── libgui.a
│   ├── libserver.a
│   ├── libraylib_cpp.a
│   └── libraygui_cpp.a
├── 📁 plugins/          # Dynamic libraries
│   ├── libraygui.so
│   └── libraylibcpp.so
├── 📋 README.md         # Documentation
├── ⚙️ install.sh        # Installation script
└── 🏷️ VERSION          # Version number
```

### Automatic Installation
```bash
# Extract
tar -xzf zappy-v1.2.0.tar.gz
cd zappy-v1.2.0

# Install
sudo ./install.sh

# Use
zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100
zappy_gui -p 4242 -h localhost
zappy_ai -p 4242 -n team1
```

## 🕒 Typical Release Timeline

```
⏰ 00:00 - Workflow trigger
├── 00:01 - Permission verification ✅
├── 00:02 - New version calculation ✅
├── 00:03 - Dependencies installation (⏳ 10 min)
├── 00:13 - Project compilation (⏳ 5 min)
├── 00:18 - Binary verification ✅
├── 00:19 - Archive creation ✅
├── 00:20 - Artifacts upload ✅
├── 00:21 - Git tag creation ✅
└── 00:22 - Release publication ✅ 🎉
```

## 🚨 Common Error Management

### Error: "Binary not found"
```
❌ Binary zappy_server not found
```
**Solution**: Check server Makefile, ensure compilation produces the correct binary name.

### Error: "Tag already exists"
```
❌ Tag v1.2.0 already exists
```
**Solution**: Tag already exists, choose appropriate version type or delete tag if necessary.

### Error: "Insufficient permissions"
```
❌ Resource not accessible by integration
```
**Solution**: Check GitHub token permissions in repository settings.

### Error: "Missing dependency"
```
❌ Package libraylib-dev not found
```
**Solution**: Add missing dependency in workflow installation section.

## 📈 Best Practices

### Release Timing
- **PATCH**: Immediately for critical hotfixes
- **MINOR**: At end of sprint/iteration (weekly/biweekly)
- **MAJOR**: At end of milestone (monthly/quarterly)

### Communication
- Announce major releases in advance
- Document breaking changes
- Provide migration guides if necessary

### Testing
- Always test locally before release
- Use pre-releases for user testing
- Maintain automated test suite

### Documentation
- Keep README.md up to date
- Document new APIs
- Provide usage examples

## 🎯 Success Metrics

A successful release should have:
- ✅ Error-free compilation
- ✅ All binaries generated
- ✅ Passing unit tests
- ✅ Complete archive created
- ✅ Git tag created correctly
- ✅ GitHub release published
- ✅ Informative changelog
- ✅ Functional download

---

*These examples cover the most common use cases of the Zappy release workflow. For specific situations, consult the complete documentation in `docs/release-workflow.md`.*
