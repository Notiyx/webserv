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

# Traiter POST
if method == "POST":
    form = cgi.FieldStorage()
    print("<h2>Données POST reçues :</h2>")
    if not form:
        print("<p>Aucune donnée POST reçue</p>")
    else:
        print("<ul>")
        for key in form.keys():
            value = form.getvalue(key)
            print(f"<li>{key} = {value}</li>")
        print("</ul>")

print("</body></html>")