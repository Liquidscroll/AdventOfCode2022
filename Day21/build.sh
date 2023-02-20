echo "Starting Building"
d=$(date +%s)
g++ -g -Wall -Wextra -pedantic -std=c++20 main.cpp -o main.exe
if [ $? -ne 0 ];
then
	echo "Build failed."
else
	echo "Build took $(($(date +%s)-d)) seconds."	
fi
