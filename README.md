# genctemplate

Template generator for you to quickly further modify and work on top.
Support C, and Makefile for now.

# How to Build & Install

`make` then `make install` (it will install at `/usr/local/bin`)

Behind the scene, it will create a new directory at `~/.genctemplate` then copy templates directory to such location.

# Usage

* `genctemplate -t <template-string> /your/path/filename` - to generate template at specified destination path
* `genctemplate templates` - to list all supported template

i.e. 

* `genctemplate myproject.c`
* `genctemplate /my/path/main.c`
* `genctemplate -t c /my/path/main.c`
* `genctemplate -t makefile Makefile`

# License

Wasin Thonkaew, MIT
