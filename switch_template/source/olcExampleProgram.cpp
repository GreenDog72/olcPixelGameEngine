/*
#include <string>
#include <map>
#include <functional>
#include <fstream>
#include <atomic>
#include <chrono>

#include <png.h>

#include <switch.h>

namespace olc {
	struct Pixel {
		union {
			uint32_t n = 0xFF000000;
			struct {
				uint8_t r; uint8_t g; uint8_t b; uint8_t a;
			};
		};
		
		Pixel();
		Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
		Pixel(uint32_t p);
		enum Mode { NORMAL, MASK, ALPHA, CUSTOM };
	};
	
	static const Pixel
		WHITE(255, 255, 255),
		GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
		RED(255, 0, 0), DARK_RED(128, 0, 0), VERY_DARK_RED(64, 0, 0),
		YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
		GREEN(0, 255, 0), DARK_GREEN(0, 128, 0), VERY_DARK_GREEN(0, 64, 0),
		CYAN(0, 255, 255), DARK_CYAN(0, 128, 128), VERY_DARK_CYAN(0, 64, 64),
		BLUE(0, 0, 255), DARK_BLUE(0, 0, 128), VERY_DARK_BLUE(0, 0, 64),
		MAGENTA(255, 0, 255), DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64),
		BLACK(0, 0, 0),
		BLANK(0, 0, 0, 0);
	
	enum rcode {
		FAIL = 0, OK = 1, NO_FILE = -1,
	};
	
	struct HWButton {
		bool bPressed = false;
		bool bReleased = false;
		bool bHeld = false;
	};
	
	class ResourcePack
	{
	public:
		ResourcePack();
		~ResourcePack();
		struct sEntry : public std::streambuf {
			uint32_t nID, nFileOffset, nFileSize; uint8_t* data; void _config() { this->setg((char*)data, (char*)data, (char*)(data + nFileSize)); }
		};

	public:
		olc::rcode AddToPack(std::string sFile);

	public:
		olc::rcode SavePack(std::string sFile);
		olc::rcode LoadPack(std::string sFile);
		olc::rcode ClearPack();

	public:
		olc::ResourcePack::sEntry GetStreamBuffer(std::string sFile);

	private:

		std::map<std::string, sEntry> mapFiles;
	};
	
	class Sprite
	{
	public:
		Sprite();
		Sprite(std::string sImageFile);
		Sprite(std::string sImageFile, olc::ResourcePack *pack);
		Sprite(int32_t w, int32_t h);
		~Sprite();

	public:
		olc::rcode LoadFromFile(std::string sImageFile, olc::ResourcePack *pack = nullptr);
		olc::rcode LoadFromPGESprFile(std::string sImageFile, olc::ResourcePack *pack = nullptr);
		olc::rcode SaveToPGESprFile(std::string sImageFile);

	public:
		int32_t width = 0;
		int32_t height = 0;
		enum Mode { NORMAL, PERIODIC };

	public:
		void SetSampleMode(olc::Sprite::Mode mode = olc::Sprite::Mode::NORMAL);
		Pixel GetPixel(int32_t x, int32_t y);
		void  SetPixel(int32_t x, int32_t y, Pixel p);

		Pixel Sample(float x, float y);
		Pixel SampleBL(float u, float v);
		Pixel* GetData();
	
	private:
		Pixel *pColData = nullptr;
		Mode modeSample = Mode::NORMAL;

#ifdef OLC_DBG_OVERDRAW
	public:
		static int nOverdrawCount;
#endif

	};
	
	enum Key {
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
		NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL,
#ifdef __SWITCH__
		// joycon buttons
		JC_A, JC_B, JC_X, JC_Y, JC_LSTICK, JC_RSTICK, JC_L, JC_R, JC_ZL, JC_ZR,
		JC_PLUS, JC_MINUS, JC_DLEFT, JC_DUP, JC_DRIGHT, JC_DDOWN,
		JC_LSTICK_LEFT, JC_LSTICK_UP, JC_LSTICK_RIGHT, JC_LSTICK_DOWN,
		JC_RSTICK_LEFT, JC_RSTICK_UP, JC_RSTICK_RIGHT, JC_RSTICK_DOWN,
		JC_SL_LEFT, JC_SR_LEFT, JC_SL_RIGHT, JC_SR_RIGHT
#endif
	};
	
	class PixelGameEngine {
	public:
		PixelGameEngine();
		
	public:
		olc::rcode Construct(uint32_t screen_w, uint32_t screen_h, uint32_t pixel_w, uint32_t pixel_h);
		olc::rcode Start();
		
	public: // Override Interfaces
		virtual bool OnUserCreate();
		virtual bool OnUserUpdate(float fElapsedTime);
		virtual bool OnUserDestroy();
		
	public: // Hardware Interfaces
		//bool IsFocused();
		HWButton GetKey(Key k);
		//HWButton GetMouse(uint32_t b);
		//int32_t GetMouseX();
		//int32_t GetMouseY();
		
	public: // Utility
		int32_t ScreenWidth();
		int32_t ScreenHeight();
		int32_t GetDrawTargetWidth();
		int32_t GetDrawTargetHeight();
		Sprite* GetDrawTarget();
		
	public: // Draw Routines
		void SetDrawTarget(Sprite* target);
		//void SetPixelMode(Pixel::Mode m);
		//Pixel::Mode GetPixelMode();
		//void SetPixelMode(std::function<olc::Pixel(const int x, const int y, const olc::Pixel& pSource, const olc::Pixel& pDest)> pixelMode);
		//void SetPixelBlend(float fBlend);
		//void SetSubPixelOffset(float ox, float oy);
		
		virtual void Draw(int32_t x, int32_t y, Pixel p = olc::WHITE);
		
	public: // Branding
		std::string sAppName;
		
	private: // Inner mysterious workings
		Sprite*     pDefaultDrawTarget = nullptr;
		Sprite*     pDrawTarget = nullptr;
		Pixel::Mode nPixelMode = Pixel::NORMAL;
		float       fBlendFactor = 1.0f;
		uint32_t    nScreenWidth = 256;
		uint32_t    nScreenHeight = 240;
		uint32_t    nPixelWidth = 4;
		uint32_t    nPixelHeight = 4;
		int32_t     nMousePosX = 0;
		int32_t     nMousePosY = 0;
		float       fPixelX = 1.0f;
		float       fPixelY = 1.0f;
		float       fSubPixelOffsetX = 0.0f;
		float       fSubPixelOffsetY = 0.0f;
		bool        bHasInputFocus = false;
		bool        bHasMouseFocus = false;
		float       fFrameTimer = 1.0f;
		int         nFrameCount = 0;
		Sprite*     fontSprite = nullptr;
		std::function<olc::Pixel(const int x, const int y, const olc::Pixel&, const olc::Pixel&)> funcPixelMode;
		
		static std::map<uint16_t, uint8_t> mapKeys;
		
		bool        pKeyNewState[256]{ 0 };
		bool        pKeyOldState[256]{ 0 };
		HWButton    pKeyboardState[256];
		
		bool        pMouseNewState[5]{ 0 };
		bool        pMouseOldState[5]{ 0 };
		HWButton    pMouseState[5];
		
		void EngineThread();
		static std::atomic<bool> bAtomActive;
		
		void olc_ConstructFontSheet();
		
#ifdef __SWITCH__
		NWindow*    olc_Window;
		Framebuffer olc_Framebuffer;
		
		u32         fbWidth = 1280;
		u32         fbHeight = 720;
		
		u32         screenOffsetX = 0;
		u32         screenOffsetY = 0;
		
		u8          operationMode;
		
		uint32_t    nDockedPixelWidth;
		uint32_t    nDockedPixelHeight;
		uint32_t    nHandheldPixelWidth;
		uint32_t    nHandheldPixelHeight;
		
		NWindow*    olc_WindowCreate();
		
public:
		void SetHandheldPixelSize(uint32_t w, uint32_t h);
		void SetDockedPixelSize(uint32_t w, uint32_t h);
#endif
	};
	
	Pixel::Pixel()
	{
		r = 0; g = 0; b = 0; a = 255;
	}

	Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		r = red; g = green; b = blue; a = alpha;
	}

	Pixel::Pixel(uint32_t p)
	{
		n = p;
	}
	
	Sprite::Sprite()
	{
		pColData = nullptr;
		width = 0;
		height = 0;
	}

	Sprite::Sprite(std::string sImageFile)
	{
		LoadFromFile(sImageFile);
	}

	Sprite::Sprite(std::string sImageFile, olc::ResourcePack *pack)
	{
		LoadFromPGESprFile(sImageFile, pack);
	}

	Sprite::Sprite(int32_t w, int32_t h)
	{
		if(pColData) delete[] pColData;
		width = w;		height = h;
		pColData = new Pixel[width * height];
		for (int32_t i = 0; i < width*height; i++)
			pColData[i] = Pixel();
	}

	Sprite::~Sprite()
	{
		if (pColData) delete pColData;
	}
	
	olc::rcode Sprite::LoadFromFile(std::string sImageFile, olc::ResourcePack *pack)
	{
		////////////////////////////////////////////////////////////////////////////
		// Use libpng, Thanks to Guillaume Cottenceau
		// https://gist.github.com/niw/5963798
		//
		// switch can use libpng as well. thank god.
		png_structp png;
		png_infop info;

		FILE *f = fopen(sImageFile.c_str(), "rb");
		if (!f) return olc::NO_FILE;

		png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) goto fail_load;

		info = png_create_info_struct(png);
		if (!info) goto fail_load;

		if (setjmp(png_jmpbuf(png))) goto fail_load;

		png_init_io(png, f);
		png_read_info(png, info);

		png_byte color_type;
		png_byte bit_depth;
		png_bytep *row_pointers;
		width = png_get_image_width(png, info);
		height = png_get_image_height(png, info);
		color_type = png_get_color_type(png, info);
		bit_depth = png_get_bit_depth(png, info);

#ifdef _DEBUG
		std::cout << "Loading PNG: " << sImageFile << "\n";
		std::cout << "W:" << width << " H:" << height << " D:" << (int)bit_depth << "\n";
#endif

		if (bit_depth == 16) png_set_strip_16(png);
		if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)	png_set_expand_gray_1_2_4_to_8(png);
		if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
		if (color_type == PNG_COLOR_TYPE_RGB ||
			color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
		if (color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);

		png_read_update_info(png, info);
		row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
		for (int y = 0; y < height; y++) {
			row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
		}
		png_read_image(png, row_pointers);
		////////////////////////////////////////////////////////////////////////////

		// Create sprite array
		pColData = new Pixel[width * height];

		// Iterate through image rows, converting into sprite format
		for (int y = 0; y < height; y++)
		{
			png_bytep row = row_pointers[y];
			for (int x = 0; x < width; x++)
			{
				png_bytep px = &(row[x * 4]);
				SetPixel(x, y, Pixel(px[0], px[1], px[2], px[3]));
			}
		}

		fclose(f);
		return olc::OK;

	fail_load:
		width = 0;
		height = 0;
		fclose(f);
		pColData = nullptr;
		return olc::FAIL;
	}
	
	Pixel Sprite::GetPixel(int32_t x, int32_t y)
	{
		if (modeSample == olc::Sprite::Mode::NORMAL)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
				return pColData[y*width + x];
			else
				return Pixel(0, 0, 0, 0);
		}
		else
		{			
			return pColData[abs(y%height)*width + abs(x%width)];
		}
	}
	
	void Sprite::SetPixel(int32_t x, int32_t y, Pixel p)
	{

#ifdef OLC_DBG_OVERDRAW
		nOverdrawCount++;
#endif

		if (x >= 0 && x < width && y >= 0 && y < height)
			pColData[y*width + x] = p;
	}
	
	Pixel* Sprite::GetData() { return pColData; }
	
	PixelGameEngine::PixelGameEngine()
	{
		sAppName = "Undefined";
		//olc::PGEX::pge = this;
	}
	
	olc::rcode PixelGameEngine::Construct(uint32_t screen_w, uint32_t screen_h, uint32_t pixel_w, uint32_t pixel_h)
	{
		nScreenWidth = screen_w;
		nScreenHeight = screen_h;
		nPixelWidth = pixel_w;
		nPixelHeight = pixel_h;
		
#ifdef __SWITCH__
		SetDockedPixelSize(pixel_w, pixel_h);
		SetHandheldPixelSize(pixel_w, pixel_h);
#endif

		fPixelX = 2.0f / (float)(nScreenWidth);
		fPixelY = 2.0f / (float)(nScreenHeight);

		if (nPixelWidth == 0 || nPixelHeight == 0 || nScreenWidth == 0 || nScreenHeight == 0)
			return olc::FAIL;
		
		// Load the default font sheet
		olc_ConstructFontSheet();
		
		// Create a sprite that represents the primary drawing target
		pDefaultDrawTarget = new Sprite(nScreenWidth, nScreenHeight);
		SetDrawTarget(nullptr);
		return olc::OK;
	}
	
	olc::rcode PixelGameEngine::Start()
	{
		// Construct the window
		if (!olc_WindowCreate())
			return olc::FAIL;

#ifndef __SWITCH__	
#else // __SWITCH__ defined
		// you know what?
		// switch doesn't have std::thread
		// fuck std::thread
		// we don't need threads
		
		bAtomActive = true;
		EngineThread();
#endif
		
		return olc::OK;
	}
	
	void PixelGameEngine::SetDrawTarget(Sprite *target)
	{
		if (target)
			pDrawTarget = target;
		else
			pDrawTarget = pDefaultDrawTarget;
	}

	Sprite* PixelGameEngine::GetDrawTarget()
	{
		return pDrawTarget;
	}
	
	HWButton PixelGameEngine::GetKey(Key k)
	{
		return pKeyboardState[k];
	}
	
	int32_t PixelGameEngine::ScreenWidth()
	{
		return nScreenWidth;
	}

	int32_t PixelGameEngine::ScreenHeight()
	{
		return nScreenHeight;
	}
	
	void PixelGameEngine::Draw(int32_t x, int32_t y, Pixel p)
	{
		if (!pDrawTarget) return;


		if (nPixelMode == Pixel::NORMAL)
		{
			pDrawTarget->SetPixel(x, y, p);
			return;
		}

		if (nPixelMode == Pixel::MASK)
		{
			if(p.a == 255)
				pDrawTarget->SetPixel(x, y, p);
			return;
		}

		if (nPixelMode == Pixel::ALPHA)
		{
			Pixel d = pDrawTarget->GetPixel(x, y);
			float a = (float)(p.a / 255.0f) * fBlendFactor;
			float c = 1.0f - a;
			float r = a * (float)p.r + c * (float)d.r;
			float g = a * (float)p.g + c * (float)d.g;
			float b = a * (float)p.b + c * (float)d.b;
			pDrawTarget->SetPixel(x, y, Pixel((uint8_t)r, (uint8_t)g, (uint8_t)b));
			return;
		}

		if (nPixelMode == Pixel::CUSTOM)
		{
			pDrawTarget->SetPixel(x, y, funcPixelMode(x, y, p, pDrawTarget->GetPixel(x, y)));
			return;
		}
	}
	
	bool PixelGameEngine::OnUserCreate()
	{ return false; }
	bool PixelGameEngine::OnUserUpdate(float fElapsedTime)
	{ return false; }
	bool PixelGameEngine::OnUserDestroy()
	{ return true; }
	
	void PixelGameEngine::EngineThread() {
		if (!OnUserCreate())
			bAtomActive = false;
		
		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();
		
		while (bAtomActive) {
#ifdef __SWITCH__
			while (appletMainLoop() && bAtomActive)
#else
			while (bAtomActive)
#endif
			{
#ifdef __SWITCH__
				u8 newOpMode = appletGetOperationMode();
				
				if (newOpMode != operationMode) {
					framebufferClose(&olc_Framebuffer);
					if (!olc_WindowCreate())
						bAtomActive = false;
				}
#endif
				
				tp2 = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = tp2 - tp1;
				tp1 = tp2;
				
				float fElapsedTime = elapsedTime.count();
				
				// handle input
				hidScanInput();
				
				u64 kp = hidKeysDown(CONTROLLER_P1_AUTO);
				u64 kh = hidKeysHeld(CONTROLLER_P1_AUTO);
				u64 kr = hidKeysUp(CONTROLLER_P1_AUTO);
				u64 mask = 1;
				
				for (int key = Key::JC_A; key <= Key::JC_SR_RIGHT; key++) {
					pKeyboardState[key].bPressed  = (kp & mask) != 0;
					pKeyboardState[key].bHeld     = (kh & mask) != 0;
					pKeyboardState[key].bReleased = (kr & mask) != 0;
					mask <<= 1;
				}
				
				// update game
				
				if (!OnUserUpdate(fElapsedTime)) {
					bAtomActive = false;
				}
				
				// display grpahics
				
#ifdef __SWITCH__
				Pixel * pixels = pDefaultDrawTarget->GetData();
				
				u32 stride;
				u32* framebuf = (u32*)framebufferBegin(&olc_Framebuffer, &stride);
				
				for (u32 y = 0, dy = screenOffsetY; y < nScreenHeight && dy < fbHeight; y++) {
					for (u32 py = 0; py < nPixelHeight; py++, dy++) {
						for (u32 x = 0, dx = screenOffsetX; x < nScreenWidth && dx < fbWidth; x++) {
							u32 spos = y * nScreenWidth + x;
							for (u32 px = 0; px < nPixelWidth; px++, dx++) {
								u32 fpos = dy * stride / sizeof(u32) + dx;
								framebuf[fpos] = pixels[spos].n;
							}
						}
					}
				}
				
				framebufferEnd(&olc_Framebuffer);
#endif
			}
			
			if (OnUserDestroy()) {
			}
			else {
				bAtomActive = true;
			}
		}
		
		// clean up
#ifdef __SWITCH__
		framebufferClose(&olc_Framebuffer);
#endif
	}
	
	void PixelGameEngine::olc_ConstructFontSheet()
	{
		std::string data;
		data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
		data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
		data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
		data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
		data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
		data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
		data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
		data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
		data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
		data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
		data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
		data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
		data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
		data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
		data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
		data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

		fontSprite = new olc::Sprite(128, 48);
		int px = 0, py = 0;
		for (int b = 0; b < 1024; b += 4)
		{
			uint32_t sym1 = (uint32_t)data[b + 0] - 48;
			uint32_t sym2 = (uint32_t)data[b + 1] - 48;
			uint32_t sym3 = (uint32_t)data[b + 2] - 48;
			uint32_t sym4 = (uint32_t)data[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 24; i++)
			{
				int k = r & (1 << i) ? 255 : 0;
				fontSprite->SetPixel(px, py, olc::Pixel(k, k, k, k));
				if (++py == 48) { px++; py = 0; }
			}
		}
	}
	
#ifdef __SWITCH__
	NWindow* PixelGameEngine::olc_WindowCreate() {
		operationMode = appletGetOperationMode();
		
		if (operationMode == AppletOperationMode_Docked) {
			fbWidth = 1920;
			fbHeight = 1080;
			nPixelWidth = nDockedPixelWidth;
			nPixelHeight = nDockedPixelHeight;
		}
		else {
			fbWidth = 1280;
			fbHeight = 720;
			nPixelWidth = nHandheldPixelWidth;
			nPixelHeight = nHandheldPixelHeight;
		}
		
		olc_Window = nwindowGetDefault();
		
		framebufferCreate(&olc_Framebuffer, olc_Window, fbWidth, fbHeight, PIXEL_FORMAT_RGBA_8888, 2);
		framebufferMakeLinear(&olc_Framebuffer);
		
		screenOffsetX = (fbWidth - (nScreenWidth * nPixelWidth)) / 2;
		screenOffsetY = (fbHeight - (nScreenHeight * nPixelHeight)) / 2;
		
		if (screenOffsetX > fbWidth)
			screenOffsetX = 0;
		
		if (screenOffsetY > fbHeight)
			screenOffsetY = 0;
		
		return olc_Window;
	}
	
	void PixelGameEngine::SetHandheldPixelSize(uint32_t w, uint32_t h) {
		nHandheldPixelWidth = w;
		nHandheldPixelHeight = h;
	}
	
	void PixelGameEngine::SetDockedPixelSize(uint32_t w, uint32_t h) {
		nDockedPixelWidth = w;
		nDockedPixelHeight = h;
	}
#endif
	
	std::atomic<bool> PixelGameEngine::bAtomActive{ false };
	std::map<uint16_t, uint8_t> PixelGameEngine::mapKeys;
};
*/

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
