#!/usr/bin/env python3 #chatGPT
import cgi
import os
import sys

print("Content-Type: text/html")
print()

print("<html><body>")
print("<h1>Test CGI Python</h1>")

# Afficher la méthode HTTP
method = os.environ.get("REQUEST_METHOD", "")
print(f"<p>Méthode HTTP : {method}</p>")

# Afficher la query string (GET)
query = os.environ.get("QUERY_STRING", "")
print(f"<p>Query string : {query}</p>")

print("</body></html>")