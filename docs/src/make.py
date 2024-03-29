#!/usr/bin/env python

from math import modf
from sys import argv
from subprocess import run
from pathlib import Path
from time import time

SOURCEDIR = "."
BUILDDIR = "src/build/html"
SPHINXOPTS = []
SPHINXBUILD = "sphinx-build"
TARGET = argv[1] if len(argv) == 2 else "html"

start = time()

Path(BUILDDIR).mkdir(parents=True, exist_ok=True)

run(
    [SPHINXBUILD, "-c", "src", "-Tqb", TARGET, "-j", "auto", SOURCEDIR, BUILDDIR]
    + SPHINXOPTS,
    check=True,
)
end = time()

millis, seconds = modf(end - start)
print(f"Built in {int(seconds)}s {int(millis * 1000)}ms")
