.PHONY: wasm all important clean

all: wasm
wasm: out/vcd.wasm


WASM_MAIN = wasm_main.cpp

HPP_FILES = \
vcd_parser.h \
wasm_main.hpp \


CPP_FILES = \
vcd_parser.c \
vcd_spans.c \



# this is a list of all C functions we want to publish to javascript
# In the main cpp file, each of these is wrapped in extern "C" {}
# the version here has a prepended underscore
# all lines must have trailing comma
EXPORT_STRING = \
"_execute", \
"_init", \
"_setTrigger", \
"_getTime", \

# warning and error flags
CLANG_WARN_FLAGS = \
-flto \
-fno-exceptions \
-Wl,--lto-O3 \
-Wall -Wextra \
-Wno-ignored-qualifiers \
-Wundef \
-Werror=return-type \
-Wshadow \
# -Wconversion


CLANG_OTHER_FLAGS = \
-DVCDWASM \



CLANG_O_FLAG = '-Oz'

ifdef NOOPT
  CLANG_O_FLAG = ' '
endif

ifdef OPT3
  CLANG_O_FLAG = '-O3'
endif

# works however slows down
#-s DISABLE_EXCEPTION_CATCHING=0 \


out/vcd.wasm: $(WASM_MAIN) $(CPP_FILES) $(HPP_FILES) Makefile
	mkdir -p out
	emcc \
	$(WASM_MAIN) \
	$(CPP_FILES) \
	-o out/vcd.html \
	-s DISABLE_EXCEPTION_CATCHING=1 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s ALLOW_TABLE_GROWTH=1 \
	-s MODULARIZE=1 \
	-s EXPORT_NAME=createVCD \
	-s EXPORTED_FUNCTIONS='[$(EXPORT_STRING) "_main"]' \
	-s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "addOnPostRun", "addFunction", "setValue", "getValue"]' \
	$(CLANG_O_FLAG) $(CLANG_WARN_FLAGS) $(CLANG_OTHER_FLAGS)

# -s WASM=0 \

.PHONY: patchlib patchlib1 patchlib2

# patchlib: patchlib1 patchlib2

# PTSRC1=lib/BehaviorTree.CPP/src/xml_parsing.cpp
# PTPAT1=patch/xml_parsing.patch
# # see https://stackoverflow.com/questions/7394290/how-to-check-return-value-from-the-shell-directive
# PAPPLIED1 := $(shell patch -R -p0 -s -f --dry-run $(PTSRC1) < $(PTPAT1) 1>&2 2> /dev/null > /dev/null; echo $$?)

# # patch is pretty annoying to use here
# # we would like to apply the patch, or skip if already applied exit 0
# # inorder to do this, we first need to run a dry-run in the reverse direction
# # then check the exit code, then run it in the forward direction if actually needed
# # we also have to do the complicated line above to deal with exit codes
# # see https://unix.stackexchange.com/questions/55780/check-if-a-file-or-folder-has-been-patched-already
# patchlib1: $(PTPAT1)
# ifneq ($(PAPPLIED1),0)
# 	@echo "$(PTSRC1) is unpatched.\n"
# 	patch --forward --reject-file=- $(PTSRC1) < $(PTPAT1)
# else
# 	@echo "$(PTSRC1) already patched, skipping..."
# endif





# PTSRC2=lib/BehaviorTree.CPP/src/tree_node.cpp
# PTPAT2=patch/tree_node.patch
# PAPPLIED2 := $(shell patch -R -p0 -s -f --dry-run $(PTSRC2) < $(PTPAT2) 1>&2 2> /dev/null > /dev/null; echo $$?)

# patchlib2: $(PTPAT2)
# ifneq ($(PAPPLIED2),0)
# 	@echo "$(PTSRC2) is unpatched.\n"
# 	patch --forward --reject-file=- $(PTSRC2) < $(PTPAT2)
# else
# 	@echo "$(PTSRC2) already patched, skipping..."
# endif




.PHONY: all build watch dev start test pretest lint jestc copydist cleandist prepare
.PHONY: test testonly


watch:
	npm run watch

test:
	npm run test

testonly:
	npm run testonly

prepare:
	npm run prepare


clean:
	rm -rf out/*
