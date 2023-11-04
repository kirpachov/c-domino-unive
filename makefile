### If you wish to use extra libraries (math.h for instance),
### add their flags here (-lm in our case) in the "LIBS" variable.

###
CFLAGS  = -std=c99
CFLAGS += -g
# 02 is optimization: https://gcc.gnu.org/onlinedocs/gcc-3.1.1/gcc/Optimize-Options.html#Optimize%20Options
CFLAGS += -O2
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
CFLAGS += -DUNITY_SUPPORT_64 -DUNITY_OUTPUT_COLOR

ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

.PHONY: test
test: tests.out
	@./tests.out

.PHONY: memcheck
memcheck: ./*.c ./*.h
	@echo Compiling $@
	@$(CC) $(ASANFLAGS) $(CFLAGS) test-framework/unity.c ./*.c -o memcheck.out $(LIBS)
	@./memcheck.out
	@echo "Memory check passed"

.PHONY: clean
clean:
	rm -rf *.o *.out *.out.dSYM

tests.out: ./*.c ./*.h
	@echo Compiling $@
	@$(CC) $(CFLAGS) test-framework/unity.c ./*.c -o tests.out $(LIBS)

.PHONY: run
run: compile
	./main.out

.PHONY: compile
compile: src/main.c ./*.c ./*.h
	@echo Compiling $@
	@$(CC) $(CFLAGS) src/main.c -o main.out $(LIBS)