CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -lm
SHELL := /bin/bash

.PHONY: all run format test new_tests time excel clean rm

TARGET = main
SRCS = main.cpp
TEST_PATH = ../tests
TEST_GENERATOR_PATH = $(TEST_PATH)/test_generator
EXCEL_MAKER_PATH = $(TEST_PATH)/excel_maker.py
GRAPH_MAKER_PATH = $(TEST_PATH)/graph_maker.py

all: $(TARGET)

$(TARGET): $(SRCS)
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	@./$(TARGET)
	@$(MAKE) -s clean

format:
	@clang-format -i $(SRCS)

test: $(TARGET)
	@passed_tests=0; \
	total_tests=0; \
	for input in $(wildcard $(TEST_PATH)/*.in); do \
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

new_tests: $(TARGET) $(TEST_GENERATOR_PATH).cpp
	@$(CXX) $(CXXFLAGS) -o $(TEST_GENERATOR_PATH) $(TEST_GENERATOR_PATH).cpp
	@last_test_num=$$(ls $(TEST_PATH)/test*.in 2>/dev/null | grep -o '[0-9]\+' | sort -n | tail -1 | sed 's/^0*//'); \
	if [ -z "$$last_test_num" ]; then \
		last_test_num=0; \
	fi; \
	new_test_num=$$((last_test_num + 1)); \
	while IFS= read -r args; do \
		new_test_name=$$(printf "$(TEST_PATH)/test%02d.in" $$new_test_num); \
		$(TEST_GENERATOR_PATH) $$args > "$$new_test_name"; \
		test_name_no_ext=$$(basename "$$new_test_name" .in); \
		python3 $(GRAPH_MAKER_PATH) "$(TEST_PATH)/$$test_name_no_ext"; \
		new_test_num=$$((new_test_num + 1)); \
	done < $(TEST_PATH)/new_tests.txt
	@$(MAKE) -s rm clean

time: $(TARGET)
	@for input in $(wildcard $(TEST_PATH)/*.in); do \
		test_name=$$(basename $$input .in); \
		echo "$$test_name: "; \
		{ time ./$(TARGET) < $$input > /dev/null; } 2>&1 | grep -E 'real|user|sys';\
	done
	@$(MAKE) -s clean

excel: $(TARGET)
	@if [ -z "$(r)" ] || [ -z "$(s)" ] || [ -z "$(c)" ] || [ -z "$(l)" ]; then \
		echo "Usage: make excel r = <num_runs> s = <stations_exp> c = <use_connections> l = <lines_exponent>"; \
	else \
		python3 $(EXCEL_MAKER_PATH) $(r) $(s) $(c) $(l); \
	fi
	@$(MAKE) -s rm clean

clean:
	@rm -f $(TARGET)

rm:
	@$(MAKE) -s clean
	@rm -f $(TEST_PATH)/*.result $(TEST_PATH)/*.diff
	@rm -f $(TEST_GENERATOR_PATH)
