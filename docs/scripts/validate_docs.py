#!/usr/bin/env python3

from __future__ import annotations

import os
import re
import sys
from pathlib import Path


DOCS_ROOT = Path(__file__).resolve().parent.parent
REPOSITORY_ROOT = DOCS_ROOT.parent
MODULE_ROOT = DOCS_ROOT / "modules" / "ROOT"
PAGES_ROOT = MODULE_ROOT / "pages"
EXAMPLES_ROOT = MODULE_ROOT / "examples"
NAV_FILE = MODULE_ROOT / "nav.adoc"


def add_error(errors: list[str], path: Path, message: str) -> None:
    errors.append(f"{path.relative_to(REPOSITORY_ROOT)}: {message}")


def resolve_xref(target: str) -> Path:
    target = target.split("#", 1)[0]
    return (PAGES_ROOT / target).resolve()


def resolve_example(target: str) -> Path:
    family, separator, relative = target.partition("/")
    if separator and family in {"sources", "tests", "benchmarks"}:
        return REPOSITORY_ROOT / family / relative
    return EXAMPLES_ROOT / target


def validate_pages(errors: list[str]) -> None:
    pages = sorted(PAGES_ROOT.rglob("*.adoc"))
    page_set = {page.resolve() for page in pages}

    nav_text = NAV_FILE.read_text(encoding="utf-8")
    nav_targets = {
        (PAGES_ROOT / target.split("#", 1)[0]).resolve()
        for target in re.findall(r"xref:([^\[]+)\[", nav_text)
    }

    for target in sorted(nav_targets):
        if target not in page_set:
            add_error(errors, NAV_FILE, f"navigation target does not exist: {target}")

    for page in sorted(page_set - nav_targets):
        add_error(errors, NAV_FILE, f"page is missing from navigation: {page}")

    source_block_pattern = re.compile(
        r"\[source,[^\]]+\]\n----\n(.*?)\n----", re.DOTALL
    )
    include_pattern = re.compile(r"include::example\$([^\[]+)\[([^\]]*)\]")

    for page in pages:
        text = page.read_text(encoding="utf-8")
        if not text.startswith("= "):
            add_error(errors, page, "page does not start with a level-one title")
        if "```" in text:
            add_error(errors, page, "Markdown code fence found")

        for target in re.findall(r"xref:([^\[\s`]+)\[", text):
            resolved = resolve_xref(target)
            if resolved not in page_set:
                add_error(errors, page, f"xref target does not exist: {target}")

        for block in source_block_pattern.findall(text):
            includes = include_pattern.findall(block)
            if not includes:
                add_error(errors, page, "source block does not use an example include")
                continue

            for target, attributes in includes:
                source = resolve_example(target)
                if not source.is_file():
                    add_error(errors, page, f"example target does not exist: {target}")
                    continue

                tag_match = re.search(r"(?:^|,)tag=([^,]+)", attributes)
                if not tag_match:
                    add_error(errors, page, f"example include has no tag: {target}")
                    continue

                tag = tag_match.group(1)
                source_text = source.read_text(encoding="utf-8")
                start_marker = f"tag::{tag}[]"
                end_marker = f"end::{tag}[]"
                if source_text.count(start_marker) != 1 or source_text.count(end_marker) != 1:
                    add_error(errors, page, f"tag pair is missing or duplicated: {target}#{tag}")
                elif source_text.index(start_marker) > source_text.index(end_marker):
                    add_error(errors, page, f"tag markers are out of order: {target}#{tag}")

    print(f"Checked {len(pages)} AsciiDoc pages and {len(nav_targets)} navigation entries.")


def validate_markdown_count(errors: list[str]) -> None:
    markdown_files: list[Path] = []
    skipped_directories = {".git", "build", "dependencies", "html", "node_modules"}

    for directory, names, files in os.walk(REPOSITORY_ROOT):
        names[:] = [name for name in names if name not in skipped_directories]
        markdown_files.extend(Path(directory) / name for name in files if name.endswith(".md"))

    expected = {REPOSITORY_ROOT / "README.md"}
    if set(markdown_files) != expected:
        rendered = ", ".join(str(path.relative_to(REPOSITORY_ROOT)) for path in markdown_files)
        add_error(errors, REPOSITORY_ROOT / "README.md", f"unexpected Markdown inventory: {rendered}")


def main() -> int:
    errors: list[str] = []
    validate_pages(errors)
    validate_markdown_count(errors)

    if errors:
        print("Documentation validation failed:", file=sys.stderr)
        for error in errors:
            print(f"- {error}", file=sys.stderr)
        return 1

    print("Documentation validation passed; README.md is the sole Markdown file.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
