# Changelog

Notable changes to occutils, in the format of [Keep a Changelog](https://keepachangelog.com/en/1.1.0/); versions follow [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

Changes wait in `changes/`, a file each (`python scripts/release.py change fixed "..."`), until the release PR writes them here (`python scripts/release.py release`). Each release's section becomes its GitHub release notes.

## [0.1.0] - 2026-11-17

### Added

- `xde::Doc::SaveSTEP` takes an optional header customizer, `std::function<void(APIHeaderSection_MakeHeader&)>`, to set the STEP header (author, organization, ...).

### Changed

- Builds on OCCT 8.0 (vcpkg `opencascade` 8.0.0): the API takes and returns OCCT 8's types, e.g. `NCollection_List<TopoDS_Shape>` for `TopTools_ListOfShape` and `occ::handle<T>` for `Handle(T)`.

### Fixed

- `xde::Doc::SaveSTEP` writes its header: the customizer's values, or else occutils' defaults. The file carried OCCT's defaults (`'Author'`, `'Open CASCADE'`) instead.

[0.1.0]: https://github.com/paulbuechner/occutils/releases/tag/v0.1.0
