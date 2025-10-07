"""setup.py: Set configurations of setuptools."""

import itertools
import sys
from pathlib import Path
from subprocess import check_output

from setuptools import Extension, setup

if sys.platform == "darwin":
    C_EXT_PATH = Path("csrc")
    BREW_PATH = Path("/opt/homebrew/bin/brew")
    LIBFFI_PREFIX = (
        check_output(
            ["/opt/homebrew/bin/brew", "--prefix", "libffi"],
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
                if LIBFFI_PREFIX is None
                else [f"{LIBFFI_PREFIX}/include"],
                library_dirs=None
                if LIBFFI_PREFIX is None
                else [f"{LIBFFI_PREFIX}/lib"],
                libraries=None if LIBFFI_PREFIX is None else ["ffi"],
            ),
        ],
    )
else:
    setup()
