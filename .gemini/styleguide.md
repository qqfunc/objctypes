# Style Guide for Gemini Code Review

## Python

- Follow PEP 8.

## C / C++ / Objective-C

- Follow PEP 7.
- Never free memory blocks that have been allocated by functions other than
  `PyMem_Malloc()`, `PyMem_Realloc()` or `PyMem_Calloc()` with `PyMem_Free()`.
