"""Generate ``compile_commands.json`` from current environment."""

from __future__ import annotations

__all__ = [
    "CompileCommand",
    "generate_command",
    "generate_compile_commands",
]

import itertools
import json
from pathlib import Path
from sysconfig import get_config_var, get_path
from typing import NotRequired, Required, TypedDict

COMPILE_COMMANDS_FILE = Path("compile_commands.json")
BASE_DIR = Path(__file__).parent

C_COMPILER = get_config_var("CC")
CXX_COMPILER = get_config_var("CXX")
INCLUDE_PATH = get_path("include")

JSON_INDENT = 2


def generate_compile_commands() -> None:
    """Generate ``compile_commands.json`` from current environment.

    See https://clang.llvm.org/docs/JSONCompilationDatabase.html for
    more information about ``compile_commands.json``.
    """
    csrc_dir = BASE_DIR / "csrc"
    target_files = itertools.chain(
        csrc_dir.glob("*.c"),
        csrc_dir.glob("*.cpp"),
        csrc_dir.glob("*.m"),
    )
    with COMPILE_COMMANDS_FILE.open("w") as f:
        json.dump(
            [generate_command(file) for file in target_files],
            f,
            indent=JSON_INDENT,
        )


def generate_command(file: Path) -> CompileCommand:
    """Generate compile commands for the specified file."""
    compiler = CXX_COMPILER if file.name.endswith("cpp") else C_COMPILER
    return {
        "directory": str(BASE_DIR),
        "file": str(file.relative_to(BASE_DIR)),
        "arguments": [compiler, f"-I{INCLUDE_PATH}", "-Weverything"],
    }


class CompileCommand(TypedDict):
    """Compile command dictionary for ``compile_commands.json``."""

    directory: Required[str]
    file: Required[str]
    arguments: Required[list[str]]
    output: NotRequired[str]


if __name__ == "__main__":
    generate_compile_commands()
