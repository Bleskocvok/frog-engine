@echo off

echo "# Run this script from the root folder of the project:"
echo "# (the folder where CMakeLists.txt is located)"
echo "# in PowerShell:"
echo "# PS      script/build.bat"
echo "# ===================================================="
echo ""

cmake.exe -G "MinGW Makefiles" -S game -B bin-win && cmake.exe --build bin-win -j4
