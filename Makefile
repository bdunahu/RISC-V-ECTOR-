CXX = g++
PYTHON_VERSION = 3.10

CXXFLAGS = -I$(shell python$(PYTHON_VERSION)-config --includes)
LDFLAGS = $(shell python$(PYTHON_VERSION)-config --ldflags --embed)

SRCDIR = src

all: rv

rv: $(SRCDIR)/rv.cc
	$(CXX) $(CXXFLAGS) -o rv $(SRCDIR)/rv.cc $(LDFLAGS)

test:
	@echo "foo"

clean:
	rm -f rv
