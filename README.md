Github-CI:<br>
[![Build Status][amd64_linux_status]][amd64_linux_link]
[![Build Status][arm64_macos_status]][arm64_macos_link]
[![Build Status][amd64_macos_status]][amd64_macos_link]
[![Build Status][amd64_windows_status]][amd64_windows_link]<br>

[![Build Status][amd64_docker_status]][amd64_docker_link]
[![Build Status][arm64_docker_status]][arm64_docker_link]
[![Build Status][riscv64_docker_status]][riscv64_docker_link]<br>

[amd64_linux_status]: ./../../actions/workflows/amd64_linux.yml/badge.svg
[amd64_linux_link]: ./../../actions/workflows/amd64_linux.yml
[arm64_macos_status]: ./../../actions/workflows/arm64_macos.yml/badge.svg
[arm64_macos_link]: ./../../actions/workflows/arm64_macos.yml
[amd64_macos_status]: ./../../actions/workflows/amd64_macos.yml/badge.svg
[amd64_macos_link]: ./../../actions/workflows/amd64_macos.yml
[amd64_windows_status]: ./../../actions/workflows/amd64_windows.yml/badge.svg
[amd64_windows_link]: ./../../actions/workflows/amd64_windows.yml

[amd64_docker_status]: ./../../actions/workflows/amd64_docker.yml/badge.svg
[amd64_docker_link]: ./../../actions/workflows/amd64_docker.yml
[arm64_docker_status]: ./../../actions/workflows/arm64_docker.yml/badge.svg
[arm64_docker_link]: ./../../actions/workflows/arm64_docker.yml
[riscv64_docker_status]: ./../../actions/workflows/riscv64_docker.yml/badge.svg
[riscv64_docker_link]: ./../../actions/workflows/riscv64_docker.yml


# Introduction

<nav for="project"> |
<a href="#requirement">Requirement</a> |
<a href="#codemap">Codemap</a> |
<a href="#build-project">Build</a> |
<a href="#test-project">Test</a> |
<a href="#install-project">Install</a> |
<a href="ci/README.md">CI</a> |
<a href="#license">License</a> |
</nav>

Unpacker for Kalisto data files (`.KBF`/`.KIX`) found in the "New York Race" (NYR) video game.

NYR is a flying car racing game developed by Kalisto Entertainment and released in 2001.
It is based on [The Fifth Element](https://en.wikipedia.org/wiki/The_Fifth_Element) film.

smx:
> Based on my discoveries, the KBF extension stands for "Kalisto Binary File" and KIX stands for "Kalisto Index File".
I don't own any other Kalisto-made game to verify if other games also use the same binary format, but yo're free to try this tool and see if it works for you

## Requirement

You'll need:

* "CMake >= 3.16".

## Codemap

The project layout is as follow:

* [CMakeLists.txt](CMakeLists.txt) Top-level for [CMake](https://cmake.org/cmake/help/latest/) based build.
* [cmake](cmake) Subsidiary CMake files.

* [ci](ci) Root directory for continuous integration.

* [libkunpacker](libkunpacker) Root directory for `libkunpacker` library.
  * [CMakeLists.txt](libkunpacker/CMakeLists.txt) for `libkunpacker`.
  * [include](libkunpacker/include) public folder.
    * [kunpacker.hpp](libkunpacker/include/kunpacker.hpp)
  * [src](libkunpacker/src) private folder.
    * [kunpacker.cpp](libkunpacker/src/kunpacker.cpp)
* [kunpacker](kunpacker) Root directory for `kunpacker` cli application.
  * [CMakeLists.txt](kunpacker/CMakeLists.txt) for `kunpacker`.
  * [src](kunpacker/src) private folder.
    * [main.cpp](kunpacker/src/main.cpp)

# Build Project
To build the C++ project, as usual:
```sh
cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target all -v
```

note: replace `all` by `ALL_BUILD` for non makefile generators.

# Test Project
To test the C++ project, as usual:
```sh
cmake --build build --config Release --target test -v
```

note: replace `test` by `RUN_TESTS` for non makefile generators.

# Install Project
To install the C++ project, as usual:
```sh
cmake --build build --config Release --target install -v
```

note: replace `install` by `INSTALL` for non makefile/xcode generators.

## Appendices

Few links on the subject...

### Resources

Project layout:
* [The Pitchfork Layout Revision 1 (cxx-pflR1)](https://github.com/vector-of-bool/pitchfork)

CMake:
* https://llvm.org/docs/CMakePrimer.html
* https://cliutils.gitlab.io/modern-cmake/
* https://cgold.readthedocs.io/en/latest/

# Contributing

The [CONTRIBUTING.md](./CONTRIBUTING.md) file contains instructions on how to
file the Contributor License Agreement before sending any pull requests (PRs).
Of course, if you're new to the project, it's usually best to discuss any
proposals and reach consensus before sending your first PR.

## License

Apache 2. See the LICENSE file for details.
