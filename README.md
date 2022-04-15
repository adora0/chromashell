# ChromaShell

Display lines of color in a true color terminal.

## Requirements

- POSIX-compliant operating system
- Terminal with true color support

## Build Requireements

- `gcc`
- `cmake` >= 3.1
- `cjson`

## Building

`(mkdir -p build && cd build && cmake .. && make)`

## Installation (as root)

`(cd build && make install)`

## Uninstallation

`(cd build && make uninstall)`

## Usage

See `chromashell -h`

### Example: Display a transgender flag

`chromashell -p trans`

Or:

`chromashell -s 7ACBF5,2 -s EAACB8,2 -s FFFFFF,2 -s EAACB8,2 -s 7ACBF5,2`

## License

Released under the GNU GPLv3.

See `LICENSE` for more information.

## Acknowledgements

Inspired by [cli-pride-flags](https://github.com/ExperiBass/cli-pride-flags) <3