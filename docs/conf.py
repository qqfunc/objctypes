"""Configure Sphinx documentation for objctypes."""

import tomllib
from importlib.metadata import version as get_version
from pathlib import Path
from urllib.parse import urljoin

from packaging.version import Version

PYPROJECT_TOML_PATH = Path("../pyproject.toml")

with PYPROJECT_TOML_PATH.open("rb") as f:
    pyproject = tomllib.load(f)

# Project information
project = pyproject["project"]["name"]
author = "qqfunc"
project_copyright = "2025, qqfunc"
release = get_version(project)
version = ".".join([str(num) for num in Version(release).release[:2]])

# General configuration
extensions = [
    "autoapi.extension",
    "sphinx.ext.autodoc",
    "sphinx.ext.extlinks",
    "sphinx.ext.intersphinx",
    "hoverxref.extension",
    "notfound.extension",
    "sphinx_copybutton",
]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

# Options for HTML output
html_theme = "furo"

# autoapi.extension
autoapi_dirs = ["../src"]

# sphinx.ext.autodoc
autodoc_typehints = "both"

# sphinx.ext.extlinks
extlinks = {
    "issue": (
        urljoin(pyproject["project"]["urls"]["Repository"], "%s"),
        "Issue #%s",
    ),
    "pull": (
        urljoin(pyproject["project"]["urls"]["Repository"], "%s"),
        "Pull Request #%s",
    ),
}
extlinks_detect_hardcoded_links = True

# sphinx.ext.intersphinx
intersphinx_mapping = {"python": ("https://docs.python.org/", None)}
intersphinx_cache_limit = 1
intersphinx_timeout = 30

# sphinx.ext.viewcode
viewcode_line_numbers = True

# sphinx-hoverxref
hoverxref_auto_ref = True
