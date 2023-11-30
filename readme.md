Todo_app_cpp_desktop
=====================
# Requirements
- Microsoft Visual Studio 2022 with v143 toolset (default for VS 2022)
- MSBuild - usually part of VS installation
- CMake >= 3.10

# Todo app for desktop (C++)
This is a simple todo app for desktop written in C++ using ImGui and SQLite3. Currently it can be compiled only on Windows.
You can add, edit and delete tasks. You can also mark them as done. The tasks are saved in a database file.
To pull the repository, you need to have git-lfs installed.

Use the following commands to clone the repository:
```
git clone https://github.com/BSadovnykovC/todo_app_cpp_desktop.git
```

To compile the project, you need to have CMake installed. Use the following commands in VS Dev command prompt to compile the project:
```
mkdir build
cd build
cmake ..
msbuild  todo_app_cpp_desktop.sln /target:todo_app_cpp_desktop /property:Configuration=Release
```
To run the project, use the following command:
```
.\Release\todo_app_cpp_desktop.exe
```


or enter with the mouse in the folder Release and run todo_app_cpp_desktop.exe

To build tests, use the following commands in VS Dev command prompt:
```
mkdir build
cd build
cmake ..
msbuild  todo_app_cpp_desktop.sln /target:todo_app_cpp_desktop_test /property:Configuration=Release
```
To run tests, use the following command:
```
.\Release\todo_app_cpp_desktop_test.exe
```

or enter with the mouse in the folder Release and run todo_app_cpp_desktop_test.exe
