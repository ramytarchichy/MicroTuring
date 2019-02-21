#Output file names
OUT=mrun
OUT_DBG=$(OUT)_dbg

#Project files
FILES_COMPILE=main.c interpreter.c
FILES_DEPEND=$(FILES_COMPILE) Makefile interpreter.h

#Compiler and common flags
CC=clang $(FILES_COMPILE) -O2 -march=native

#Program test arguments
ARGS=

#Release version
$(OUT): $(FILES_DEPEND)
	$(CC) $(CC_FLAGS) -o $(OUT)
	strip -s $(OUT)

#Debug version
$(OUT_DBG): $(FILES_DEPEND)
	$(CC) -o $(OUT_DBG) -g -fsanitize=address -fsanitize-address-use-after-scope -fno-optimize-sibling-calls


.PHONY: clean run debug

clean:
	-rm $(OUT) $(OUT_DBG)

run: $(OUT)
	./$(OUT) $(ARGS)

debug: $(OUT_DBG)
	./$(OUT_DBG) $(ARGS)
