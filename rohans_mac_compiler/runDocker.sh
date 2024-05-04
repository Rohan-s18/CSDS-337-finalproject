# A lot is happening here, basically it's building the docker container and mounting the current directly to it.
docker run -v "$(pwd):/LLVM-Lab" --rm -it $(docker build -q .)

# So the created files lack our user permissions, we should set them here.
sudo chown -R $USER build
sudo chown -R $USER bin