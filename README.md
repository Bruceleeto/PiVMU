
<!-- PROJECT LOGO -->
<br />



very wip. 


# Building #
Building is done with standard CMake. You should be able to open CMakeLists.txt directly as a project file in most IDEs such as XCode, Qt Creator, CLion, etc if you wish to build from a UI.

First, ensure submodules are installed with:
```
git submodule update --init --recursive
```

To build the project and its unit tests from the command-line, you can do the following:
```
mkdir build
cd build
cmake -DEVMU_ENABLE_TESTS=ON ..
cmake --build . 
```

# Credits #
Author
- Falco Girgis

Contributors
- Colton Pawielski
- jvsTSX

Collaborators 
- Andrew Apperley
- Patrick Kowalik
- Walter Tetzner
- Tulio Goncalves
- Kresna Susila 
- Sam Hellawell

Special Thanks
- Marcus Comstedt
- Ruslan Rostovtsev
- Shirobon
- Deunan Knute
- Dmitry Grinberg
- RinMaru
- UltimaNumber
- Joseph-Eugene Winzer
