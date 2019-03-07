# genctemplate
Template C code generator to quickly have code to test something

# How to Build & Install

`make` then `make install` (it will install at `/usr/local/bin`)

then

copy `templates/` directory to your home's directory at `~/.genctemplate/`. Create such directory if need. Program will read template files from such place. You can as well modify the template file and it will take effect next time you execute the command.

# Usage

`genctemplate [-t <c,makefile>] /your/path/filename`

in which

`-t` can be `c`, `makefile` (case sensitive for now).

# License

Wasin Thonkaew, MIT
