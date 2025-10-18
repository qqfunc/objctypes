# Development Guide for AI Agents

## Project Structure

- `csrc/`: C / C++ / Objective-C / Objective-C++ source code
  - `objctypes.h`: C header for public function declarations.
- `src/`: Python source code / type stubs

## Setup

- Install dependencies and build project with `uv sync`
- Install Git Hooks with `uv run lefthook install`

## Coding Conventions

### Python

Follow PEP 8.

### C / C++ / Objective-C / Objective-C++

Follow PEP 7.

## Tests

Run tests with `uv run task test`.

## Commands

- Run all checks with `uv run lefthook run check`
- Fix all problems with `uv run lefthook run fix`

### editorconfig-checker

- `uv run ec`

### Typos

- `uv run typos`

### Ruff

Check and format Python code with Ruff.

#### Check Only

- `uv run ruff check`
- `uv run ruff format --check`

#### Format / Fix

- `uv run ruff check --fix`
- `uv run ruff format`

### ty

Type-check Python code with ty.

- `uv run ty check`

## Commit Messages

Follow Conventional Commits.
