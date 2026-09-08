# Makefile - Seed Filter Minecraft (Tahap 1: Core Engine C)
#
# Target:
#   make static   -> lib/libseedfilter.a   (pustaka statis, Linux/macOS)
#   make dll       -> lib/seedfilter.dll    (Windows DLL, perlu MinGW cross-compiler)
#   make test      -> build/run_tests       (semua unit test)
#   make clean

CC       ?= gcc
STD      = -std=c99
WARN     = -Wall -Wextra
OPT      = -O2 -fwrapv
INCLUDES = -Iinclude -Ithird_party/cubiomes

SRC       = src/seed.c src/validate.c src/finder.c src/output.c src/seedfilter.c
CUBIOMES  = $(filter-out third_party/cubiomes/tests.c, $(wildcard third_party/cubiomes/*.c))
OBJDIR    = build

OBJS         = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))
CUBIOMES_OBJS = $(patsubst third_party/cubiomes/%.c,$(OBJDIR)/cubiomes_%.o,$(CUBIOMES))

.PHONY: all static dll clean test

all: static

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(STD) $(WARN) $(OPT) $(INCLUDES) -c $< -o $@

$(OBJDIR)/cubiomes_%.o: third_party/cubiomes/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(STD) $(OPT) $(INCLUDES) -c $< -o $@

# ---- Pustaka statis (Linux/macOS) ----
static: $(OBJS) $(CUBIOMES_OBJS)
	@mkdir -p lib
	ar rcs lib/libseedfilter.a $(OBJS) $(CUBIOMES_OBJS)
	@echo "OK -> lib/libseedfilter.a"

# ---- Windows DLL (jalankan dengan MinGW cross-compiler) ----
# Contoh: make dll CC=x86_64-w64-mingw32-gcc
dll: $(SRC) $(CUBIOMES)
	@mkdir -p lib
	$(CC) $(STD) $(WARN) $(OPT) $(INCLUDES) -DSEEDFILTER_BUILD_DLL \
		-shared -o lib/seedfilter.dll $(SRC) $(CUBIOMES) \
		-Wl,--out-implib,lib/libseedfilter_dll.a -lm -lpthread \
		-static-libgcc
	@echo "OK -> lib/seedfilter.dll (+ lib/libseedfilter_dll.a untuk linking)"

# ---- Jalankan semua unit test (Langkah 2,4,3,5,6) ----
test:
	@mkdir -p build/tests
	$(CC) $(STD) $(WARN) $(OPT) $(INCLUDES) -o build/tests/test_seed tests/test_seed.c src/seed.c -lm
	$(CC) $(STD) $(WARN) $(OPT) $(INCLUDES) -o build/tests/test_validate tests/test_validate.c src/validate.c -lm
	$(CC) $(STD) $(WARN) $(OPT) -fwrapv $(INCLUDES) -o build/tests/test_finder tests/test_finder.c src/finder.c src/validate.c $(CUBIOMES) -lm -lpthread
	$(CC) $(STD) $(WARN) $(OPT) $(INCLUDES) -o build/tests/test_output tests/test_output.c src/output.c -lm
	$(CC) -std=c11 $(WARN) $(OPT) -fwrapv $(INCLUDES) -o build/tests/test_pipeline tests/test_pipeline.c $(SRC) $(CUBIOMES) -lm -lpthread
	@echo "=== test_seed ===";     ./build/tests/test_seed
	@echo "=== test_validate ==="; ./build/tests/test_validate
	@echo "=== test_finder ===";   ./build/tests/test_finder
	@echo "=== test_output ===";   ./build/tests/test_output
	@echo "=== test_pipeline ==="; ./build/tests/test_pipeline

clean:
	rm -rf $(OBJDIR) lib
