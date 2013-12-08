Remodel: Make Done Right
=======
Motivation
---------------------

There is a problem with building code using make:
      It checks whether files are up-to-date based on their timestamps, which is not always reliable. Timestamps can also be insufficient when building dependencies in parallel. 

**Note** This tool was developed for a problem set for Prof. Emery Berger's CS630 class. The idea for the tool, the syntax for the RemodelFile, and the example below are due to him. 

Usage
---------------------
1) Build  Remodel from source (Makefile provided). See notes below on building it. 
2) Make a file called 'RemodelFile' that encodes the dependency structure of your project. See below for syntax.
3) Make a directory called 'remodel' in the same directory where RemodelFile is. 
4) Build your project!

How It Works
---------------------
You specify the dependency structure of your project using a RemodelFile, described below. This induces a DAG between the dependencies. 
The code considers all of the files in these dependencies and computes their MD5sum. It compares this hash value to a value cached from the last time Remodel was run.
(md5sums are cached in ./remodel/.md5-map). If the sum has changed, then the file is marked as changed. It and all of its children will need to be rebuilt.

We use the Intel Thread Building Blocks Flow Graph library for executing the build commands for the project in parallel. This instantiates a the DAG induced by the RemodelFile, where each node corresponds to a build command. Nodes' build commands are executed only when their parents nodes finish. 


RemodelFile Syntax
---------------------


Remodel uses a different grammar than make. Dependencies can appear in any order. If you execute remodel with no arguments, it should start with the pseudo-target DEFAULT. Otherwise, the root is the argument to remodel, as in remodel foo.o. 

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
Build using 'make.' The Makefile is configured to for macosx (it uses clang), but can be changed for other compilers easily. 
You will need to install the Intel Thread Building Blocks, 