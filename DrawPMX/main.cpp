#include "api.h"
#include "Windows.h"
#include "Directx11.h"
#include "Model.h"

Windows window{};
DirectX11 directX11{};
Model model{};


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
	HRESULT hr = directX11.init(window);
	bool res = model.init(L"resources\\m_GUMI_V3_201306\\GUMI1.pmx", directX11.getDevice());

	// ���C�����[�v
	while (window.processMessage())
	{
		// �r���[�|�[�g�̃Z�b�g
		D3D11_VIEWPORT viewport{};
		viewport.Width = static_cast<float>(Windows::WINDOW_WIDTH);
		viewport.Height = static_cast<float>(Windows::WINDOW_HEIGHT);
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		directX11.getContext()->RSSetViewports(1, &viewport);

		// �萔�o�b�t�@�̍X�V
		TexturedModelConstantBufferData tmcb{};
		tmcb.world = XMMatrixIdentity();
		tmcb.view = XMMatrixTranspose(
			XMMatrixLookAtLH({ 0.0f, 10.0f, -30.0f }, { 0.0f, 10.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })
		);
		tmcb.projection = XMMatrixTranspose(
			XMMatrixPerspectiveFovLH(50.0f * (XM_PI / 180.0f), static_cast<float>(Windows::WINDOW_WIDTH) / static_cast<float>(Windows::WINDOW_HEIGHT), 1.0f, 1000.0f)
		);

		// ���f���̕`��
		for (unsigned i = 0; i < model.meshes.size(); i++)
		{
			model.drawMesh(directX11, i, reinterpret_cast<void*>(&tmcb));
		}

		directX11.display();
	}

	//�I��
	model.end();
	directX11.end();

	CoUninitialize();

	return 0;
}