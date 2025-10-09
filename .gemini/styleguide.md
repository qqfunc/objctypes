# Style Guide for Gemini Code Review

## Python

- Follow PEP 8.

## C / C++ / Objective-C

- Follow PEP 7.

### Python C API

- Use `PyMem_New()` rather than `PyMem_Malloc()` if possible.
- Use `PyMem_Resize()` rather than `PyMem_Realloc()` if possible.
- Use `PyMem_Free()` rather than `PyMem_Del()`.
- Never free memory blocks that have been allocated by functions other than
  `PyMem_Malloc()`, `PyMem_Realloc()` or `PyMem_Calloc()` with `PyMem_Free()`.

