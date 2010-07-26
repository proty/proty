# A Makefile that wraps the scons script
# May be helpful for some IDEs

all: debug

release:
	@scons -Q mode=release

debug:
	@scons -Q mode=debug

clean:
	@scons -Q -c

install:
	@scons -Q mode=release install

uninstall:
	@scons -Q -c mode=release install

test:
	@bash test/test

pull:
	git pull origin master

push:
	git push origin master

doc:
	cd doc && texi2html pyrite.texi
	cd doc && texi2pdf pyrite.texi

.PHONY: all release debug clean install uninstall test pull push doc
