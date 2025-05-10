CXXFLAGS ?= -std=c++20 -Wall -Wextra

ifdef DEBUG
    CXXFLAGS += -g
endif

INCLUDES = -I../libraries/doctest/ \
           -I../src/

CXXFLAGS += $(INCLUDES)

TESTS = test_geometry test_fixed test_string

GEO_DEPS = ../src/frog/geometry/general.cpp \
           ../src/frog/geometry/camera.cpp \
           ../src/frog/geometry/objects.cpp \
           ../src/frog/geometry/paddle.cpp \
           ../src/frog/geometry/physics.cpp \
           ../src/frog/geometry/polar.cpp \
           ../src/frog/geometry/transform.cpp

STR_DEPS = ../src/frog/utils/string.cpp \
           ../src/frog/utils/ini.cpp \
           ../src/frog/utils/exception.cpp

RUN_TESTS = $(TESTS:%=run.%)

all: $(TESTS)

run.%: %
	./$*

run_all: $(RUN_TESTS)
	@echo "Tests ok"

test_geometry: test_geometry.cpp $(GEO_DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_fixed: fixed/test_fixed.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

bench_fixed: fixed/bench_fixed.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

test_string: test_string.cpp $(STR_DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:

distclean:
	-$(RM) $(TESTS) bench_fixed

.PHONY: clean distclean all run_all
