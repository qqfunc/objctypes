# Style Guide for Gemini Code Review

## Python

- Follow PEP 8.

## C / C++ / Objective-C

- Follow PEP 7.
- Declare variables and constants in the middle of a function as needed.

### Python C API

- Use `PyDoc_STR()` rather than `PyDoc_STRVAR()`.

#### Memory Management

- Use `PyMem_New()` rather than `PyMem_Malloc()` if possible.
- Use `PyMem_Resize()` rather than `PyMem_Realloc()` if possible.
- Use `PyMem_Free()` for memory blocks that have been allocated with
  `PyMem_Malloc()`, `PyMem_Calloc()` or `PyMem_Realloc()`.
- Use `PyMem_Del()` for memory blocks that have been allocated with
  `PyMem_New()` or `PyMem_Resize()`.
- Never resize or free memory blocks that have been allocated by functions other
  than `PyMem_Malloc()`, `PyMem_Realloc()`, `PyMem_Calloc()`, `PyMem_New()` or
  `PyMem_Resize()` with `PyMem_Realloc()`, `PyMem_Free()`, `PyMem_Resize()` or `PyMem_Del()`.
