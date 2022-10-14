TARGET = https_service
OUTDIR = ../build/https_service
OBJECTS = $(OUTDIR)/obj/main.o \
	$(OUTDIR)/obj/https_session.o \
	$(OUTDIR)/obj/service.o \
	$(OUTDIR)/obj/service_helpers.o \
	$(OUTDIR)/obj/endpoints.o
CURR_DIR = $(shell pwd)
LDFLAGS = "-Wl,-rpath,$(CURR_DIR)/../build/boostbeast/lib"
LDFLAGS += "-Wl,-rpath,$(CURR_DIR)/../build/openssl/lib"
CXXFLAGS=-std=c++17 -g -O0 -Wall -Wextra -Werror -pedantic

$(TARGET): $(OBJECTS)
	mkdir -p $(OUTDIR)/bin
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $(OUTDIR)/bin/$(TARGET) \
		$(OBJECTS) \
		-L../build/boostbeast/lib \
		-L../build/openssl/lib \
    	-lboost_filesystem \
    	-lboost_system \
    	-lboost_thread \
    	-lcrypto.3 \
    	-lssl.3 \

$(OUTDIR)/obj/main.o: main.cxx \
		$(OUTDIR)/obj/https_session.o \
    	$(OUTDIR)/obj/service.o \
    	$(OUTDIR)/obj/service_helpers.o \
    	$(OUTDIR)/obj/endpoints.o
	$(CXX) $(CXXFLAGS) \
		-I../build/boostbeast/include \
		-I../build/openssl/include -c main.cxx -o $@

$(OUTDIR)/obj/https_session.o: https_session.cxx \
	$(OUTDIR)/obj/service.o
	$(CXX) $(CXXFLAGS) -I../build/boostbeast/include \
		-I../build/openssl/include -c https_session.cxx -o $@

$(OUTDIR)/obj/service.o: service.cxx \
		$(OUTDIR)/obj/service_helpers.o \
		$(OUTDIR)/obj/endpoints.o
	$(CXX) $(CXXFLAGS) -I../build/boostbeast/include \
		-I../build/openssl/include -c service.cxx -o $@

$(OUTDIR)/obj/endpoints.o: service_helpers.cxx
	mkdir -p $(OUTDIR)/obj
	$(CXX) $(CXXFLAGS) -I../build/boostbeast/include \
		-I../build/openssl/include -c endpoints.cxx -o $@

$(OUTDIR)/obj/service_helpers.o: service_helpers.cxx
	mkdir -p $(OUTDIR)/obj
	$(CXX) $(CXXFLAGS) -I../build/boostbeast/include \
		-I../build/openssl/include -c service_helpers.cxx -o $@

all: $(TARGET)
.PHONY: all

clean:
	rm -rf $(OUTDIR)/*
.PHONY: clean

run:
	$(OUTDIR)/bin/https_service
.PHONY: run