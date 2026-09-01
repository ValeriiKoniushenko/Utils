---
name: docs-generation
description: Use this skill whenever writing, updating, reviewing, or generating repository documentation, especially AsciiDoc pages for an Antora site or human-written API reference material. Synthesize reference content from the current source, tests, and project guidance rather than mechanically converting comments; keep real code examples synchronized through tagged includes. Apply it when a user asks to update a README, guide, API reference, documentation page, navigation entry, or documentation site.
---

# Documentation Generation Skill

Writes and maintains repository documentation, with a focus on AsciiDoc/Antora sites, and
generates API reference content by reading the codebase directly rather than mechanically
dumping documentation comments.

## Repository layout

Use this standard layout when working in repositories that invoke this skill:

- `sources/` — implementation and public API source files.
- `tests/` — unit tests and canonical usage examples.
- `benchmarks/` — performance measurements and, where relevant, usage examples.
- `docs/` — documentation sources, Antora configuration, and generated output.
- `.agents/` — repository-specific contributor instructions and agent skills.

Treat these directories as the default locations, while checking the repository
configuration when a particular file or tool uses a more specific subdirectory.

## Hard rules

- **Never read, open, or edit generated documentation output under `docs/`.**
  For example, `docs/html/` may contain generated site output. It is not a
  source of truth; use the source documentation instead.
- Documentation source files live under `docs/`. For an Antora site, pages
  normally live under `docs/modules/ROOT/pages/` and navigation under
  `docs/modules/ROOT/nav.adoc`. Update navigation whenever a page is added,
  removed, or renamed.
- **At the end of every documentation-generation task, build the site with
  this exact command:**
  ```sh
  cd docs
  npx antora --fetch antora-playbook.yml
  ```
  Run it after all documentation edits and navigation changes. If it fails,
  report the failure and its relevant output to the user; do not silently
  treat the documentation work as complete.
- **Never copy-paste real code into documentation pages.** Every code sample
  (API usage, examples pulled from tests, snippets from source) must be
  pulled in live via AsciiDoc's `include::` directive with a `tag`/`tags`
  attribute, referencing a tagged region in the actual source/test file —
  not typed or pasted into the page by hand. This keeps examples from
  silently falling out of sync with the code. Concretely:

  - In the source/test file, wrap the relevant lines in tag comments:
    ```cpp
    // tag::basic_usage[]
    auto result = library::example("Hello");
    // end::basic_usage[]
    ```
    If a suitable tag does not already exist around the code you want to
    show, add one; do not fall back to copy-pasting because a tag is missing.
  - In the `.adoc` page, pull it in by reference:
    ```
    [source,cpp]
    ----
    include::path/to/file.cpp[tag=basic_usage]
    ----
    ```
  - If a tagged file lives outside the documentation tree, normally under
    `sources/` or `tests/`, check whether the Antora component/playbook already
    exposes it for `include::` resolution (commonly via an `examples` resource
    family or similar mapping). If it does not, flag this to the user rather
    than silently pasting the code; the documentation configuration may need a
    small, explicit addition.
  - This applies everywhere a code example appears: narrative pages, API
    reference entries, and other documentation. A hand-copied snippet is only
    acceptable when the content genuinely is not real code, such as an
    illustrative pseudocode fragment explicitly labeled as such.

## Source-of-truth priority

When sources disagree, use the most authoritative and current source available.
As a default, trust sources in this order:

1. Current source code and public interfaces
2. Executable tests and verified examples
3. Benchmarks when documenting measured behavior or performance
4. Maintained documentation and project guidance, including `.agents/`
5. Generated documentation or derived artifacts

Project-specific instructions may change this order. If documentation has
drifted from the implementation, correct it rather than silently preserving
the drift, and briefly tell the user what changed and why.

## Workflow: narrative docs (guides, overviews, README-derived pages)

1. Read what already exists under `docs/`, including the documentation pages
   and navigation files, so you know the current structure and do not
   duplicate a page.
2. Read the README, contributor instructions, and relevant guidance under
   `.agents/` for stated intent, terminology, and conventions.
3. Read relevant files under `tests/`, examples under `sources/`, and
   `benchmarks/` when performance or measured behavior is involved, to verify claims and
   locate concrete, real examples (usage snippets and expected output) instead
   of inventing illustrative code. Pull any such example in via a tagged
   `include::` rather than copy-pasting it.
4. Cross-check the source for anything specific enough to be wrong.
5. Write or update the documentation pages and update navigation if the page
   set changed.

## Workflow: API reference generation (autonomous, not a comment dump)

The goal is synthesized, human-quality API reference documentation — reading
and understanding the code, not converting documentation comments one-to-one.

1. Locate the public API surface under `sources/` — start from package exports,
   public headers, umbrella interfaces, build files, or contributor guidance
   that identify what users are expected to consume.
2. For each public type, function, or macro, read both the documentation
   comment (`/** ... */`, `@brief`, `@param`, and similar) and the actual
   signature/implementation. Comments can be stale, missing, or aspirational;
   do not trust them blindly.
3. Where a unit test exercises the symbol, skim it. Tests often provide the
   most accurate usage example and reveal behavior that the comment does not
   mention, such as invalid-input handling or actual return values.
4. Write the reference natively in the repository's documentation format and
   in your own words. Structure each entry as appropriate, for example with a
   signature, description, parameters/returns, notable behavior or gotchas,
   and a short usage example when a real test demonstrates one. Write the
   prose yourself; pull actual code through tagged includes.
5. Running a documentation generator to produce an inventory or intermediate
   representation is optional supplementary tooling. It can help cross-check
   completeness, but it is not a shortcut that replaces reading the code, and
   you should not assume an existing comment-to-documentation integration.

## AsciiDoc conventions

- For Antora pages, follow the project's page anatomy and component
  conventions. The usual AsciiDoc structure starts with `= Page Title` and
  uses `==`/`===` for sections.
- Every real code snippet is a tagged `include::`, never a pasted
  `[source,cpp]` block with hand-typed content.
- Cross-reference other Antora pages with `xref:` when supported rather than
  hard-coding site URLs.

## Before finishing

- Confirm no generated documentation output was read or touched.
- Confirm navigation reflects any added, removed, or renamed pages when the
  site uses explicit navigation.
- Confirm anything specific (API shape, behavior, or numbers) was checked
  against the implementation or executable tests.
- Confirm every code sample in the pages you touched is a tagged `include::`,
  including samples that were already present in a page you edited.
- Flag to the user any notable drift found and fixed between the documentation
  and the actual implementation.
