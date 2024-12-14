CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -lm
SHELL := /bin/bash

.PHONY: all run test time clean rm format

TARGET = main
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	@./$(TARGET)
	@$(MAKE) -s clean

test: $(TARGET)
	@passed_tests=0; \
	total_tests=0; \
	for input in $(wildcard ../tests/*.in); do \
		total_tests=$$((total_tests + 1)); \
		output=$${input%.in}.out; \
		result=$${input%.in}.result; \
		diff_file=$${input%.in}.diff; \
		timeout 90 ./$(TARGET) < $$input > $$result; \
		if [ $$? -eq 124 ]; then \
			echo ""; \
			test_name=$$(basename $$input .in); \
			echo "Failed $$test_name: Time limit exceeded"; \
		elif diff -q $$result $$output > /dev/null; then \
			printf "."; \
			passed_tests=$$((passed_tests + 1)); \
		else \
			echo ""; \
			test_name=$$(basename $$input .in); \
			echo "Failed $$test_name: See file $$diff_file"; \
			diff $$result $$output | tee $$diff_file; \
		fi; \
	done; \
	echo ""; \
	echo "Tests passed $$passed_tests out of $$total_tests"; \
	$(MAKE) -s clean

time: $(TARGET)
	@for input in $(wildcard ../tests/*.in); do \
		test_name=$$(basename $$input .in); \
		echo "$$test_name: "; \
		{ time ./$(TARGET) < $$input > /dev/null; } 2>&1 | grep -E 'real|user|sys';\
	done
	@$(MAKE) -s clean

clean:
	@rm -f $(TARGET)

rm:
	@$(MAKE) -s clean
	@rm -f ../tests/*.result ../tests/*.diff
	@rm -f ../tests/test_generator

format:
	@clang-format -i $(SRCS)

# TODO: Add new <V> <E> <L> <b> <seed> to the list of arguments.
new_test: ../tests/test_generator.cpp
	@$(CXX) $(CXXFLAGS) -o ../tests/test_generator ../tests/test_generator.cpp
	@last_test_num=$$(ls ../tests/test*.in 2>/dev/null | grep -o '[0-9]\+' | sort -n | tail -1 | sed 's/^0*//'); \
	if [ -z "$$last_test_num" ]; then \
		last_test_num=0; \
	fi; \
	new_test_num=$$((last_test_num + 1)); \
	for args in '8 8 8 8 8'; do \
		new_test_name=$$(printf "../tests/test%02d.in" $$new_test_num); \
		../tests/test_generator $$args > "$$new_test_name"; \
		test_name_no_ext=$$(basename "$$new_test_name" .in); \
		python3 ../tests/graph_maker.py "../tests/$$test_name_no_ext"; \
		new_test_num=$$((new_test_num + 1)); \
	done
	@$(MAKE) -s rm
