#!/usr/bin/env python3
import cgi
import os
import sys

form = cgi.FieldStorage()

fileitem = form['file']

if fileitem.filename:
    filename = os.path.basename(fileitem.filename)
    filepath = os.path.join("front/uploads/", filename)
    with open(filepath, 'wb') as f:
        f.write(fileitem.file.())
