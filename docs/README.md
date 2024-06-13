---
description: General info about Algolite
---

# 📘 General

## Algolite

Software for algorithm's complexity analysis, based on developed language Algolite.

### Installation

For example, you can use **g++** as a software to compile Algolite code to C++. Anyway, **g++** is required to compile C++ code to executable file.

### Using

1. Write code in _**SourceAlgolite.txt**_ using Algolite syntax.
2. Compile transpiler program. For example, for Windows you can use:

```
g++ AlgoliteTranspiler.cpp TranslatingVisitor.cpp -std=c++20 -O3 -o transpile_process.exe
```

For Unix you can use:

```
g++ AlgoliteTranspiler.cpp TranslatingVisitor.cpp -std=c++20 -O3 -o transpile_process.out
```

3. Execute compiled file and get _**finalOutput.cpp**_.
4. For better appearence of _**finalOutput.cpp**_ you can use **uncrustify** with _**msvc.cfg**_ (or use custom config). For example, you can use:

```
uncrustify -c msvc.cfg -f finalOutput.cpp --no-backup -o finalOutput.cpp
```

You can install **uncrustify** from [here](https://github.com/uncrustify/uncrustify).

5. Compile _**finalOutput.cpp**_. For example, for Windows you can use:

```
g++ -O3 -std=c++20 -o output_program.exe finalOutput.cpp pbPlots.cpp supportLib.cpp -lm -D _WIN32_WINNT=0x0A00
```

For Unix you can use:

```
g++ -O3 -std=c++20 -o output_program.out finalOutput.cpp pbPlots.cpp supportLib.cpp
```

6. Execute compiled file and get result.

### Result

As the result you will get a **.png** picture with description of algorithm's complexity.
