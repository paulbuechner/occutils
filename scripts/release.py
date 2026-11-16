#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Paul Büchner
# SPDX-License-Identifier: Apache-2.0

"""occutils' changelog and releases, like changesets without npm (standard library only).

    python scripts/release.py change fixed "what changed"    # an entry for the next release: changes/<name>.<type>.md
    python scripts/release.py changelog [--version 1.2.0]    # a release's notes; default: what's pending
                                                             # --pull-request main: its release PR's description
    python scripts/release.py changelog --check origin/main  # fail if the library changed since then without an entry
    python scripts/release.py version [--latest]             # the version (CMakeLists.txt, vcpkg.json), or the newest release
    python scripts/release.py release [--version 1.0.0]      # write the next release into CHANGELOG.md, bump the version

Entry types and the part of the version (semver) they bump:
    breaking, removed               major
    added, changed, deprecated      minor
    fixed, security                 patch
"""
from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CHANGELOG = ROOT / "CHANGELOG.md"
# pending changelog entries, one file each: <name>.<type>.md
CHANGES = ROOT / "changes"
# the version lives in both
CMAKELISTS = ROOT / "CMakeLists.txt"
VCPKG_JSON = ROOT / "vcpkg.json"
REPOSITORY = "https://github.com/paulbuechner/occutils"

# entry type -> heading, in the changelog's order (Keep a Changelog's, with Breaking first)
CHANGE_TYPES = {"breaking": "Breaking", "added": "Added", "changed": "Changed", "deprecated": "Deprecated",
                "removed": "Removed", "fixed": "Fixed", "security": "Security"}
# heading -> the version part it bumps: 0 major, 1 minor, 2 patch (other headings: patch)
BUMPS = {"Breaking": 0, "Removed": 0, "Added": 1, "Changed": 1, "Deprecated": 1, "Fixed": 2, "Security": 2}
# what the library ships: a pull request that changes it needs an entry
SHIPPED = re.compile(r"(include|src|cmake)/|CMakeLists\.txt$|vcpkg[^/]*\.json$")

SEMVER = r"\d+\.\d+\.\d+"
VERSION = rf"{SEMVER}(?:-[0-9A-Za-z.-]+)?"
SECTION_END = r"(?=^## \[|^\[[^\]\n]+\]: |\Z)"
CMAKE_VERSION = re.compile(r"^(set\(OCCUTILS_VERSION\s+)([^\s)]+)(\))", re.M)
VCPKG_VERSION = re.compile(r'^(\s*"version"\s*:\s*")([^"]*)(")', re.M)


# --------------------------------------------------------------------------- files
def read_text(path: Path) -> str:
    return path.read_bytes().decode("utf-8").replace("\r\n", "\n") if path.exists() else ""


def write_text(path: Path, text: str) -> None:
    """Writes the text with the file's line endings: CRLF if it has them (a Windows checkout), else LF."""
    crlf = path.exists() and b"\r\n" in path.read_bytes()
    path.write_bytes((text.replace("\n", "\r\n") if crlf else text).encode("utf-8"))


# --------------------------------------------------------------------------- version
def cmake_version() -> str:
    match = CMAKE_VERSION.search(read_text(CMAKELISTS))
    if not match:
        sys.exit("CMakeLists.txt has no set(OCCUTILS_VERSION <version>)")
    return match.group(2)


def current_version() -> str:
    """The version CMakeLists.txt and vcpkg.json carry; they must agree."""
    cmake = cmake_version()
    vcpkg = json.loads(read_text(VCPKG_JSON)).get("version")
    if cmake != vcpkg:
        sys.exit(f"CMakeLists.txt has version {cmake}, vcpkg.json {vcpkg}")
    if not re.fullmatch(SEMVER, cmake):
        sys.exit(f"version {cmake}: not MAJOR.MINOR.PATCH")
    return cmake


def bumped(version: str, part: int) -> str:
    """The version with one part raised and the ones after it reset: 1.2.3 -> 2.0.0, 1.3.0, 1.2.4."""
    numbers = [int(n) for n in version.split(".")]
    numbers[part] += 1
    numbers[part + 1:] = [0] * (2 - part)
    return ".".join(map(str, numbers))


def write_version(version: str) -> None:
    """The version into CMakeLists.txt and vcpkg.json, each edited in place."""
    for path, pattern in ((CMAKELISTS, CMAKE_VERSION), (VCPKG_JSON, VCPKG_VERSION)):
        text, count = pattern.subn(rf"\g<1>{version}\g<3>", read_text(path), count=1)
        if count != 1:
            sys.exit(f"{path.name}: no version to replace")
        write_text(path, text)
    if json.loads(read_text(VCPKG_JSON)).get("version") != version:
        sys.exit("vcpkg.json: the first \"version\" isn't the top-level one")


# --------------------------------------------------------------------------- changelog
def read_changelog() -> str:
    return read_text(CHANGELOG)


def released_versions(changelog: str) -> list[str]:
    """The versions the changelog has sections for, newest first."""
    return re.findall(rf"^## \[({VERSION})\]", changelog, re.M)


def latest_release(changelog: str) -> str | None:
    """The newest release in the changelog, which must be the version CMakeLists.txt and vcpkg.json carry."""
    released = released_versions(changelog)
    if not released:
        return None
    current = current_version()
    if released[0] != current:
        sys.exit(f"CHANGELOG.md's newest release is {released[0]}, CMakeLists.txt and vcpkg.json have {current}")
    return released[0]


def changelog_section(changelog: str, version: str | None) -> str | None:
    """The body of a version's section (None: [Unreleased]); None if the changelog has no such section."""
    heading = re.escape(version) if version else "Unreleased"
    match = re.search(rf"^## \[{heading}\][^\n]*\n(.*?){SECTION_END}", changelog, re.S | re.M)
    return match.group(1).strip() if match else None


def fragments() -> list[tuple[str, Path]]:
    """The pending entries: (type, file), in the order of the file names."""
    if not CHANGES.is_dir():
        return []
    found = []
    for path in sorted(CHANGES.glob("*.md")):
        kind = path.name[:-3].rsplit(".", 1)[-1]
        if kind not in CHANGE_TYPES:
            sys.exit(f"changes/{path.name}: name it <name>.<type>.md, type one of {', '.join(CHANGE_TYPES)}")
        if not read_text(path).strip():
            sys.exit(f"changes/{path.name} is empty")
        found.append((kind, path))
    return found


def as_bullet(text: str) -> list[str]:
    """An entry as one list item: its first line after "- ", the rest indented under it."""
    lines = text.strip().splitlines()
    first = lines[0] if lines[0].startswith("- ") else f"- {lines[0]}"
    return [first] + [f"  {line}" if line.strip() and not line.startswith("  ") else line for line in lines[1:]]


def parse_notes(body: str | None) -> dict[str, list[str]]:
    """A section's body by heading (### Added): each heading's lines."""
    notes: dict[str, list[str]] = {}
    heading = None
    for line in (body or "").splitlines():
        if line.startswith("### "):
            heading = line[4:].strip()
            notes.setdefault(heading, [])
        elif heading is not None:
            notes[heading].append(line)
    return notes


def merge_notes(into: dict[str, list[str]], notes: dict[str, list[str]]) -> None:
    """Adds each heading's lines after the ones under the same heading, without the blank lines between."""
    for heading, lines in notes.items():
        entries = into.setdefault(heading, [])
        while entries and not entries[-1].strip():
            entries.pop()
        start = next((i for i, line in enumerate(lines) if line.strip()), len(lines))
        entries += lines[start:]


def render_notes(notes: dict[str, list[str]]) -> str | None:
    """Notes as a section's body: the known headings in their order, others after; None without any."""
    if not notes:
        return None
    order = list(CHANGE_TYPES.values()) + [h for h in notes if h not in CHANGE_TYPES.values()]
    return "\n\n".join(f"### {heading}\n\n" + "\n".join(notes[heading]).strip("\n") for heading in order if heading in notes)


def pending_notes(changelog: str) -> str | None:
    """What the next release says: the [Unreleased] section, if the changelog has one, with the entries added under
    their headings; None if nothing is pending."""
    notes = parse_notes(changelog_section(changelog, None))
    for kind, path in fragments():
        merge_notes(notes, {CHANGE_TYPES[kind]: as_bullet(read_text(path))})
    return render_notes(notes)


def with_release(changelog: str, version: str, notes: str, date: str) -> str:
    """The changelog with a release written in: its section where [Unreleased] was, else before the first release, and its
    link first among the links; [Unreleased]'s link goes with its section."""
    section = f"## [{version}] - {date}\n\n{notes}\n\n"
    place = (re.search(rf"^## \[Unreleased\][^\n]*\n.*?{SECTION_END}", changelog, re.S | re.M)
             or re.search(r"^## \[|^\[[^\]\n]+\]: |\Z", changelog, re.M))
    head = changelog[:place.start()].rstrip("\n")
    rest = changelog[place.end():] if place.group(0).startswith("## [Unreleased]") else changelog[place.start():]
    text = (f"{head}\n\n" if head else "") + section + rest
    text = re.sub(r"^\[Unreleased\]: [^\n]*\n?", "", text, flags=re.M).rstrip("\n") + "\n"
    link = f"[{version}]: {REPOSITORY}/releases/tag/v{version}\n"
    links = re.search(r"^\[[^\]\n]+\]: ", text, re.M)
    return text[:links.start()] + link + text[links.start():] if links else f"{text}\n{link}"


def release_pr_body(version: str, notes: str, branch: str) -> str:
    """The release pull request's description: changesets' "Version Packages" text with occutils' facts, then the release."""
    return "\n\n".join([
        f"This PR was opened by the [occutils release]({REPOSITORY}/blob/{branch}/.github/workflows/release.yml) GitHub "
        "action. When you're ready to do a release, you can merge this and the release will be published on GitHub "
        f"automatically (tag `v{version}`). If you're not ready to do a release yet, that's fine, whenever you add more "
        f"changelog entries to {branch}, this PR will be updated.",
        "# Releases", f"## occutils@{version}", notes])


def write_out(text: str, output: str | None) -> None:
    if output:
        Path(output).write_bytes((text + "\n").encode("utf-8"))
    else:
        sys.stdout.buffer.write((text + "\n").encode("utf-8"))  # UTF-8 on any console code page


# --------------------------------------------------------------------------- commands
def cmd_change(args: argparse.Namespace) -> None:
    """A changelog entry for the next release, like a changeset: changes/<name>.<type>.md."""
    text = " ".join(args.text).strip()
    if not text:
        sys.exit("say what changed")
    name = args.name or "-".join(re.findall(r"[a-z0-9]+", text.lower())[:6]) or f"{datetime.now(timezone.utc):%Y%m%d%H%M%S}"
    CHANGES.mkdir(exist_ok=True)
    path = CHANGES / f"{name}.{args.type}.md"
    if path.exists():
        sys.exit(f"changes/{path.name} exists: pick another --name")
    path.write_bytes((text + "\n").encode("utf-8"))
    print(f"changes/{path.name}")


def check_changes(base: str) -> None:
    """A pull request that changes what the library ships says so: an entry in changes/ (or an edit of CHANGELOG.md)."""
    changed = subprocess.run(["git", "diff", "--name-only", f"{base}...HEAD"], cwd=ROOT, check=True,
                             capture_output=True, text=True).stdout.split()
    shipped = [p for p in changed if SHIPPED.match(p)]
    noted = [p for p in changed if p.startswith("changes/") and p.endswith(".md") or p == "CHANGELOG.md"]
    if shipped and not noted:
        sys.exit("this changes the library but adds no changelog entry: python scripts/release.py change <type> "
                 "\"what changed\" (or the label 'no changelog' for changes users don't see)")
    fragments()  # names and contents
    print(f"changelog: {len(noted)} entr{'y' if len(noted) == 1 else 'ies'} for {len(shipped)} shipped file(s)")


def cmd_changelog(args: argparse.Namespace) -> None:
    if args.check:
        check_changes(args.check)
        return
    if args.pull_request and not args.version:
        sys.exit("--pull-request needs --version: the release it describes")
    changelog = read_changelog()
    section = changelog_section(changelog, args.version) if args.version else pending_notes(changelog)
    if section is None:
        sys.exit(f"CHANGELOG.md has no section [{args.version}]" if args.version else "nothing pending: no changes/ and no [Unreleased]")
    write_out(release_pr_body(args.version, section, args.pull_request) if args.pull_request else section, args.output)


def cmd_version(args: argparse.Namespace) -> None:
    """The version CMakeLists.txt and vcpkg.json carry, or the newest release in CHANGELOG.md (empty if none)."""
    print((latest_release(read_changelog()) or "") if args.latest else current_version())


def cmd_release(args: argparse.Namespace) -> None:
    """Writes the next release into CHANGELOG.md (the pending notes under ## [version] - date) and its version into
    CMakeLists.txt and vcpkg.json, and deletes the entries. Prints the version (nothing with --if-pending when nothing is
    pending)."""
    changelog = read_changelog()
    notes = pending_notes(changelog)
    if notes is None:
        if args.if_pending:
            return
        sys.exit("nothing to release: no changes/ and no [Unreleased]")
    current = current_version()
    latest_release(changelog)  # CHANGELOG.md agrees with the version
    version = args.version or bumped(current, min(BUMPS.get(heading, 2) for heading in parse_notes(notes)))
    if not re.fullmatch(SEMVER, version):
        sys.exit(f"version {version}: not MAJOR.MINOR.PATCH")
    if tuple(map(int, version.split("."))) <= tuple(map(int, current.split("."))):
        sys.exit(f"version {version}: not after {current}")
    write_text(CHANGELOG, with_release(changelog, version, notes, f"{datetime.now(timezone.utc):%Y-%m-%d}"))
    write_version(version)
    for _, path in fragments():
        path.unlink()
    print(version)


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    ch = sub.add_parser("change", help="a changelog entry for the next release: changes/<name>.<type>.md")
    ch.add_argument("type", choices=list(CHANGE_TYPES))
    ch.add_argument("text", nargs="+", help="what changed, for the library's users")
    ch.add_argument("--name", help="the file's name; default: from the text")
    ch.set_defaults(fn=cmd_change)
    c = sub.add_parser("changelog", help="a release's notes; default: what's pending (changes/, [Unreleased])")
    c.add_argument("--version", help="the section of this release")
    c.add_argument("--output", help="write to this file instead of stdout")
    c.add_argument("--pull-request", metavar="BRANCH", help="with --version: the description of its release PR on BRANCH")
    c.add_argument("--check", metavar="BASE", help="fail if the commits since BASE change the library without an entry")
    c.set_defaults(fn=cmd_changelog)
    v = sub.add_parser("version", help="the version CMakeLists.txt and vcpkg.json carry")
    v.add_argument("--latest", action="store_true", help="the newest release in CHANGELOG.md instead, empty if none")
    v.set_defaults(fn=cmd_version)
    r = sub.add_parser("release", help="write the next release into CHANGELOG.md and bump the version")
    r.add_argument("--version", help="this version instead of the one the entries' types give")
    r.add_argument("--if-pending", action="store_true", help="do nothing, successfully, when nothing is pending")
    r.set_defaults(fn=cmd_release)
    return p


def main() -> None:
    args = parser().parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
