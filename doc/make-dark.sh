#!/bin/sh


HEADER="s|\(HTML_HEADER\s*=\).*\$|\1 \"dark/doxygen-dark-theme/html_header.html\"|"
FOOTER="s|\(HTML_FOOTER\s*=\).*\$|\1 \"dark/doxygen-dark-theme/html_footer.html\"|"
CSS="s|\(HTML_EXTRA_STYLESHEET\s*=\).*\$|\1 \"dark/doxygen-dark-theme/custom.css\" \"dark/doxygen-dark-theme/custom_dark_theme.css\"|"
OUT="s|\(OUTPUT_DIRECTORY\s*=\).*\$|\1 doc-dark|"

sed -e "$HEADER" -e "$FOOTER" -e "$CSS" -e "$OUT" Doxyfile >Doxyfile-dark

