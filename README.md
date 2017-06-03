The aim of the ns3-lxc project is to create a program that automates and integrates the usage of Linux Containers (LXCs) with the real-time simulation capabilities of ns-3; abstracting away the need for users to write their own ns-3 script, instantiate their own containers, and ease the process of running applications on those containers. Primary use-cases for ns3-lxc include: testing of network routing protocols, simulation of large-scale, rapidly changing topologies, testing of new ns-3 modules, and website development and testing.

A more in-depth overview may be found in the 'docs/report.odt' document.

In essence, the project uses YAML topology files to automate creation of linux containers and interface them with ns-3 to simulate a network.
Settings may be set in the settings.conf file.

Resources, including example topology files and a readme on how to write topology files can be found in the resources folder.

# Links
Linux Containers - https://linuxcontainers.org/lxc/introduction/

ns-3 - https://www.nsnam.org/

yaml-cpp - https://github.com/jbeder/yaml-cpp

# Setup
## Dependencies
yaml-cpp - libyaml-cpp-dev

ns-3.26

LXC - lxc lxc-dev


## YAML
Download yaml-cpp (libyaml-cpp-dev) and follow the install instructions.
You may have to download and install the Boost libraries as well (libboost-dev).


## NS-3
Download the 3.26 NS-3 release to a directory (currently supported).
Run `./build.py`.
cd into the built directory (usually "ns-3.26" or appropriate version).
run `CXXFLAGS="-std=c++11" ./waf -d debug --enable-examples --enable-tests configure`
this configures NS-3 for C++11.
The simulations output xml for NetAnim. NetAnim build instructions may be found here: https://www.nsnam.org/wiki/NetAnim_3.105#Building_and_Starting_NetAnim

## LXC
Install LXC via package manager `sudo apt install lxc`. Install LXC Dev for the headers
`sudo apt install lxc-dev`.
