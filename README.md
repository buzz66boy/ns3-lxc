This is a work in progress.

The purpose of this project is to integrate Network Simulator 3 (ns-3) and Linux Containers (lxc) and automate their usage.

The program will use config/topology files to automate creation of linux containers and interface them with ns-3 to simulate a network.
Settings may be set in the settings.conf file.

Resources including example topology files can be found in the resources folder.

Setup
	YAML:
		Download yaml-cpp
		Follow the install instructions
		You may have to download and install Boost as well
	NS-3:
		Download the latest NS-3 release to a directory
		run `./build.py'
		cd into the built director (usually "ns-3.26" or appropriate version)
		run `CXXFLAGS="-std=c++11" ./waf -d debug --enable-examples --enable-tests configure`
			this configures NS-3 for C++11
