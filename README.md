Remodel: Make Done Right
=======
Motivation
---------------------

There is a problem with building code using make:
      It checks whether files are up-to-date based on their timestamps, which is not always reliable. Timestamps can also be insufficient when building dependencies in parallel. 


Usage
---------------------


How It Works
---------------------


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


You should store the dependencies on disk in a special directory called .remodel/, so that remodel will not re-execute any commands unless a dependency has been violated. 

Building Remodel From Source
----------------------------
There is one remaining situation where you should use Unix make: when you are building Remodel!
Build using 'make.' The Makefile is configured to for macosx (it uses clang), but can be changed for other compilers easily. 
You will need to install the Intel Thread Building Blocks, 