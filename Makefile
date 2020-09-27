.PHONY: wasm all important clean

all: wasm
wasm: out/vcd.wasm


WASM_MAIN = wasm_main.cpp

HPP_FILES = \
# csrc/wrapper.hpp \


CPP_FILES = \
# csrc/wrapper.cpp \
# lib/BehaviorTree.CPP/src/action_node.cpp \
# lib/BehaviorTree.CPP/src/basic_types.cpp \
# lib/BehaviorTree.CPP/src/behavior_tree.cpp \
# lib/BehaviorTree.CPP/src/blackboard.cpp \
# lib/BehaviorTree.CPP/src/bt_factory.cpp \
# lib/BehaviorTree.CPP/src/decorator_node.cpp \
# lib/BehaviorTree.CPP/src/condition_node.cpp \
# lib/BehaviorTree.CPP/src/control_node.cpp \
# lib/BehaviorTree.CPP/src/shared_library.cpp \
# lib/BehaviorTree.CPP/src/tree_node.cpp \
# lib/BehaviorTree.CPP/src/decorators/inverter_node.cpp \
# lib/BehaviorTree.CPP/src/decorators/repeat_node.cpp \
# lib/BehaviorTree.CPP/src/decorators/retry_node.cpp \
# lib/BehaviorTree.CPP/src/decorators/subtree_node.cpp \
# lib/BehaviorTree.CPP/src/decorators/delay_node.cpp \
# lib/BehaviorTree.CPP/src/controls/if_then_else_node.cpp \
# lib/BehaviorTree.CPP/src/controls/fallback_node.cpp \
# lib/BehaviorTree.CPP/src/controls/parallel_node.cpp \
# lib/BehaviorTree.CPP/src/controls/reactive_sequence.cpp \
# lib/BehaviorTree.CPP/src/controls/reactive_fallback.cpp \
# lib/BehaviorTree.CPP/src/controls/sequence_node.cpp \
# lib/BehaviorTree.CPP/src/controls/sequence_star_node.cpp \
# lib/BehaviorTree.CPP/src/controls/switch_node.cpp \
# lib/BehaviorTree.CPP/src/controls/while_do_else_node.cpp \
# lib/BehaviorTree.CPP/src/loggers/bt_cout_logger.cpp \
# lib/BehaviorTree.CPP/src/loggers/bt_file_logger.cpp \
# lib/BehaviorTree.CPP/src/private/tinyxml2.cpp \
# lib/BehaviorTree.CPP/src/xml_parsing.cpp \


# this is a list of all C functions we want to publish to javascript
# In the main cpp file, each of these is wrapped in extern "C" {}
# the version here has a prepended underscore
# all lines must have trailing comma
EXPORT_STRING = \
# "_somefn", \
# "_int_sqrt", \
# "_pass_write_fn", \
# "_get_saved_node_count", \
# "_get_saved_node_name", \
# "_get_saved_node_id", \
# "_get_child_node_count", \
# "_get_child_node_id", \
# "_register_action_node", \
# "_register_condition_node", \
# "_unregister_builder", \
# "_parse_xml", \
# "_lt", \
# "_ltd", \
# "_reset_trackers", \
# "_reset_factory", \
# "_reset_all", \

# Functions used in debugging
# "_callBoundJs", \
# "_debug_example", \


# warning and error flags
CLANG_WARN_FLAGS = \
-Wall -Wextra \
-Wno-ignored-qualifiers \
-Wundef \
-Werror=return-type \
-Wshadow \
# -Wconversion


CLANG_OTHER_FLAGS = \
-DBT_NO_COROUTINES \



CLANG_O_FLAG = '-O3'

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
	emcc $(WASM_MAIN) $(CPP_FILES) -s WASM=1 -o out/vcd.html \
	-s DISABLE_EXCEPTION_CATCHING=0 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s ALLOW_TABLE_GROWTH=1 \
	-s EXPORTED_FUNCTIONS='[$(EXPORT_STRING) "_main"]' \
	-s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "addOnPostRun", "addFunction", "setValue", "getValue"]' \
	'-std=c++2a' $(CLANG_O_FLAG) $(CLANG_WARN_FLAGS) $(CLANG_OTHER_FLAGS)


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




.PHONY: all build watch dev start test pretest lint jestc copydist cleandist
.PHONY: test


test:
	npm run test


clean:
	rm -rf out/*

