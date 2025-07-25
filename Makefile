PKG_CONFIG = pkg-config
PKG_FLAGS = gtk+-3.0 gtk-layer-shell-0
CXX = g++
CXXFLAGS = -Wall -g -Iinclude -Ithird_party $(shell $(PKG_CONFIG) --cflags $(PKG_FLAGS))
LDFLAGS = $(shell $(PKG_CONFIG) --libs $(PKG_FLAGS)) -lX11

SRC = src/main.cc
OUT = xwccbar

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OUT)
