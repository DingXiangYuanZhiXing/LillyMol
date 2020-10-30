# LillyMol
LillyMol Public Code

### This repository is a compiled version in centos 7 with gcc-8 g++-8 gfortran-8
If you are currently working on centos:7 maybe you can run it without docker.
But you need to install the dependencies in the Dockerfile.

### You can build docker image with Dockerfile for other systems.

### Docker Usage
Just build with command `docker build --rm -t lillymol .`

Run the container with command `docker run -it --rm -w /app/test/ lillymol ./run_all_test.sh` to test the installation.
