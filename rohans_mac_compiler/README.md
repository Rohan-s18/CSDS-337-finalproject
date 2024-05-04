# LLVM With AST Codegen
You already know how to use LLVM, but how is LLVM utilized to compile usable code? First, a parser is ran to create an Abstract Syntax Tree (AST) from input code. With this representation, we can modify it as we please. In this assignment, you will extend the given AST to allow for a toy C implementation. Feel free to use this setup as inspiration for future projects.

## DETAILED Setup and Build Instructions for WINDOWS (In Case Desired)
### Install WSL:
1. Open Powershell as administrator (you can do this by right-clicking *PowerShell* in your start menu, and selecting *Run as administrator*
2. Run `wsl --install` in the command line interface. This will install the Ubuntu distribution of Linux.
3. Restart your computer.
4. Open the app *Ubuntu on Windows*. You should be tasked with setting your username and password.
    > You will have to enter your password periodically when you run a `sudo` (abbreviation of "superuser do") command on linux. When you enter your password, nothing will appear on screen (this is called "blind typing" and is normal), but the machine is still reading your keystrokes.
5. You should regularly update and upgrade your packages by running the commands `sudo apt update` and `sudo apt upgrade`. Do this now.
### Install the required packages:
- Run `sudo apt install clang`. This installs the **CLANG** compilers for C and C++, which are the compilers that LLVM uses.
- Run `sudo apt install make`. This installs the **Make** utility, which is used for directing compilation of projects using "makefiles".
- Run `sudo apt install cmake`. This installs **CMake** utility, which is used to generate the build files.
    > **Build files** are files that describe targets that own the source files.
      **Source files** are files that contain source code, like .cpp or .java files.
- Run `sudo apt install llvm-dev`. This installs the **LLVM** (Low-Level Virtual Machine) package as well as its development package which includes libraries and headers for llvm source code.
- Run `sudo apt install flex`. This installs **Flex** (Fast Lexical Analyzer Generator), which is a program that generates lexical analyzers out of flex code.
- Run `sudo apt install bison`. This installs **Bison** which is a program that generates parsers using the
- Run `sudo apt install graphviz`. This installs the graph visualization software that we will use to visualize control flow and data flow of compiled programs.
### Download and navigate to the project:
1. Outside the *Ubuntu on Windows* app (using a browser of your choice, on your Windows machine) download the zip file at `https://github.com/Gota7/LLVM-Lab/archive/refs/heads/main.zip`.
    > The folder inside the zip file be named **LLVM-Lab-main** and should contain a several files, including a **.gitignore**, a **README.md**, a **CMakeLists.txt** and a **src** folder.
2. Copy the folder inside the zip file onto your desktop.
3. Open the *Ubuntu on Windows* app.
4. We need to change the working directory of Ubuntu to the *LLVM-Lab-main* folder on your desktop. To do this, use the `cd` command ("Change Directory") to traverse your system's file tree.
    >The working directory you start in when *Ubuntu on Windows* is launched is `~`, which is semantically the same as `/home/yourwslusername` (this is the starting directory of _Ubuntu on Windows_, and it can be configured).
    >
    >`..` refers to the current working directory's "parent" directory (which is the directory that contains the working directory), so Running `cd ..` (note the space in between the `cd` and the `..`) while in `~` will set the working directory to `/home`, executing an upwards traversal of the file tree. Do this once more, and you should be in the directory `/`, which is the root directory of your file system.
    #### TIP: To print the name of your current working directory, run `pwd` ("Print Working Directory"). To see a list of all directories and files contained inside your working directory, run `ls` ("list").
5. The "C:" drive on Windows is organized in WSL as the directory `/mnt/c`, so running `cd /mnt/c` will bring you to your "C:" drive.
6. From there, use successive `cd` commands to navigate your file tree (which from here on out, can be seen in your Windows File Explorer).
    #### TIP: The traversal should look something like `cd /mnt/c/Users/yourwindowsusername/Onedrive/Desktop/LLVM-Lab-main`.
### Build and run the project:
1. Run `mkdir build`. This creates a new folder in LLVM-Lab-main named **build**.
2. Run `cd build`. This sets the working directory to `../LLVM-Lab-main/build`.
3. Run `cmake ..`. This uses the cmake to write the build files into the current directory, **build**.
4. Run `make -j8`. This uses the files in the **build** folder to build the c++ files in the **src** folder, and places the resulting files in a new, automatically created folder named **bin**.
5. Run `cd ../bin`. This sets the directory to the new **bin** folder.
6. Now, when you run `ls` you should see a single file named **LLVM-Lab**.
    #### TIP: To execute a file, simply run `./FileName`.
7. Run `./LLVM-Lab` to execute **LLVM-Lab**. This should create 2 more files in **bin**, which are **testMod.bc** and **testMod.ll**. Both of them are LLVM source code files (one is in assembly, and one is in bitcode), and each can be compiled into an executable program.
8. To execute one of these files, run `lli testMod.bc`. Congratulations, you have executed your first LLVM-interpreted program!
>Every time you want to re-build and run the project, you must this section. This can be tedious, and for this reason we would like to present the opportunity to dip your toe in some shell scripting, to automate the process.
>1. Create a file named **run.sh** in **LLVM-Lab-main**. You can do this by creating an empty text (.txt) file in, and renaming it to **run.sh**.
>2. Edit the file using a text editor. Type the command you would like to run, in order.
>3. To run the bash script, run `./run.sh`. This will execute each command in order, so you don't have to do it manually.
### Display a Control Flow graph of testMod.bc:
1. In the bin directory, run `opt -dot-cfg-only testMod.bc`. This will create a **.main.dot** file, which contains a text representation of testMod's CFG.
2. Run `dot .main.dot -Tsvg -o testMod.svg`. This allows graphviz to interpret **.main.dot** into an image file named **testMod.svg**, which can be opened via the browser.

## Assignment
Your task is to make some modifications to the AST code. If you need help, you can always look at the guide section below for an overview on the design of the AST system. Also note that you are free to create test cases by making C files in the `tests` folder and running the `./executeTests.sh` file. Their output compiling log, execution log, and LLVM assembly will be generated.

### Part 1 - Subtraction
The addition expression is added, but not subtraction. Shouldn't be too different right? Remember that there is a difference between integer and floating point operations. This hint will apply to the other operators.

### Part 2 - Headless
We have code for converting booleans to integers, but the header file is missing. Could you make one for it? Make sure to make a static `Create` method for it like the other expressions have.

### Part 3 - Comparisons
Some types of comparisons seem to be missing from comparison expression. The header file has a list of all of them. Remember, are the integers we're dealing with signed or unsigned?

### Part 4 - Int 2 Bool
In C, when we have an integer expression we can use it as a boolean value (Ex: `if (num)`). How do we know whether or not the number results in true or false?

### Part 5 - Multiplication, Division, and Negation
You've been able to (hopefully) add subtraction operations. Now, you'll have to come up with both the header and source files. Make sure the headers are `multiplication.h`, `division.h`, and `negative.h`. The classes should also be called `ASTExpressionMultiplication`, `ASTExpressionDivision`, and `ASTExpressionNegation`. If these are not named properly, things may not work. Make sure to have the static `Create` function.

### Part 6 - Booligan
We need code to generate constant boolean expressions! There is code for `int` and `float`. Remember from the last assignment how booleans are represented in LLVM?

### Part 7 - And
The boolean or operator is given, though and has yet to be implemented. Make sure your header is `and.h` and has the class called `ASTExpressionAnd`. Make sure to include the static `Create` function as always. Also remember that when executing an and operation, the 2nd operand is not evaluated if the first is false. Also note that making stack variables when not necessary is bad practice. There is no expression that requires declaring a stack variable. You are not allowed to use `alloca` here. No heap memory either.

Here are some hints:
* How and statements work is a bit tricky to understand, so an example helps.

Consider the following:

```cpp
if (x && y) doThing();
```

Note that one property of the && operator is that if x is false, we don't calculate what y is to save resources.
But this involves jumping as well, so we must have a layout such as this:

```cpp
bool tmp = x;
if (tmp) tmp = y;
if (tmp) doThing();
```

It looks different yes, but try and convince yourself that it does the same exact thing as the code above.
* If x is false, then tmp is false, so we never calculate what y is and don't call the function.
* If x is true and y is false, then we still don't call the function.
* If x is true and y is true, then we finally do call the function.

This makes the control flow:

```cpp
entry:
    bool tmp = x;
    if (tmp) goto checkRight else goto cont;
checkRight:
    tmp = y;
    goto cont;
cont:
    // Check tmp here and use it for calling function.
```

So if we are ignoring the function call we do, `tmp` ends up representing the result of `x && y`. We know that if we don't need to check the right side, we have a false value. We also know that the final result depends on the right side. If we can't use stack memory, how do we get the result? There are 2 ways to do this, and one is much easier than the other :>

## Part 8 - For
So far, you have been working with expressions. For loops are statements rather than expressions. Create `for.h` in the `src/statements` folder and have the class be named `ASTStatementFor`. Its constructor should take an `std::unique_ptr<ASTStatement>` for the body, init, condition (actually this one is `std::unique_ptr<ASTExpression>`), and increment in that order. Note that anything but the body is allowed to be a nullptr and should be by default.

Here are some hints:
* It is completely possible for a for's condition to never be true, so even if does return something it's not confirmed.
* An exception to the above is if the init function returns something.

Suppose we have the following for loop example:

```cpp
for (init(), condition, increment())
{
    doThing();
}
```

We can desugar it to the following:

```cpp
// Redacted.
```

This is quite literally just the while loop with extra steps. But what if any of these components are null?
* init - Just don't include the statement in the entry block.
* condition - Just always jump to the for loop body.
* increment - Just exclude the increment part from the for loop body.

Another thing to note is that if any of these return at any time we are just done building the loop.

## AST Structure
Welcome to the AST! Feel free to extend this for your own future projects. But how does it work? Like LLVM, the AST itself has multiple parts that work together.

### Overview
* VarType - Represents a high level type. A type must be able to be converted to an LLVM type and check for equality with other types.
* Expression - Anything that results in a value without the need to declare stack memory or basic blocks (though you still may use basic blocks if it makes things easier). An expression must determine whether or not it results in an L-value or R-value. There are also static casting functions given to help if needed.
* Statement - Requires creating basic blocks but no stack allocations.
* ScopeTable - Maps variable names to types and values. Currently, ASTs have a scope table for "globals", and each function has a scope table for locals. If a local variable is not in scope, it will search for globals. This scoping system does not support arbitrary layers for this simple language, though you are welcome to add it if you want.
* Function - Declared with a function type and name, have stack variables declared in advance and can be given an optional definition. Functions are allowed to be variadic (have infinite parameters).
* AST - The abstract syntax tree. It contains functions, a scope table, and a function pass manager for optimizations. The `demo.cpp` files shows how everything joins together.

### Quick Review Of L-Values And R-Values
* L-Values are anything you can load from or store to (like a variable).
* Raw addresses are not L-Values, but they can be converted to one by using the dereference `*` operator.
* You can only assign to L-Values (when's the last time you ever assigned to `5` or the result of `a + b`)?
* Any L-Value can be converted to an R-Value by loading from the L-Value.