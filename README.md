# frog-engine

<div align="center">
    <a href="https://github.com/Bleskocvok/frog-engine">
        <img width="275" height="227" src="frog.png" alt="frog illustration">
    </a>
    <!-- ![Illustration](frog.png) -->
</div>



Simple game engine in C++.

[![Build 2D](https://github.com/Bleskocvok/frog-engine/actions/workflows/build.yml/badge.svg)](https://github.com/Bleskocvok/frog-engine/actions/workflows/build.yml)
[![Build 3D](https://github.com/Bleskocvok/frog-engine/actions/workflows/build-3d.yml/badge.svg)](https://github.com/Bleskocvok/frog-engine/actions/workflows/build-3d.yml)
[![Tests](https://github.com/Bleskocvok/frog-engine/actions/workflows/test.yml/badge.svg)](https://github.com/Bleskocvok/frog-engine/actions/workflows/test.yml)
[![coverage badge](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/wiki/Bleskocvok/frog-engine/lines.md)](https://github.com/Bleskocvok/frog-engine/actions/workflows/loc.yml)


## Table of contents

-   [frog-engine](#frog-engine)
    -   [Table of contents](#table-of-contents)
    -   [Build examples](#build-examples)
        -   [3D example](#3d-example)
        -   [2D example](#2d-example)
    -   [Folder structure:](#folder-structure)
        -   [1. Engine](#1-engine)
        -   [2. Game](#2-game)
    -   [Notes](#notes)
        -   [Windows build
            instructions](#windows-build-instructions)
    -   [Misc](#misc)


## Build examples

1. Clone repository and install engine dependencies.

    ```sh
    git clone https://github.com/Bleskocvok/frog-engine.git
    cd frog-engine
    sh ./script/libraries.sh
    ```

    Folder `game` contains an example 3D game and folder `game2d` contains a sample of
    2D game code.

### 3D example

1. The build might need additional system packages installed:
`libgl1-mesa-dev`, `mesa-common-dev`, `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev` and
`libxi-dev` *should* cover it. These will be probably in your package manager's repository.

2. It can be build by using the helper `build` script.

    ```sh
    sh ./script/build -o bin-game -b Release gen-build game
    ```

3. Or by manually invoking `cmake` commands.

    ```sh
    cmake -S game -B bin-game
    cmake --build bin-game -j4
    ```

4. To run the compiled example, execute the `Game` binary.

    ```sh
    ./bin-game/Game
    ```

### 2D example

1. It can be build by using the helper `build` script.

    ```sh
    sh ./script/build -o bin-game2d -b Release gen-build game2d
    ```

2. Or by manually invoking `cmake` commands.

    ```sh
    cmake -S game2d -B bin-game2d
    cmake --build bin-game2d -j4
    ```

3. To run the compiled example, execute the `Game` binary.

    ```sh
    ./bin-game2d/game_2d
    ```


## Folder structure:

### 1. Engine

- `./engine/`
    the whole engine

- `./engine/src/`
    the engine source files

- `./engine/tests/`
    tests for linear algebra calculations

- `./engine/docs/`
    generated documentation using Doxygen, see
    `./engine/docs/html/index.html`

- `./engine/libraries/`
    contains a folder for each additional library used by the engine

    - `GLFW/` (https://www.glfw.org/)
    - `glad/` (https://glad.dav1d.de/)
    - `stb_image/` (https://github.com/nothings/stb/blob/master/stb_image.h)
    - `tiny_obj_loader/` (https://github.com/tinyobjloader/tinyobjloader)
    - `doctest/` (https://github.com/doctest/doctest)
<br>

- `./engine/assets/`
    the engine assets such as shaders and font atlas

### 2. Game

- `./src/`
    source files of the game

- `./game_assets/`
    textures used in the game

- `./bin-win/Game.exe` and `./bin-lin/Game`
    the resulting compiled game for Windows and Linux respectively

- `./out/build/x64-Debug/Debug/Game.exe`
    the resulting compiled game for Windows when compiled in Visual Studio

## Notes

1. The engine is wrapped in the namespace `frog` and individual
    components are separated into nested namespaces:

    - `frog` - core engine part
    - `frog::geo` - geometry & maths: matrix, vector, etc.
    - `frog::gl` - graphics stuff: rendering, mesh, mesh generation
    - `frog::os` - system stuff: window creation, events handling
<br>

2. **Font** in `./engine/assets/font.png` is based off of **my own font** from
    [this repository](https://github.com/Bleskocvok/project_jumpy_tanx/blob/main/models/beautifont.png)

3. The architecture used for the engine is inspired by Unity:
    - the engine follows this structure:
    `engine` -> `scene_manager` -> `scene` -> `game_object` -> `script`

### Windows build instructions

#### a. Using Visual Studio 🤮

- It should **just work** in VS 2019 (the C++ VS
    components need to be installed in VS Installer)

- Make sure to select the desired target (`Game.exe` or `game_2d.exe`)

#### b. Using command line

1. Install necessary tools: `cmake`, `mingw64`

2. In `PowerShell`, enter the root folder of the project;
    If you perform the `ls` command, you should see files
    such as `CMakeLists.txt`, `README.md`, `engine/`, ...

3. Make sure the folder `bin-win` is empty

4. Call `script/build.bat`

5. Pray that it works; if not, return to step 1. and try to reconfigure
    the installation of `cmake` or `mingw`

6. If it worked, after it has finished you should be able to open
    `bin-win/Game.exe`, which starts the game

<!--
### Linux (Ubuntu)

1. Install `cmake` and `gcc`/`clang`

2. Install these packages and hope that it covers all
    necessary dependencies:
    - `sudo apt install libgl1-mesa-dev`
    - `sudo apt install mesa-common-dev`
    - `sudo apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev`
<br>

3. In `shell` enter the root folder of the project;
    If you perform the `ls` command, you should see files
    such as `CMakeLists.txt`, `README.md`, `engine/`, ...

4. Call `script/build.sh`

5. After it has finished you should be able to execute `bin-lin/Game`,
    which starts the game -->

## Misc

Originally made as a project assignment.

Link to the original repository [pa199-project](https://gitlab.fi.muni.cz/xbrablik/pa199-project)

#### Development tracking

- [Devlog](devlog.md)

- [TODO](TODO.md)


Life is pointless.

## TODO

- Fixed floating type arithmetic functions (sin, cos, sqrt, …)
- Android build support
- Net code support for multiplayer
- Refactor to use more namespaces
- Test fixed thoroughly
- Packaging android and linux support
- More fixed variants (uint-based, or int-based, overflow-safe)
- Unify 2d/3d interface
