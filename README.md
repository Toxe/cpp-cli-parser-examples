# C++ CLI Arguments Parser Examples

A quick repository to check out and compare a couple of C++ CLI arguments parser libraries.

## Libraries

- [clipp](https://github.com/muellan/clipp) (seems to be no longer maintained)
- [argparse](https://github.com/p-ranav/argparse)
- [CLI11](https://github.com/CLIUtils/CLI11)

Note: At the moment clipp does not compile with C++20 and MSVC.

### clipp

```
DESCRIPTION
    CLI Parser Example (clipp)

SYNOPSIS
        clipp [-h] [-v|-vv] [-t|-p|-d|-r] [-s <seed>] [-z <zoom>] [-i] <width> <height> [<filename>]
              [([--single] [--multi]) | --all]

OPTIONS
        -h, --help  show help
        -v, --verbose
                    show info log messages (verbose)

        -vv, --debug
                    show debug log messages (very verbose)

        -t, --text|-p, --pretty|-d, --data|-r, --raw
                    output format (default: text)

        -s, --seed <seed>
                    random seed (0 or bigger)

        -z, --zoom <zoom>
                    pixel zoom factor for .raw files (default: 1)

        -i, --info  output additional info
        <width>     maze width
        <height>    maze height
        <filename>  output filename
        --single    run test: single inserts for every row
        --multi     run test: insert multiple rows in one request
        --all       run all tests (default)

EXAMPLE
    $ clipp --text 10 10 out.txt
```

### argparse

```
Usage: argparse [options] width height 

Positional arguments:
width           maze width
height          maze height

Optional arguments:
-h --help       shows help message and exits
-v --version    prints version information and exits
--verbose       show info log messages (verbose) [default: false]
-vv --debug     show debug log messages (very verbose) [default: false]
-t --text       output format: text [default: false]
-p --pretty     output format: pretty [default: false]
-d --data       output format: data [default: false]
-r --raw        output format: raw [default: false]
-i --info       output additional info [default: false]
--single        run test: single inserts for every row [default: false]
--multi         run test: insert multiple rows in one request [default: false]
--all           run all tests (default) [default: false]
-s --seed       random seed (0 or bigger) [default: 0]
-z --zoom       pixel zoom factor for .raw files [default: 1]
-f --filename   output filename [default: ""]
```

### CLI11

```
CLI Parser Example (CLI11)
Usage: ./build/src/cli11 [OPTIONS] width height [filename]

Positionals:
  width INT REQUIRED          maze width
  height INT REQUIRED         maze height
  filename TEXT               output filename

Options:
  -h,--help                   Print this help message and exit
  -s,--seed INT               random seed (0 or bigger)
  -z,--zoom INT=1             pixel zoom factor for .raw files
  -i,--info                   output additional info
  -v                          log level (-v: verbose, -vv: debug messages)
[Option Group: output format (default: text)]
  Options:
    -t,--text Excludes: --pretty --data --raw
                                text: uses ASCII '#' characters for walls
    -p,--pretty Excludes: --text --data --raw
                                pretty: uses UNICODE line drawing characters
    -d,--data Excludes: --text --pretty --raw
                                data: export the internal wall data
    -r,--raw Excludes: --text --pretty --data
                                raw: generate a 1 byte per pixel grayscale raw image
[Option Group: run tests (default: all)]
  Options:
    --single Excludes: --multi --all
                                run test: single inserts for every row
    --multi Excludes: --single --all
                                run test: insert multiple rows in one request
    --all Excludes: --single --multi
                                run all tests
```
