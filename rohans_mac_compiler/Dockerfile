# Use latest Ubuntu release, ensures we have latest LLVM.
FROM dokken/ubuntu-22.10

# Install necessary packages.
RUN apt-get update
RUN apt-get install -y build-essential clang llvm-dev cmake llvm

# Mount the lab files then build and run them.
WORKDIR /LLVM-Lab
CMD ./run.sh