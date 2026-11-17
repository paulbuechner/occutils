#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Paul Büchner
# SPDX-License-Identifier: Apache-2.0

"""Deletes the archives of vcpkg's binary cache that no install uses (standard library only).

    python scripts/prune-vcpkg-archives.py <archives> <vcpkg_installed>...

A new compiler or CMake on the runner image changes every port's ABI, so vcpkg rebuilds them under new names; without
this, the old archives would stay in the cache that .github/workflows/quality.yml saves. The archives are the files
provider's (<abi[:2]>/<abi>.zip), the install roots CMake's (<build>/vcpkg_installed).
"""
from __future__ import annotations

import re
import sys
from pathlib import Path


def installed_abis(install_root: Path) -> set[str]:
    """ABIs of the packages installed under a vcpkg install root, from its status database (status + updates/)."""
    database = install_root / "vcpkg"
    packages: dict[tuple[str, str, str], dict[str, str]] = {}
    for file in [database / "status", *sorted((database / "updates").glob("*"))]:
        if not file.is_file():
            continue
        for paragraph in re.split(r"\n\s*\n", file.read_text(encoding="utf-8")):
            fields = dict(line.split(": ", 1) for line in paragraph.splitlines() if ": " in line and not line[0].isspace())
            if "Package" in fields:
                # a later paragraph for the same package and feature replaces the earlier one
                packages[(fields["Package"], fields.get("Architecture", ""), fields.get("Feature", ""))] = fields
    return {f["Abi"] for f in packages.values() if f.get("Status") == "install ok installed" and "Abi" in f}


def main() -> int:
    if len(sys.argv) < 3:
        print(__doc__, file=sys.stderr)
        return 2
    archives = Path(sys.argv[1])
    keep = set().union(*(installed_abis(Path(root)) for root in sys.argv[2:]))
    # nothing installed (a failed install, a wrong path): keep everything
    if not keep:
        print("no installed packages found, nothing pruned", flush=True)
        return 0
    if not archives.is_dir():
        print(f"no binary cache at {archives}, nothing pruned", flush=True)
        return 0
    pruned = 0
    for archive in sorted(archives.glob("*/*.zip")):
        if archive.stem not in keep:
            print(f"pruned {archive.relative_to(archives).as_posix()}", flush=True)
            archive.unlink()
            pruned += 1
    for folder in archives.iterdir():
        if folder.is_dir() and not any(folder.iterdir()):
            folder.rmdir()
    print(f"{pruned} archive(s) pruned, {len(keep)} in use", flush=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
