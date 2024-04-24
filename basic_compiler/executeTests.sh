# This will run our compiler on a C file, export its LLVM ASM to a .ll file and output how it went to the log.
# The LLVM ASM is than ran and its outut sent to the execution log.
function runTest {
    ./run.sh -i tests/$1.c -fAsm -o tests/$1.ll &> tests/$1.log
    lli tests/$1.ll &> tests/$1Execution.log
}

# Run all the C file tests.
for file in "tests"/*.c; do
    file=$(basename $file .c)
    echo "Testing: $file"
    runTest $file
done