cmake -DCMAKE_BUILD_TYPE:STRING=Release -H./C -B./build 
cmake --build build --config Release --target all

sudo cp build/ambilight /usr/bin/
sudo cp build/ambilight-color /usr/bin/