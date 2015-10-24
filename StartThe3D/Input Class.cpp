#include "Input Class.h"



#pragma region Constructors and Deconstructor
InputClass::InputClass()
{

}

InputClass::InputClass(const InputClass& original)
{

}

InputClass::~InputClass()
{

}

#pragma endregion

#pragma region Initialize

void InputClass::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		mKeys[i] = false;
	}
	mMouseDown = false;
	mWasMouseDown = false;
}

#pragma endregion

#pragma region KeyFunctions

void InputClass::KeyDown(unsigned int keyNumber)
{
	mKeys[keyNumber] = true;
}

void InputClass::KeyUp(unsigned int keyNumber)
{
	mKeys[keyNumber] = false;
}

bool InputClass::IsKeyDown(unsigned int keyNumber)
{
	return mKeys[keyNumber];
}

void InputClass::MouseDown()
{
	if (!mWasMouseDown)
	{
		mMouseDown = true;
		mWasMouseDown = true;
	}
	else
	{
		mMouseDown = false;
	}
}

void InputClass::MouseUp()
{
	mMouseDown = false;
	mWasMouseDown = false;
}

bool InputClass::IsMouseDown()
{
	return mMouseDown;
}

#pragma endregion