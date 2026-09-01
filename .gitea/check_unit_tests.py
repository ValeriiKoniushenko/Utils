#!/usr/bin/env python3
"""Utils entry point for shared unit-test execution and Gitea reporting."""

from ci.unit_tests import main


if __name__ == "__main__":
    main(default_executable="build/bin/UtilsTests")
