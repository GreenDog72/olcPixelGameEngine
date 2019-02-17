#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));
		
#ifdef __SWITCH__
		if (GetKey(olc::Key::JC_PLUS).bPressed) return false;
#endif
		
		return true;
	}
};


int main(int argc, char* argv[])
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4)) {
#ifdef __SWITCH__
		demo.SetHandheldPixelSize(3, 3);
#endif
		demo.Start();
	}

	return 0;
}
