#Aeon Framework

The Aeon Framework is a powerful, generic data-driven framework to implement real-time 2D/3D applications, such as simulations, VR and games. 

## Components
The Aeon Framework provides all core functionality for a game and abstract subsystems (Renderer, Input, Audio, Platform, UI). It provides a collection of common classes and templates that are required for developing for applications of this nature.

The components in detail:

* Application
* Logging System
* Process Manager / Multicore and Threading
* Memory Management & -Pooling
* Event/Messaging System
* Entity System
* Extensible Serialization structure
* Delegates
* LUA Scripting
* Plugins (Plugins/ZIP, Plugins/XML)
* Finite State Machine
* Basic AI (α-β-Pruning, A*)

## Aeon Editor
The QT-based Aeon Editor allows developers (esp. non-programmers) to edit the game components.

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