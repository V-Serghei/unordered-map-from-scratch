# Unordered Map From Scratch

A learning implementation of an `std::unordered_map`-like associative container in modern C++.

The project explores how a hash table can store key-value pairs, resolve collisions, rehash buckets, and expose an iterator-based interface similar to the C++ Standard Library.

## Features

- Template-based key-value container: `UnorderedMAP<Key, Data>`
- Hash table storage with separate chaining
- Basic bidirectional iterator support
- Element access through `operator[]` and `at`
- Insertion through `emplace`, `insert`, `try_emplace`, and `insert_or_assign`
- Lookup through `find`
- Erase, clear, reserve, rehashing, merge, and load factor support
- Example comparison with `std::unordered_map`

## Project Status

This is an educational implementation, not a drop-in replacement for `std::unordered_map` yet.

Implemented parts cover the core idea of a hash map, but the public API, iterator behavior, const-correctness, return types, memory management, bucket interface, allocator support, and edge cases still need work before the container can behave like the standard library version.

## Build

Requirements:

- CMake 3.26 or newer
- C++20-compatible compiler

```bash
cmake -S . -B build
cmake --build build
```

Run:

```bash
./build/unordered_map_final
```

On Windows, the executable may be:

```bash
build/unordered_map_final.exe
```

## Current Structure

```text
.
|-- CMakeLists.txt
|-- README.md
|-- main.cpp
`-- .gitignore
```

At the moment, the implementation and example usage live in `main.cpp`.

## Roadmap

- Align method signatures and return types with `std::unordered_map`
- Add const iterators and local bucket iterators
- Add `contains`, `count`, `equal_range`, bucket API, and observer API
- Improve rehashing and load factor policy
- Add copy/move constructors and assignments with correct ownership behavior
- Add unit tests for insertion, deletion, collisions, rehashing, and iterator validity
- Move implementation out of `main.cpp` into reusable headers

## License

No license has been selected yet.
