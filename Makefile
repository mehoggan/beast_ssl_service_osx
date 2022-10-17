OUTDIR = "./make_build"
EXEPATH := $(OUTDIR)/https_service/bin/https_service

all: deps
	make all -C ./src
.PHONY: all

just_bin:
	make just_bin -C ./src
.PHONY: just_bin

clean:
	make clean -C ./src
.PHONY: clean

run: all
	make run -C ./src
.PHONY: run

deps:
	./scripts/setup.deps.sh
.PHONY: deps
