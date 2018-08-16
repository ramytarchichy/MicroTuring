BUILD_DIR=build

COMMON_DIR=src/common
COMMON_FILES="$(COMMON_DIR)/common.cpp"

INTERPRETER_DIR=src/Interpreter
INTERPRETER_FILES="$(INTERPRETER_DIR)/main.cpp" "$(INTERPRETER_DIR)/Interpreter.cpp"
INTERPRETER_EXEC="$(BUILD_DIR)/i"

COMPILER_DIR=src/Compiler
COMPILER_FILES="$(COMPILERr_DIR)/main.cpp" "$(COMPILER_DIR)/Token.cpp"
COMPILER_EXEC="$(BUILD_DIR)/icc"

COMPILER=clang++ -std=c++17 -O2 -I "$(COMMON_DIR)" $(COMMON_FILES)

.PHONY: clean interpreter Compiler

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

interpreter: $(BUILD_DIR)
	$(COMPILER) $(INTERPRETER_FILES) -o $(INTERPRETER_EXEC)

compiler: $(BUILD_DIR)
	$(COMPILER) $(COMPILER_FILES) -o $(COMPILER_EXEC)

clean: $(BUILD_DIR)
	rm -r $(BUILD_DIR)
