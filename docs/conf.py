"""Configure Sphinx documentation for objctypes."""

from importlib.metadata import metadata
from importlib.metadata import version as get_version
from urllib.parse import urljoin

from packaging.version import Version

package_metadata = metadata("objctypes")

project_url_strings = package_metadata.get_all("Project-URL", [])
project_urls = {}
for url_string in project_url_strings:
    name, url = url_string.split(", ")
    project_urls[name] = url

# Project information
project = package_metadata["Name"]
# author = ?
project_copyright = "%Y"
release = get_version(project)
version = ".".join([str(num) for num in Version(release).release[:2]])

# General configuration
extensions = [
    "autoapi.extension",
    "sphinx.ext.autodoc",
    "sphinx.ext.extlinks",
    "sphinx.ext.intersphinx",
    "notfound.extension",
    "sphinx_copybutton",
]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

# Options for HTML output
html_theme = "furo"

# autoapi.extension
autoapi_dirs = ["../src"]
autoapi_python_class_content = "both"

# sphinx.ext.autodoc
autodoc_typehints = "both"

# sphinx.ext.extlinks
extlinks = {
    "issue": (
        urljoin(project_urls["Source"], "issues/%s"),
        "Issue #%s",
    ),
    "pull": (
        urljoin(project_urls["Source"], "pull/%s"),
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
