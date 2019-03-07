# genctemplate

Template generator for you to quickly further modify and work on top.
Support C, and Makefile for now.

# How to Build & Install

`make` then `make install` (it will install at `/usr/local/bin`)

then

copy `templates/` directory to your home's directory at `~/.genctemplate/`. Create such directory if need. Program will read template files from such place. You can as well modify the template file and it will take effect next time you execute the command.

# Usage

`genctemplate [-t <c,makefile>] /your/path/filename`

in which

`-t` can be `c`, `makefile` (case sensitive for now).

i.e. 

* `genctemplate myproject.c`
* `genctemplate /my/path/main.c`
* `genctemplate -t c /my/path/main.c`
* `genctemplate -t makefile Makefile`

# License

Wasin Thonkaew, MIT
