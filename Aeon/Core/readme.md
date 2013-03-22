#AEON Framework

The Aeon Core Framework library provides a set of versatile, commonly needed classes for building a high performance, data-driven real time application with C++.

It comes with the following features:

- Entity Layer
- Entity Database/Loaders
- Component Layer
- Templated Properties/Attributes
- Memory Pooling & Management
- Multicore support
- Task Scheduling
- Timers
- Point-to-point and publish-subscribe messaging
- Automated serialization layers
- UUID system
- Prebuilt common types
- Finite State Machine
- Bucketing

##Dependencies
- Boost >= 1.47 or a recent C++11 compiler providing std::thread (MSVC >= 2012 or GCC >= 4.8)
- Google Protocol Buffers >= 2.4.1
- CMake >= 2.8

##Building Aeon Core

In order to build Aeon Core, open CMakeLists.txt.

If you are on Windows, you need to specifiy the path to your PROTOBUF_LIB.
To build protobuf statically, download the sources from http://code.google.com/p/protobuf/downloads/list and build them for the platform/compiler you're using to compile Aeon with and point it to the protobuf root directory.

#License
Copyright (C) 2012 Aeon Development Team.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
