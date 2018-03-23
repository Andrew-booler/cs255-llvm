#===- test/Makefile -------------------------*- Makefile -*-===##

# Relative path to the top of the source tree.
#
LEVEL=..
BUILD=../build

#LLVM_BIN_DIR=/u/cs255/build-llvm-38/bin
#CLANG_BIN_DIR=/u/cs255/build-llvm-38/bin
CLANG_BIN_DIR=/LLVM_ROOT/build/bin
#LLVM_BIN_DIR=/u/cs255/dc_llvm/build/bin
LLVM_BIN_DIR=/LLVM_ROOT/build/bin

OPT= $(LLVM_BIN_DIR)/opt
LLC= $(LLVM_BIN_DIR)/llc
CC= $(CLANG_BIN_DIR)/clang
DIS= $(LLVM_BIN_DIR)/llvm-dis
RT = $(BUILD)/runtime

check: qitest.c
	$(CC) -O0 -c -emit-llvm qitest.c -o qitest.bc
	$(OPT) -load ../build/lib/libInstCounterPass.so -inst-count <qitest.bc> qitest.bc.opt
	$(LLC) -filetype=obj qitest.bc.opt -o qitest.o
	$(CC) qitest.o $(RT)/libInstCounting_rt.a -o qitest

ir:	qitest.bc.opt
	$(DIS) qitest.bc.opt

clean:
	rm -f *.bc *.opt *.o *.ll qitest
