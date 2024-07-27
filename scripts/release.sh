cmake -S .. -B ../build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=RELEASE -DHalcyon_BUILD_TESTS=OFF -DHalcyon_BUILD_EXAMPLES=ON
cmake --build ../build -j --config Release
