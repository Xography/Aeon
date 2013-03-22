@echo off
@echo Compiling Aeon Core IDL
rmdir /S /Q Aeon\Core\Source\Serialization
mkdir Aeon\Core\Source\Serialization
Dependencies\protobuf-compiler\protoc --cpp_out=dllexport_decl=AEON_EXPORT:Aeon/Core/Source/Serialization --proto_path=Aeon/Core/IDL/ Aeon/Core/IDL/*.proto
rmdir /S /Q Aeon\Core\Include\Aeon\Serialization
mkdir Aeon\Core\Include\Aeon\Serialization
FOR /R "Aeon\Core\Source\Serialization" %%G IN (*.h) DO move "%%G" "Aeon\Core\Include\Aeon\Serialization"
@echo Compiling Game IDL
rmdir /S /Q Game\Source\Serialization
mkdir Game\Source\Serialization
Dependencies\protobuf-compiler\protoc --cpp_out=Game/Source/Serialization --proto_path=Game/IDL  --proto_path=Aeon/Core/IDL/ Game/IDL/*.proto
rmdir /S /Q Game\Include\Serialization
mkdir Game\Include\Serialization
FOR /R "Game\Source\Serialization" %%G IN (*.h) DO move "%%G" "Game\Include\Serialization"