# TeknoFfb
TeknoFfb is the FFB plugin for TeknoParrot, allowing users to modify the way they feel when they drive in certain supported titles.

# How to compile
You will need Visual Studio to compile the project, [premake5](http://premake.github.io/) (place the exe in the project directory) to generate the project files and [SDL2](https://www.libsdl.org/download-2.0.php) "development libraries" which you will place in `deps\inc\SDL2` (rename 'include' directory) and `deps\lib` (SDL2.lib).

After you've got the pre-requisites, open a command prompt in the project directory and run `premake5 vs2017` (or whatever version of visual studio you have) - a `TeknoFfb.sln` file will be generated. Open that with visual studio, do any edits you wish, and compile it to a dll.

# Usage
Place the compiled `ffbplugin.dll` into the **game's** directory (not TeknoParrot's!)

# Supported games

 - Initial D 6 AA
 - Initial D 7 AA X