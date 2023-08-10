#include "Main.h"

int main(void)
{
	using namespace Simplex;
	Application* pApp = new Application();
	pApp->Init("", RES_C_1920x1080_16x9_FULLHD, false, false);
	pApp->Run();
	SafeDelete(pApp);
	return 0;
}