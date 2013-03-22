#Aeon Framework

The Aeon Core Framework library provides a set of versatile, commonly needed classes for building a high performance, data-driven real time application with C++.

## Components
The Aeon Framework provides components for the core functionality of large scale, data intensive real time applications such as simulations, VR and games. It provides a collection of common classes and templates that are required when developing applications of this nature:

* Application Framework
* Dynamic module loader
* Logging System
* Processes / Process Manager 
* Multicore and Threading
* Memory Management & -Pooling
* Event / Messaging System
* Entity Layer
* Entity Database/Loaders
* Component Layer
* Task Scheduling
* Timers
* Automated serialization layers
* UUID system
* Prebuilt common types
* Finite State Machine
* Bucketing
* Extensible Serialization structure
* Delegates
* LUA/JavaScript (V8) Scripting
* Plugin Architecture (ZIP, XML)
* Abstract Subsystems (Renderer, Input, Audio, Platform, UI)

## Aeon Editor
The QT-based Aeon Editor allows developers (esp. non-programmers) to edit application/game components.

It loads the native Entities and Components from the Game.dll and allows the Designers to place those in the World Editor and create Entity Instances to their liking.
Every game mechanic (Quest, NPCs, Items, ..) requires a plugin so that the Designers can create those elements and serialize them to the GameDB.

## Directory Structure
- `Aeon/` contains the framework source code.
- `Dependencies/` contains some smaller dependencies for easier compilation 
- `AppDLL/`, `AppLauncher` contains the sample source code.
- `Tools/` contains the tools source code.

## Contributors

* Felix Bruckner
* Tom Gerrits
* Mikhail Yanakaev