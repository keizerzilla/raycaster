windres assets/raycaster.rc -O coff -o assets/raycaster.res
make -f Makefile_Windows
start "" build/raycaster.exe