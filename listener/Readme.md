## 🔧 Prerequisites
- [CMake](https://cmake.org/download/) (>= 3.20 recommended)
- Microsoft Visual Studio with **MSVC** toolchain  
  (Use the **x64 Native Tools Command Prompt**)

---

## ⚙️ Build Instructions

### One-time configuration
From the project root, run:
```bat
cmake -S . -B build -G "NMake Makefiles"
cd build
nmake
```

## Rebuild
From the project root, run:
```
cd build
nmake clean
cd..
cmake -S . -B build -G "NMake Makefiles"
cd build
nmake
```

## raw cmake commands NO nmake
From the project root:
```
maybe rmdir /S /Q build
cmake -S . -B build -G "NMake Makefiles"
cmake --build build --config Release
```
