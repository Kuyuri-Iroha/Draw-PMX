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
	bool res = model.init(L"resources\\�����~�Nver.2.1\\�����~�Nver.2.1.pmx", directX11.getDevice());

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
			XMMatrixLookAtLH({ 0.0f, 10.0f, -25.0f }, { 0.0f, 10.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })
		);
		tmcb.projection = XMMatrixTranspose(
			XMMatrixPerspectiveFovLH(50.0f * (XM_PI / 180.0f), static_cast<float>(Windows::WINDOW_WIDTH) / static_cast<float>(Windows::WINDOW_HEIGHT), 1.0f, 1000.0f)
		);

		// ���f���̕`��
		for (unsigned i = 0; i < model.getMeshesSize(); i++)
		{
			if (model.meshHasTexture(i))
			{
				model.drawMesh(directX11, i, reinterpret_cast<void*>(&tmcb));
			}
			else
			{
				// �P�F�}�e���A���̒萔�o�b�t�@�̍X�V
				NotTexturedModelConstantBufferData ntmcb{};
				ntmcb.world = tmcb.world;
				ntmcb.view = tmcb.view;
				ntmcb.projection = tmcb.projection;
				ntmcb.diffuseColor = XMVECTOR{model.meshes[i].diffuseColor.x, model.meshes[i].diffuseColor.y, model.meshes[i].diffuseColor.z, model.meshes[i].diffuseColor.w};
				ntmcb.ambientColor = XMVECTOR{model.meshes[i].ambientColor.x, model.meshes[i].ambientColor.y, model.meshes[i].ambientColor.z};

				model.drawMesh(directX11, i, reinterpret_cast<void*>(&ntmcb));
			}
		}

		directX11.display();
	}

	//�I��
	model.end();
	directX11.end();

	CoUninitialize();

	return 0;
}