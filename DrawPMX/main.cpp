#include "api.h"
#include "windows.h"
#include "directx11.h"

Windows window = {};
DirectX11 directX11 = {};


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// COM�n�̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// �Z�p���C�u�������g�p�ł��邩�̌���
	if (XMVerifyCPUSupport() != TRUE)
	{
		return 1;
	}

	// ������
	window.init();
	directX11.init(window);

	// ���C�����[�v
	while (window.processMessage())
	{
		directX11.display();
	}

	//�I��
	directX11.end();

	CoUninitialize();

	return 0;
}