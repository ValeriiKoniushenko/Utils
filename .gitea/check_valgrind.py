#!/usr/bin/env python3
"""Utils entry point for the shared Valgrind test checker."""

from ci.valgrind import main


if __name__ == "__main__":
    main(default_executable="build/bin/UtilsTests")
