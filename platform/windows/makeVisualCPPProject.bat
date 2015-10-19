
cd visual_project
cmake .. -G "Visual Studio 14 2015 ARM" ../../../ -DMINIENGINE_PLATFORM=windows -DCMAKE_CXX_FLAGS=/D_UNICODE  -DCMAKE_SYSTEM_NAME=WindowsStore  -DCMAKE_SYSTEM_VERSION=10.0