# The ns3-lxc Project
ns-3 automation with Linux Container integration

## Download the latest version of ns3-lxc
Version 0.1 ([details](https://github.com/buzz66boy/ns3-lxc/releases))
- zip: https://github.com/buzz66boy/ns3-lxc/archive/v0.1.zip
- tar.gz: https://github.com/buzz66boy/ns3-lxc/archive/v0.1.tar.gz

## Overview
The aim of the ns3-lxc project is to create a program that automates and integrates the usage of Linux Containers (LXCs) with the real-time simulation capabilities of ns-3; abstracting away the need for users to write their own ns-3 script, instantiate their own containers, and ease the process of running applications on those containers. Primary use-cases for ns3-lxc include: testing of network routing protocols, simulation of large-scale, rapidly changing topologies, testing of new ns-3 modules, and website development and testing.

A more in-depth overview may be found in the 'docs/report.odt' document.

In essence, the project uses YAML-based topology files to automate creation of linux containers and interface them with ns-3 to simulate a network.
Settings may be set in the settings.conf file.

Resources, including example topology files and a readme on how to write topology files can be found in the resources folder.

# Links
- Linux Containers - https://linuxcontainers.org/lxc/introduction/
- ns-3 - https://www.nsnam.org/
- yaml-cpp - https://github.com/jbeder/yaml-cpp

# Setup
## Dependencies
- yaml-cpp - libyaml-cpp-dev

- ns-3.26

- LXC - lxc lxc-dev

- CMake - cmake

- GCC - gcc

### YAML
- Download yaml-cpp (libyaml-cpp-dev) and follow the install instructions.
- You may have to download and install the Boost libraries as well (libboost-dev).


### NS-3
- Download the 3.26 NS-3 release to a directory (currently supported).
- Run `./build.py`.
- cd into the built directory (usually "ns-3.26" or appropriate version).
run `CXXFLAGS="-std=c++11" ./waf -d debug --enable-examples --enable-tests configure`
this configures NS-3 for C++11.
- The simulations output xml for NetAnim. NetAnim build instructions may be found here: https://www.nsnam.org/wiki/NetAnim_3.105#Building_and_Starting_NetAnim

### LXC
- Install LXC via package manager `sudo apt install lxc`.
- Install LXC Dev for the headers `sudo apt install lxc-dev`.

## Running the program for the first time
- Edit the settings.yaml file to:
  - Reflect the path to ns-3 (the directory where the "waf" executable exists).
  - The appropriate temporary directories. You can create them or let the program create them for you. 
- build the program from the cloned directory by running:
```
cmake .
make -j8
```
- If anything fails on the cmake/make stage, it is most likely because dependencies are not installed on the system properly
- It is recommended to run the "resources/test_cases/testCase1.yaml" file first with the command `bin/ns3lxc resources/test_cases/testCase1.yaml`
- **NOTE**: First run will take some time on the container creation phase, as it will download the appropriate template and you will see no output from this step. You can avoid this by creating a container manually via lxc of the appropriate type with a command such as`lxc-create -n nodeName -t download` and selecting the appropriate distro, release, and architecture.

## Program Options (help text)
usage: 'sudo bin/ns3lxc pathToTopology [-n] [-c] [-s] [-g] [-h]'
- -n: write only ns-3 script SUDO NOT REQUIRED
- -c: cleanup mode, teardown bridges, taps, and containers
- -s: spawn mode, do not teardown after simulation completion
- -g: GDB mode, run ns-3 script with GDB
- -h: show this help text
