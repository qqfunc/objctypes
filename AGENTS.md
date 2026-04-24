# Development Guide for AI Agents

## Project Structure

- `csrc/`: C / C++ / Objective-C / Objective-C++ source code
  - `objctypes.h`: C header for public function declarations.
- `src/`: Python source code / type stubs

## Setup

- Install dependencies and build project with `uv sync`
- Setup the project environment with `uv run task setup`

## Coding Conventions

### Python

Follow PEP 8.

### C / C++ / Objective-C / Objective-C++

Follow PEP 7.

## Tests

Run tests with `uv run task test`.

## Commands

- Run all checks with `uv run task check`
- Fix all problems with `uv run task fix`

## Commit Messages

Follow Conventional Commits.
