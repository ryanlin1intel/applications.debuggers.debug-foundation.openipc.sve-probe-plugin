# Reference Probe Plugin



## Using the Probe Plugin Interface (PPI)
The header files that define the interface for the plugin to implement are in the include directory. These can be made available to your project with CMake's FetchContent or by simply copying them into your project.

**Using with CMake FetchContent**
```cmake
include(FetchContent)
FetchContent_Declare(
  plugin_interface
  GIT_REPOSITORY https://github.com/intel-innersource/applications.debuggers.debug-foundation.openipc.reference-probe-plugin
  GIT_TAG        <insert tag or commit hash>
)
FetchContent_MakeAvailable(plugin_interface)

...

target_link_libraries(<YourPluginTargetName> PRIVATE OpenIPC::PluginInterface)
```

For details on implementing the PPI methods see the [documentation](https://goto.intel.com/ppidocs)

## Compiling the Reference Plugin

 **Prerequisites**: Visual Studio 2022. CMake 3.24 or later.

```
git clone https://github.com/intel-innersource/applications.debuggers.debug-foundation.openipc.reference-probe-plugin.git reference-probe-plugin
cd reference-probe-plugin
cmake.exe --workflow --preset default
```

