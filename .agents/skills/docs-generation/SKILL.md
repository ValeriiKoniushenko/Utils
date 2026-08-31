---
name: utils-docs
description: Use this skill whenever writing, updating, reviewing, or generating documentation for Utils. Covers writing/updating AsciiDoc pages for the Antora documentation site under docs/, autonomously synthesizing API reference content by reading source code and Doxygen-style comments directly (not a mechanical Doxygen dump), pulling every code sample in via tagged AsciiDoc includes instead of copy-pasting so examples stay in sync with the code, and reconciling docs with the current codebase, unit tests, benchmarks, README.md, AGENTS.md, and other project SKILL files when they're outdated or incomplete. Trigger this whenever the user asks to write or refresh Utils docs, update the Utils' README, generate or update an API reference for Utils, add or edit an Antora page under docs/, or prepare content for the Utils docs site — even if they don't say "documentation" explicitly (e.g. "explain the codegen API in the docs", "the README is stale, fix it").
---

# Utils Documentation Skill

Writes and maintains the AsciiDoc/Antora documentation for Utils repository (this one), and
generates API reference content by reading the codebase directly rather than
mechanically dumping Doxygen output.

## Hard rules

- **Never read, open, or edit anything under `docs/html`.** That's generated
  build output, not a source of truth, and not something to reason about or
  quote from. If you're tempted to peek in there for "what the docs currently
  say," go read the `.adoc` sources instead.
- AsciiDoc sources live in the standard Antora layout:
  `docs/modules/ROOT/pages/`. New pages go there; update
  `docs/modules/ROOT/nav.adoc` (or wherever the module's nav file lives) any
  time a page is added, removed, or renamed, or it won't show up in the site.
- Building the site (`npx antora --fetch antora-playbook.yml`) is a separate,
  user-run step. Don't run it as part of a doc-writing task — mention it if
  relevant, but writing content and building the site are decoupled.
- **VITAL — never copy-paste code into `.adoc` pages.** Every code sample
  (API usage, examples pulled from tests, snippets from source) must be
  pulled in live via AsciiDoc's `include::` directive with a `tag`/`tags`
  attribute, referencing a tagged region in the actual source/test file —
  not typed or pasted into the page by hand. This is what keeps examples
  from silently rotting out of sync with the code. Concretely:

  - In the source/test file, wrap the relevant lines in tag comments:
    ```cpp
    // tag::basic_usage[]
    auto result = StringAtom::Intern("Hello");
    // end::basic_usage[]
    ```
    If a suitable tag doesn't already exist around the code you want to
    show, add one — don't fall back to copy-pasting because a tag is
    missing.
  - In the `.adoc` page, pull it in by reference:
    ```
    [source,cpp]
    ----
    include::path/to/file.cpp[tag=basic_usage]
    ----
    ```
  - If a tagged file lives outside the `docs/` tree (e.g. in `src/` or
    `tests/`), check whether `antora.yml`/`antora-playbook.yml` already
    exposes it for `include::` resolution (commonly via an `examples`
    resource family or similar mapping). If it doesn't, flag this to the
    user rather than silently pasting the code instead — it likely needs a
    small Antora config addition to resolve paths outside the docs
    component.
  - This applies everywhere a code example appears: narrative pages, API
    reference entries, everything. A hand-copied snippet is only acceptable
    as a last resort when the content genuinely isn't real code (e.g. an
    illustrative pseudocode fragment explicitly labeled as such).

## Source-of-truth priority

The README is explicitly known to be a little outdated. When sources disagree,
trust in this order:

1. **README.md** - a little bit outdated, but you will find the main concepts of all things there. Be aware, and don't try to read it as one file, it's very big. Try to grep to find some specific information.
2. **Unit tests** - it was mostly written manually, so it can be the source of truth for some details.
3. **Current source code**
4. **Different agent skills**

When you find drift between README/docs and the actual code, fix it rather
than silently perpetuating it — and it's worth a short note to the user about
what changed and why.

## Workflow: narrative docs (guides, overviews, README-derived pages)

1. Read what already exists: `docs/modules/ROOT/pages/` and the nav file, so
   you know the current structure and don't duplicate a page.
2. Read `README.md`, `AGENTS.md`, and any other project SKILL files for
   stated intent, terminology, and conventions to stay consistent with.
3. Partially read relevant unit tests and benchmarks to verify claims and
   locate concrete, real examples (usage snippets, expected output) instead
   of inventing illustrative code. Pull any such example in via a tagged
   `include::` (see Hard rules) rather than copy-pasting it.
4. Cross-check against the source for anything specific enough to be wrong.
5. Write/update the `.adoc` page(s); update the nav file if the page set
   changed.

## Workflow: API reference generation (autonomous, not a Doxygen dump)

The goal is synthesized, human-quality API reference documentation — reading
and understanding the code, not converting Doxygen comments 1:1 into AsciiDoc.

1. Locate the public API surface — the headers that make up Utils' public
   interface (start from what's included in the "main" umbrella header(s) or
   whatever the AGENTS.md / build files indicate is public).
2. For each public type/function/macro, read **both** the Doxygen-style
   comment (`/** ... */`, `@brief`, `@param`, etc.) **and** the actual
   signature/implementation. Comments can be stale, missing, or aspirational
   — don't trust them blindly.
3. Where a unit test exercises the symbol, skim it. Tests are often the best
   source of a canonical, correct usage example and reveal behavior the doc
   comment doesn't mention (what happens on invalid input, what's actually
   returned, etc.).
4. Write the reference natively in AsciiDoc, in your own words, structured
   per entry — something like: signature block, one-paragraph description,
   parameters/returns, notable behavior or gotchas, and a short usage example
   when a real test demonstrates one. The prose (description, param/return
   notes) is written by you; any actual code — the signature, the usage
   example — is pulled in via a tagged `include::`, not copy-pasted (see
   Hard rules).
5. Running `doxygen` to produce XML, and feeding it through a converter (e.g.
   AsciiDoxy or doxml2asciidoc), is optional supplementary tooling — useful
   if you want a full symbol inventory to cross-check completeness against.
   It's not a shortcut that replaces reading the code, and Utils has no
   existing Doxygen→AsciiDoc integration set up, so don't assume one.

## AsciiDoc conventions

- Standard Antora page anatomy: `= Page Title` at the top, `==`/`===` for
  sections.
- Every real code snippet is a tagged `include::`, never a pasted
  `[source,cpp]` block with hand-typed content (see Hard rules).
- Cross-reference other pages with `xref:other-page.adoc[]` rather than raw
  links.

## Before finishing

- Confirm nothing under `docs/html` was read or touched.
- Confirm the nav file reflects any added/removed/renamed pages.
- Confirm anything specific (API shape, behavior, numbers) was checked
  against code/tests, not just carried over from the old README.
- Confirm every code sample in the page(s) you touched is a tagged
  `include::`, not pasted code — including ones you didn't add yourself but
  happened to edit around.
- Flag to the user any notable drift you found and fixed between the docs
  and the actual code.