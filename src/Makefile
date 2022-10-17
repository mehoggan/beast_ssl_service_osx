TARGET = https_service
ROOT_BUILD_DIR = ../make_build
OUTDIR = $(ROOT_BUILD_DIR)/https_service
OBJECTS = $(OUTDIR)/obj/main.o \
	$(OUTDIR)/obj/https_session.o \
	$(OUTDIR)/obj/service.o \
	$(OUTDIR)/obj/service_helpers.o
CURR_DIR = $(shell pwd)
LDFLAGS = "-Wl,-rpath,$(CURR_DIR)/$(ROOT_BUILD_DIR)/boostbeast/lib"
LDFLAGS += "-Wl,-rpath,$(CURR_DIR)/$(ROOT_BUILD_DIR)/openssl/lib"
CXXFLAGS=-std=c++17 -g -O0 -Wall -Wextra -Werror -pedantic

$(TARGET): $(OBJECTS)
	mkdir -p $(OUTDIR)/bin
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $(OUTDIR)/bin/$(TARGET) \
		$(OBJECTS) \
		-L../make_build/boostbeast/lib \
		-L../make_build/openssl/lib \
		-lboost_filesystem \
		-lboost_system \
		-lboost_thread \
		-lcrypto.3 \
		-lssl.3 \

$(OUTDIR)/obj/main.o: main.cxx \
		$(OUTDIR)/obj/https_session.o \
    	$(OUTDIR)/obj/service.o \
    	$(OUTDIR)/obj/service_helpers.o
	$(CXX) $(CXXFLAGS) \
		-I../make_build/boostbeast/include \
		-I../make_build/openssl/include -c main.cxx -o $@

$(OUTDIR)/obj/https_session.o: https_session.cxx \
	$(OUTDIR)/obj/service.o
	$(CXX) $(CXXFLAGS) -I../make_build/boostbeast/include \
		-I../make_build/openssl/include -c https_session.cxx -o $@

$(OUTDIR)/obj/service.o: service.cxx \
		$(OUTDIR)/obj/service_helpers.o
	$(CXX) $(CXXFLAGS) -I../make_build/boostbeast/include \
		-I../make_build/openssl/include -c service.cxx -o $@

$(OUTDIR)/obj/service_helpers.o: service_helpers.cxx
	mkdir -p $(OUTDIR)/obj
	$(CXX) $(CXXFLAGS) -I../make_build/boostbeast/include \
		-I../make_build/openssl/include -c service_helpers.cxx -o $@

all: $(TARGET)
.PHONY: all

just_bin: $(TARGET)
.PHONY: just_bin

clean:
	rm -rf $(OUTDIR)/*
.PHONY: clean

run:
	$(OUTDIR)/bin/https_service
.PHONY: run
