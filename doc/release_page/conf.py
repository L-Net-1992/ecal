# -*- coding: utf-8 -*-
#
# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys

# -- Generate download archive and tables for the homepage --------------------
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, r'../extensions')

import generate_release_documentation

gh_api_key = os.getenv("ECAL_GH_API_KEY")
if gh_api_key:
    release_page_dir = "."
    index_file_path = "./index.rst"
    generate_release_documentation.generate_release_documentation(gh_api_key, index_file_path, release_page_dir)
else:  
    print("ERROR: Environment variable ECAL_GH_API_KEY not set. Skipping generating download tables.")
    exit(1)

# -- Project information -----------------------------------------------------

project = u'Eclipse eCAL™'
copyright = u'2023, Continental'
#author = u'Continental'

# The short X.Y version
# version = u''
# The full version, including alpha/beta/rc tags
# release = u''

# -- General configuration ---------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx_book_theme',
    'sphinx_tabs.tabs',
    'sphinx.ext.githubpages',
]

# Tell sphinx what the primary language being documented is.
#primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
#highlight_language = 'cpp'


# Add any paths that contain templates here, relative to this directory.
templates_path = ['../_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = [u'_build', 'Thumbs.db', '.DS_Store']

# The name of the Pygments (syntax highlighting) style to use.
# pygments_style = 'sphinx'

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
html_theme = 'sphinx_book_theme'


# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['../_static']

html_css_files = [
    'css/bignums.css',                              # Enable the bignum feature from the sphinx-typo3-theme
    'css/sphinx-book-theme-1.1.2-ecaladdon.css',    # Change colors of the sphinx-book-theme
    'css/tabs-3.4.5-ecaladdon.css',                 # Change colors of the sphinx-tabs
]

html_title = "Eclipse eCAL™"
html_logo = "../_static/img/ecal-logo.svg"
html_favicon = "../_static/img/favicon.png"

html_theme_options = {
    "logo_only": True,
    "show_navbar_depth": 1,
    "show_toc_level": 2,
    "repository_url": "https://github.com/eclipse-ecal/ecal/",
    "use_repository_button": False,
    "use_issues_button": False,
    "use_edit_page_button": False,
    "repository_branch": "master",
    "path_to_docs": "doc/rst/",
    "extra_navbar": "", # => Remove the default text
    "footer_start": ["footer.html"],
    "extra_footer": '',
}

# -- Options for HTMLHelp output ---------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'eCALdoc'

# C++ defines used in function definitions
cpp_id_attributes = ['ECAL_API', 'ECALTIME_API']
