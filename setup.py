"""setup.py: Set configurations of setuptools."""

from pathlib import Path
from subprocess import check_output

from setuptools import Extension, setup

C_EXT_PATH = Path("csrc")
BREW_PREFIX = check_output(
    ["/opt/homebrew/bin/brew", "--prefix"],
    text=True,
).removesuffix("\n")

setup(
    ext_modules=[
        Extension(
            name="objctypes",
            sources=C_EXT_PATH.glob("*.c") + C_EXT_PATH.glob("*.cpp"),
            include_dirs=[f"{BREW_PREFIX}/include"],
            library_dirs=[f"{BREW_PREFIX}/lib"],
            libraries=["ffi"],
        ),
    ],
)
