#===- runtime/Makefile -------------------------*- Makefile -*-===##

# Relative path to the top of the source tree.
#
LEVEL=..

#LLVM_BIN_DIR=/u/cs255/build-llvm-38/bin
CLANG_BIN_DIR=/u/cs255/build-llvm-38/bin
LLVM_BIN_DIR=/u/cs255/dc_llvm/build/bin


OPT= $(LLVM_BIN_DIR)/opt
LLC= $(LLVM_BIN_DIR)/llc
CC= $(CLANG_BIN_DIR)/clang
DIS= $(LLVM_BIN_DIR)/llvm-dis

benchmark := test case_a case_b case_c case_d case_e case_f case_g 

check: test.c
	$(CC) -O0 -c -emit-llvm test.c -o test.bc
	$(OPT) -load ../Debug+Asserts/lib/libLLVMInstCnt.so -nloop <test.bc> test.bc.opt

bench:
	for prog in $(benchmark); do\
		echo $(prog);\
		$(CC) -O0 -c -emit-llvm $(prog).c -o $(prog).bc;\
		$(OPT) -load ../Debug+Asserts/lib/libLLVMInstCnt.so -nloop <$(prog).bc> $(prog).bc.opt;\
	done;\

all:
	$(foreach prog, $(benchmark), $(CC) -O0 -c -emit-llvm $(prog).c -o $(prog).bc ;)
	$(foreach prog, $(benchmark), $(OPT) -load ../Debug+Asserts/lib/libLLVMInstCnt.so -nloop <$(prog).bc> $(prog).bc.opt ;)


hard: hard.c
	$(CC) -O0 -c -emit-llvm hard.c -o hard.bc
	$(OPT) -load ../Debug+Asserts/lib/libLLVMInstCnt.so -nloop <hard.bc> hard.bc.opt

ir:
	$(foreach prog, $(benchmark), $(DIS) $(prog).bc.opt ;)

clean:
	rm -f *.bc *.opt *.o *.ll

a: case_a.c
	$(CC) -O0 -c -emit-llvm case_a.c -o case_a.bc
	$(OPT) -load ../Debug+Asserts/lib/libLLVMInstCnt.so -nloop <case_a.bc> case_a.bc.opt

