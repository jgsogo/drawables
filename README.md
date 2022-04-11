[![Build and tests](https://github.com/jgsogo/drawables/actions/workflows/ci.yml/badge.svg?event=push)](https://github.com/jgsogo/drawables/actions/workflows/ci.yml)
[![Build and tests](https://github.com/jgsogo/drawables/actions/workflows/conan_package.yml/badge.svg?event=push)](https://github.com/jgsogo/drawables/actions/workflows/ci.yml)

## Consume the library

This library is available as a Conan package in my personal remote repository. To consume it, you just
need to add the remote:


<!-- TODO: Use your library/version here -->
```
conan remote add sogo https://sogo.jfrog.io/artifactory/api/conan/conan-center
conan install drawables/<library_version>@jgsogo/stable
```

Or, add the remote and list the reference in your `conanfile.txt/py` file.


## Build and run locally

```
mkdir cmake-build-xxxx && cd cmake-build-xxxx
```

```
conan lock create --profile:host=../.conan/profiles/cpp20 --profile:build=default --lockfile=../lockfile.json --lockfile-out=lockfile.json --name=drawables --version=0.1 ../conanfile.txt --build --update
```

```
conan install --lockfile=lockfile.json ../conanfile.txt --build=missing --generator=virtualenv
```

## Update dependencies

```
conan lock create --name=drawables --version=0.1 --base --lockfile-out lockfile.json --build --profile:host=.conan/profiles/cpp20 --profile:build=default conanfile.txt
```
