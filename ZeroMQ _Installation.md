# ZeroMQ (cppzmq) Installation Guide

This document provides step-by-step instructions to install **ZeroMQ** and **cppzmq** on **Linux** and **Windows (MinGW + Qt)** and configure them using **CMake**.

---

## 📦 Supported Platforms

- **Linux** (Ubuntu / Debian-based)
- **Windows 10 / 11** (Qt + MinGW)

------------- **Linux** ----------------------------------------------------

## 🐧 Linux Installation (Ubuntu / Debian)

### 1️⃣ Install Dependencies
```bash
sudo apt update
sudo apt install -y pkg-config libzmq3-dev

### 2️⃣ ✔ Verify Installation
```bash
pkg-config --modversion libzmq


------------- **Windows** ----------------------------------------------------

#####  1️⃣ Clone vcpkg
```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

#####  2️⃣ Bootstrap vcpkg
```powershell
.\bootstrap-vcpkg.bat


#####  3️⃣ Add MinGW to PATH
```powershell
$env:PATH="C:\Qt\Tools\mingw1310_64\bin;$env:PATH"

#####  ✔ Verify Compiler
```powershell
gcc --version


#####  4️⃣ Install ZeroMQ
```powershell
.\vcpkg.exe install zeromq:x64-mingw-dynamic --host-triplet=x64-mingw-dynamic


#####  5️⃣ Install cppzmq (C++ Bindings)
```powershell
.\vcpkg.exe install cppzmq:x64-mingw-dynamic --host-triplet=x64-mingw-dynamic

------------------------------------------

#####    Add the ZeroMQ installation path in your CMakeLists.txt

set(ZeroMQ_DIR "D:/github/vcpkg/installed/x64-mingw-dynamic/share/ZeroMQ")

