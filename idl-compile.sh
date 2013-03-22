#!/bin/sh
echo Compiling Aeon Core IDL
rm -rf Aeon/Core/Source/Serialization
mkdir Aeon/Core/Source/Serialization
protoc --cpp_out=Aeon/Core/Source/Serialization --proto_path=Aeon/Core/IDL/ Aeon/Core/IDL/*.proto
rm -rf Aeon/Core/Include/Aeon/Serialization
mkdir Aeon/Core/Include/Aeon/Serialization
cp Aeon/Core/Source/Serialization/*.h Aeon/Core/Include/Aeon/Serialization -v
echo Compiling Game IDL
rm -rf Game/Source/Serialization
mkdir Game/Source/Serialization
protoc --cpp_out=Game/Source/Serialization --proto_path=Game/IDL  --proto_path=Aeon/Core/IDL/ Game/IDL/*.proto
rm -rf Game/Include/Serialization
mkdir Game/Include/Serialization
cp Game/Source/Serialization/*.h Game/Include/Serialization -v
