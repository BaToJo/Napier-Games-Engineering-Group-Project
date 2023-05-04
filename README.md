## How to build _Cartharsis_ from this source code

1. Install _Git_, _CMake_ and _Visual Studio_ if you do not already have them.

2. Clone this repo.

3. Navigate a _Command Prompt_ or _Powershell_ instance to the folder into which you cloned this repo, and then run the following commands in order:
 - `git submodule add https://github.com/SFML/SFML.git lib/sfml`
 - `git submodule add https://github.com/dooglz/Box2D.git lib/b2d`
 - `git submodule init`
 - `git submodule update`

4. Use _CMake_ to generate and build from the `CMakeLists.txt` file provided in the root repo folder you cloned. This project was developed using _Visual Studio 17 (2022)_.

5. Use _Visual Studio_ to open `Cartharsis.sln` in the root of the output folder you built with _CMake_.

6. In _Visual Studio_, set `CARTHARSIS` as your startup project.

You should now be able to build and run the solution.
