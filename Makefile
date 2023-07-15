CXX = clang++
CXXFLAGS = --std=c++11 -O3 -Wall -Werror

TARGET = rvemu

$(TARGET): src/rvemu.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run r: $(TARGET)
	./$(TARGET) $(args)

clean c:
	rm -f $(TARGET)

scheck:
	clang --analyze --verbose -Xanalyzer -analyzer-output=text src/rvemu.cpp

asan: CXXFLAGS += -fsanitize=address
asan: $(TARGET)
	-./$(TARGET) > asan_report.txt 2>&1
	grep "ERROR: AddressSanitizer" asan_report.txt && ASan has detected an error. Please check the asan_report.txt file for details."

test:
	./$(TARGET) ./test/$(file)

.PHONY: clean run scheck asan test
