# -*- coding: utf-8 -*-

import sys, os
sys.path.append(os.path.join(os.getcwd(), os.path.dirname(__file__)))

extensions = ["proty"]
templates_path = []
source_suffix = ".rst"
source_encoding = "utf-8-sig"
master_doc = "index"

project = u"Proty"
copyright = u"2010-2012, Thomas Gatzweiler"

version = "0.5"
release = version

#language = None
primary_domain = "pr"

today_fmt = "%B %d, %Y"

exclude_patterns = []
add_module_names = True
show_authors = False

pygments_style = "sphinx"
modindex_common_prefix = []

html_theme = "default"
html_theme_options = {}
html_theme_path = []

html_title = "Proty v%s documentation" % version
html_short_title = html_title
html_logo = None
html_favicon = None
html_static_path = []
html_last_updated_fmt = "%b %d, %Y"
html_use_smartypants = True
html_sidebars = {}
html_additional_pages = {}
html_domain_indices = True
html_use_index = True
html_split_index = False
html_show_sourcelink = True
html_show_sphinx = True
html_show_copyright = True
html_use_opensearch = ""
html_file_suffix = ".html"
htmlhelp_basename = "protydoc"

latex_paper_size = "a4"
latex_font_size = "10pt"

latex_documents = [
  ("index", "proty.tex", u"Proty Documentation",
   u"Thomas Gatzweiler", "manual"),
]

latex_logo = None
latex_use_parts = False
latex_show_pagerefs = False
latex_show_urls = False
latex_preamble = ""
latex_appendices = []
latex_domain_indices = True


man_pages = [
    ("index", "proty", u"Proty Documentation",
     [u"Thomas Gatzweiler"], 1)
]
