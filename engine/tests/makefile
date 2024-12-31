CXXFLAGS ?= -std=c++20 -Wall -Wextra -O2

INCLUDES = -I../libraries/doctest/ \
           -I../src/

CXXFLAGS += $(INCLUDES)

TESTS = test_engine test_fixed

GEO_DEPS = ../src/frog/geometry/general.cpp \
           ../src/frog/geometry/camera.cpp \
           ../src/frog/geometry/objects.cpp \
           ../src/frog/geometry/paddle.cpp \
           ../src/frog/geometry/physics.cpp \
           ../src/frog/geometry/polar.cpp \
           ../src/frog/geometry/transform.cpp

RUN_TESTS = $(TESTS:%=run.%)

all: $(TESTS)

run.%: %
	./$*

run_all: $(RUN_TESTS)
	@echo "Tests ok"

test_engine: test_engine.cpp $(GEO_DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_fixed: fixed/test_fixed.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:

distclean:
	-$(RM) $(TESTS)

.PHONY: clean distclean all run_all
