# ChromaShell

Display lines of color in a true color terminal.

## Requirements

- POSIX-compliant operating system
- Terminal with true color support

## Build requirements

- `cmake` >= 3.1
- `gcc`
- `gettext`
- `cjson`

## Building

`$ (mkdir -p build && cd build && cmake .. && make)`

## Installation

`$ (cd build && make install)`

## Uninstallation

`$ (cd build && make uninstall)`

## Usage

`$ chromashell -h`

### Example: Display a transgender flag

`$ chromashell -p trans`

Or:

`$ chromashell -s 7ACBF5,2 -s EAACB8,2 -s FFFFFF,2 -s EAACB8,2 -s 7ACBF5,2`

## Documentation

See [`doc/`](doc).

## License

Released under the GNU GPLv3.

See `LICENSE` for more information.

## Acknowledgements

Inspired by [cli-pride-flags](https://github.com/ExperiBass/cli-pride-flags) <3