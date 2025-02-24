"""setup.py: Set configurations of setuptools."""

import itertools
import sys
from pathlib import Path
from subprocess import check_output

from setuptools import Extension, setup

if sys.platform == "darwin":
    C_EXT_PATH = Path("csrc")
    BREW_PATH = Path("/opt/homebrew/bin/brew")
    BREW_PREFIX = (
        check_output(
            ["/opt/homebrew/bin/brew", "--prefix"],
            text=True,
        ).removesuffix("\n")
        if Path("/opt/homebrew/bin/brew").is_file()
        else None
    )
    setup(
        ext_modules=[
            Extension(
                name="objctypes",
                sources=itertools.chain(
                    C_EXT_PATH.glob("*.c"),
                    C_EXT_PATH.glob("*.cpp"),
                ),
                include_dirs=None
                if BREW_PREFIX is None
                else [f"{BREW_PREFIX}/include"],
                library_dirs=None
                if BREW_PREFIX is None
                else [f"{BREW_PREFIX}/lib"],
                libraries=None if BREW_PREFIX is None else ["ffi"],
            ),
        ],
    )
else:
    setup()
