OUTDIR = "./build"
EXEPATH := $(OUTDIR)/https_service/bin/https_service

all:
	make all -C ./src
.PHONY: all

clean:
	make clean -C ./src
.PHONY: clean

run:
	make run -C ./src
.PHONY: run

deps:
	./scripts/setup.deps.sh
.PHONY: deps
