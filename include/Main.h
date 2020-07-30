#pragma once

namespace Ostaga { int Load(); }

#ifdef OSTAGA_BUILD_WINDOWS
	#ifdef OSTAGA_DIST
		int WINAPI wWinMain(
		_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE,
		_In_ PWSTR pCmdLine,
		_In_ int nCmdShow)
		{
			return Ostaga::Load();
		}
	#else
		int main(int argc, char **argv)
		{
			return Ostaga::Load();
		}
	#endif
#else
	int main(int argc, char **argv)
	{
		return Ostaga::Load();
	}
#endif
