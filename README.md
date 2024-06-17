
<!-- PROJECT LOGO -->
<br />



https://github.com/Bruceleeto/PiVMU/assets/103404050/1f5d6d7c-fcd9-40a8-b896-bbf03651977e

libgimbal causes a lot of errors. that i've suppressed on my local build to get to run that can cause this to crash I believe.
Audio is fucked but sometimes it does play even if it's not accurate.
speed is prob jank. 
lot of issues. i had a little go of getting this somewhat functional and its good enough. 

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
