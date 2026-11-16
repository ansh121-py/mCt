# mCt

A modular, locally hosted PC control and monitoring application.

Core: C++20 · Gateway: Python FastAPI · Frontend: Vanilla JavaScript · Storage: SQLite

## Status

Phase 0: project foundation. See `mCt.md` for the full roadmap.

## Building (Windows, LLVM-MinGW)

    cmake --preset llvm-mingw-debug
    cmake --build --preset llvm-mingw-debug
    .\build\llvm-mingw-debug\core\mct_core.exe