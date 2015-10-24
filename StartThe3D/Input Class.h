#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

class InputClass
{
private:
	bool mKeys[256];
	bool mMouseDown;
	bool mWasMouseDown;

public:
	InputClass();
	InputClass(const InputClass& original);
	~InputClass();

	void Initialize();
	void KeyDown(unsigned int keyNumber);
	void KeyUp(unsigned int keyNumber);
	void MouseDown();
	void MouseUp();
	bool IsMouseDown();

	bool IsKeyDown(unsigned int keyNumber);

};

#endif
