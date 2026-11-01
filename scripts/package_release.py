"""Package compiled artifacts; never fetch or redistribute Nexus assets."""
import argparse
import pathlib
import shutil
import zipfile


def package(dll, pdb, output):
    root = pathlib.Path(__file__).resolve().parents[1]
    output.mkdir(parents=True, exist_ok=True)
    for source in (dll, pdb):
        if not source.is_file() or source.stat().st_size == 0:
            raise RuntimeError(f'Missing or empty compiled artifact: {source}')
        shutil.copy2(source, output / source.name)
    with zipfile.ZipFile(output / 'PhotoModeF4-1.11.240.zip', 'w', zipfile.ZIP_DEFLATED) as archive:
        archive.write(dll, 'Data/F4SE/Plugins/' + dll.name)
        for name in ('README.md', 'DEPENDENCIES.md', 'TESTING.md', 'LICENSE.txt', 'THIRD_PARTY_NOTICES.md'):
            archive.write(root / name, name)
        for source in sorted((root / 'licenses').rglob('*')):
            if source.is_file():
                archive.write(source, source.relative_to(root).as_posix())
        # Upstream checkout contains no runtime assets. Preserve any reviewed assets
        # later supplied in Data; fonts must remain an independently installed mod.
        for source in sorted((root / 'Data').rglob('*')):
            if source.is_file():
                if 'ImGuiFonts' in source.parts:
                    raise RuntimeError('External ImGui Fonts must not be bundled')
                archive.write(source, source.relative_to(root).as_posix())


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--dll', required=True, type=pathlib.Path)
    parser.add_argument('--pdb', required=True, type=pathlib.Path)
    parser.add_argument('--output', required=True, type=pathlib.Path)
    args = parser.parse_args()
    package(args.dll, args.pdb, args.output)
