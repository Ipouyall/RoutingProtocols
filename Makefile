# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11
SRCDIR = codes
OBJDIR = obj
BINDIR = bin
TESTDIR = test
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/RoutingProtocols

# ANSI color codes
RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m

# Build rules
build: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test rules
test: build
	@for i in $(TESTDIR)/*.input; do \
		echo "Running test $${i%.input}"; \
		./$(TARGET) test < $$i > $${i%.input}.result; \
		if diff -w -bB $${i%.input}.result $${i%.input}.output >/dev/null; then \
			echo "$(GREEN)  Test Passed.$(NC)"; \
		else \
			echo "$(RED)  Test Failed!$(NC)"; \
		fi; \
		rm $${i%.input}.result; \
	done

# Evaluation rules
eval: build
	@echo "$(GREEN)  Starting evaluation...$(NC)"; \
	./$(TARGET) < ./evaluation/eval > ./evaluation/result;

# Default, we want to build and test program
all: build test

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(TESTDIR)/*.result

.PHONY: all build test clean
