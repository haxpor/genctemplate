CC := gcc
OUT := genctemplate
CFLAGS += -std=c99 -Wall -Isrc/
LFLAGS :=
TARGETS := \
	src/main.c

.PHONY: all

all: $(TARGETS) genctemplate

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

genctemplate: src/main.o
	$(CC) $^ -o $(OUT) $(LFLAGS)

install: all
	cp -p $(OUT) /usr/local/bin/$(OUT)

install-templates:
	@mkdir -p ~/.genctemplate
	cp -Rp templates ~/.genctemplate/

clean:
	rm -f src/*.o
	rm -f genctemplate
