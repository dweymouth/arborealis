arborealis
==========
An [Arborealis](http://esolangs.org/wiki/Arborealis) interpreter written in C.
Currently, one Arborealis program (cat) is included with the source.

## Building
On GNU/Linux, run `make` - simple as that! Should compile on Windows, but you might have to remove the dependency on sysexits.h.

## Usage
`arborealis <source file>`

## Implementation Details
* Execution takes place in two stages: parse and run. Currently, the parse stage
  discards non-instruction characters and pre-compiles an end-of-loop jump table.
* Number of nested loops and size of the tree data structure are unbounded.
* The Arborealis specification does not define a value for EOF. This implementation
  assigns EOF the value 0. (Note: this makes EOF indistinguishable from ASCII NUL.)

## Todo
* Parse-time optimizations (e.g. reducing a run of '+' or '-' instructions)
* Write some more arb programs that aren't direct translations from brainfuck
  and are idiomatic to the language. (Please feel free to submit your own!)

## Contributing
Arborealis is released under the MIT license. Feel free to fork and pull!
