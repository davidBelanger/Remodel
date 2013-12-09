Remodel: Make Done Right
=======
Motivation
---------------------

There is a problem with building code using make:

It checks whether files are up-to-date based on their timestamps, which is not always reliable. Timestamps can also be insufficient when building dependencies in parallel. 

**Note** This tool was developed for a problem set for Prof. Emery Berger's CS630 class. The idea for the tool, the syntax for the RemodelFile, and the example below are due to him. 

Setup
---------------------
1a) Build  Remodel from source (Makefile provided). See notes below on building it.This produces a binary called 'BuildRemodel.'

1b) Alternatively, download the pre-built binary provided with a release on github. 

2) Make a file called 'RemodelFile' that encodes the dependency structure of your project. See below for syntax.

3) Make a directory called 'remodel' in the same directory where RemodelFile is. 

4) Build your project!

Usage
---------------------
Syntax: 
  ./BuildRemodel (target)

The second argument is optional. If you don't specify it, it builds the target DEFAULT target described below. This is analogous to how unix make builds the 
first target listed in the Makefile if no target is specified. 

Special use case:

  ./BuildRemodel clean

This will delete the cached information for files' status, which will force BuildRemodel to build everything from scratch next time you run it. Note that this doesn't remove any intermediate files that you may want to remove (like you would specify in the 'clean' target in a Makefile). 


How It Works
---------------------
You specify the dependency structure of your project using a RemodelFile, described below. This induces a DAG between the dependencies. 
The code considers all of the files in these dependencies and computes their MD5sum. It compares this hash value to a value cached from the last time Remodel was run.
(md5sums are cached in ./remodel/.md5-map). If the sum has changed, then the file is marked as changed. It and all of its children will need to be rebuilt.


We use the Intel Thread Building Blocks Flow Graph library for executing the build commands for the project in parallel. This instantiates a the DAG induced by the RemodelFile, where each node corresponds to a build command. Nodes' build commands are executed only when their parents nodes finish. 


RemodelFile Syntax
---------------------


Remodel uses a different grammar than unix make. Dependencies can appear in any order. If you execute remodel with no arguments, it should start with the pseudo-target DEFAULT. Otherwise, the root is the argument to remodel, as in remodel foo.o. 


program ::= production*

production ::= target '<-' dependency (':' '"' command '"")

dependency ::= filename (',' filename)*

target ::= filename (',' filename)*


Example that builds the program baz from two source files, foo.cpp and bar.cpp. 

DEFAULT <- baz

baz <- foo.o, bar.o: "g++ foo.o bar.o -o baz"

foo.o <- foo.cpp : "g++ -c foo.cpp -o foo.o"

bar.o <- bar.cpp: "g++ -c bar.cpp -o bar.o"



Building Remodel From Source
----------------------------
There is one remaining situation where you should use Unix make: when you are building Remodel!

Build using 'make.' The Makefile is configured for macosx (it uses clang), but can be changed for other compilers easily. 
You will need to install the Intel Thread Building Blocks and OpenSSL libraries. These can be installed, for example, using macports. 

Testing Remodel
----------------------------

If you want to force BuildRemodel to build everything from scratch, use 'BuildRemodel clean.'
BuildRemodel will catch any exceptions from system calls for the build process that fail. 

We have provided examples for testing BuildRemodel in the 'example' subdirectory. There are 4 subdirectories in this, for 4 different tests. To run each, cd to the directory and execute '../../BuildRemodel'

*The Tests*

example1: Simple hello world with the same RemodelFile as the example above.

example1Failure: Same as example1, but with a bug in the specification of a build command in the RemodelFile. This shows that it properly catches build errors. 

example2: This has a copy of the source code for Remodel, and uses ../../Remodel to build itself. This one has a very simple RemodelFile with just one line.

example3: This is the same as example3, but the RemodelFile is more sophisticated, since it generates intermediate .o files. 

example3Failure: This is the same as example3, but it has a bug in the specification of the dependency DAG (BuildRemodel doesn't depend on fileParsing.o, but it should). If you run ../../BuildRemodel enough times, it will crash. Make sure to run ../../BuildRemodel clean between trials so that it builds from scratch. 

Code Tour
----------------------------

There are 4 main sub-tasks and 4 files:

**remodel.cpp** This just contains the main function, which parses the RemodelFile into an internal represenation, finds out whether files on disk are up to date (by checking their md5sum), and then executes the build commands (in parallel, when it can). Finally, we call a method from fileStatus.cpp to cache update md5sums for the built files. 

**fileParsing.cpp** This reads in RemodelFiles, parses them, and creates a DAG of DependencyNodes. There is one DependencyNode for every target in the RemodelFile. This has pointers to DependencyNodes for its parents and implements a method for doing a system call for the target's compile command.  

**fileStatus.cpp** This has code for returning a map from filenames to whether they are changed on disk. It loads in the previously cached md5sums from ./remodel/md5-map.  It says a file has changed if its md5sum has changed. We compute the MD5sum using the implementation in the openssl library. 

**parallelBuild.cpp** The RemodelFile encodes a DAG of dependencies, and the build command for a DependencyNode can be executed as soon as its parents have built. This is captured easily using the Intel Thread Building Blocks Flow Graph library. We create a DAG of continute_nodes, one for each DepndencyNode, where each of them has a callback that executes the relevant build command. The Flow Graph library is very user-friendly. You basically just declare the graph structure among the continue_node, tell it to start computing, and wait for all the lambdas to be called. The only tricky part is that the lambdas need to mutate a global structure for whether an intermediate file needs to be built. We wrap all accesses to this datastructure (FileStatus) using Thread Building Block scoped locks. 



