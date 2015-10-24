#ifndef HELP_FUNCTIONS_AND_LIBRARIES_H
#define HELP_FUNCTIONS_AND_LIBRARIES_H

#include "OverLoad Functions.h"
#include <DirectXCollision.h>

#include <Windows.h>
#include <string>
#include <sstream>
#include <windowsx.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include "DirectXTK\WICTextureLoader.h"

using namespace std;

//Taken from Luna's book, makes sure that the object we try to release isn't NULL.
//If it isn't NULL, we release it and set it to NULL.
#define ReleaseCOM(x) if (x != NULL) { x->Release(); x = NULL; } 



#endif
