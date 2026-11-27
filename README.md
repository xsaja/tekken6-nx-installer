# tekken6-nx installer

The installer page of [tekken6-nx](https://github.com/xsaja/tekken6-nx), the native Nintendo Switch port of Tekken 6
(Xbox 360). You give it your own copy of the game (the disc image, or a folder you extracted it into) and it gives you
`tekken6-nx.zip`, ready to extract to the root of the SD card. Everything runs in your browser: the game files never
leave your computer.

It is [renut_nx_ultimate_installer](https://github.com/ChanseyIsTheBest/renut_nx_ultimate_installer) by
ChanseyIsTheBest (the page of the ports made from the ultimate base; its README is
[docs/renut_nx_ultimate_installer.md](docs/renut_nx_ultimate_installer.md)) with Tekken 6's `game.json` and the changes
below.

## What it does

1. Finds the game in what you chose (a disc image read in place, or a folder with `default.xex`) and checks that the
   copy is whole: every file of the build, with its size.
2. Identifies the game by the SHA-256 of `default.xex` and downloads the build's NRO and release files, checking each.
3. Makes the shaders of the deko3d renderer from your copy (`deko3d_glsl.bin`): the 70 in `default.xex` and 5,838 inside
   the data banks `DATA302`-`DATA304`. The NRO carries the other 964 itself (the vertex shaders of `DATA301`'s first
   entry, which a second coder of the game's own packs). Then it compiles the variants the console drew in its test
   sessions (`deko3d_shaders.bin`, with `pipeline_list.bin`), so the game meets few shaders it has to compile itself.
4. Puts the game's files into the zip as they are (the data banks are already compressed), and streams the zip to disk.

## What is different from the base

| Where | What |
|---|---|
| `game.json` | Tekken 6: the package `tekken6-nx`, every file kept as it is, no DLC, no re-encodes, no VFS index |
| `lib/sources.js` | The `xctd` coding: Tekken's data banks are XCTD files whose 0x20000-byte raw blocks are each an independent run of XMem LZX frames (window 2^17), decoded with the page's LZX module |
| `tools/make_release.py` | Reads tekken6-nx's records (`{ "records": [...] }`, with each XCTD record's block sizes) and `--nro-carries`, the list of shaders the NRO carries itself |
| `wasm/renut_hlsl.*`, `wasm/uam.*` | Rebuilt from tekken6-nx's translator and uam (Emscripten 6.0.11); `wasm/src/translator/container_0e.h` stands in for Kameo's header (Tekken's containers are all the standard form), and the front end sets no user clip plane, as tekken6-nx's translator has none |
| `py/` | tekken6-nx's shader passes (`tools/sync_from_base.py --base <tekken6-nx>`) |
| `index.html` | The forwarder notice: a Sphaira forwarder with a 39-bit address space, started from its icon for the four CPU cores |

## Making a release

In tekken6-nx: `python tools/tekken_shader_records.py` (the records, and `out/release/records_missing.txt`, the shaders
no file holds), then `python tools/renut_release_shaders.py --keep out/release/records_missing.txt` and the release NRO
(`-DBANJO_RENUT_RELEASE_SHADERS=ON`). Here:

```bash
python tools/make_release.py --port <tekken6-nx> --nro <release NRO> --game <game folder> \
  --records <tekken6-nx>/out/release/records.json --nro-carries <tekken6-nx>/out/release/records_missing.txt \
  --pipeline-list <a console's pipeline_list.bin> --file <tekken6-nx>/out/release/LICENSES.tekken6.txt:LICENSES.txt \
  --version 1.0.0
node test/full_node.mjs <game folder> test/out_zip/tekken6-nx.zip --reference
```

`full_node.mjs` runs the page's whole build in Node and writes the shader files' SHA-256 into the manifest, so the page
can say whether a player's are the release's. Its GLSL differs from tekken6-nx's native build in its variable numbering
(the page's DXC is a newer version); the first release was checked by playing it in Ryujinx with the page's files: the
menus, the character select and a fight, no shader missing and no compile failure.

The NRO is in `release/` here (GitHub Pages serves it with the page). Testing locally: `python serve.py 8193 .`, then
http://127.0.0.1:8193/ in Chrome or Edge.

Outputs of the tests (zips, shader files) are game data: never commit or share them.

## License

GPL-3.0 (see [LICENSE](LICENSE)). Third-party components are listed in [NOTICES.md](NOTICES.md).

Tekken is a trademark of Bandai Namco Entertainment. Not affiliated with or endorsed by Bandai Namco, Microsoft or
Nintendo.
