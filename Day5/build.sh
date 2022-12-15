echo "Starting Building"
d=$(date +%s)
g++ -Wall -Wextra -pedantic -std=c++20 main.cpp -o main.exe
echo "Build took $(($(date +%s)-d)) seconds."
