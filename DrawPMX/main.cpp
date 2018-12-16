#include "api.h"
#include "Windows.h"
#include "Directx11.h"
#include "Model.h"
#include "EdgeEffector.h"

Windows window{};
DirectX11 directX11{};
Model model{};
EdgeEffector postEffector{};


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
	model.init(L"resources\\�����~�N2.1Cstyle\\�����~�Nver.2.1Cstyle.pmx", directX11.getDevice());
	postEffector.init(directX11.getDevice());

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
		XMVECTOR eyePos{ 0.0f, 18.0f, -7.0f };
		XMVECTOR focusPos{ 0.0f, 17.5f, 0.0f };
		TexturedModelConstantBufferData tmcb{};
		tmcb.world = XMMatrixIdentity();
		tmcb.view = XMMatrixTranspose(
			XMMatrixLookAtLH(eyePos, focusPos, { 0.0f, 1.0f, 0.0f })
		);
		tmcb.projection = XMMatrixTranspose(
			XMMatrixPerspectiveFovLH(50.0f * (XM_PI / 180.0f), static_cast<float>(Windows::WINDOW_WIDTH) / static_cast<float>(Windows::WINDOW_HEIGHT), 1.0f, 1000.0f)
		);
		tmcb.lightDir = XMVector3Normalize({-10.0f, 60.0f, -70.0f});
		tmcb.eyeDir = XMVector3Normalize(XMVectorSubtract(eyePos, focusPos));

		// ���f���̕`��
		for (unsigned i = 0; i < model.getMeshesSize(); i++)
		{
			if (model.meshHasTexture(i))
			{
				tmcb.specular = XMVECTOR{model.meshes[i].specularColor.x, model.meshes[i].specularColor.y, model.meshes[i].specularColor.z, model.meshes[i].specularity};
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

		// �|�X�g�G�t�F�N�g
		EdgeConstantBuffer ecb{};
		ecb.resolution = XMVECTOR{Windows::WINDOW_WIDTH, Windows::WINDOW_HEIGHT};
		postEffector.draw(directX11, reinterpret_cast<void*>(&ecb));

		directX11.display();
	}

	//�I��
	model.end();
	directX11.end();

	CoUninitialize();

	return 0;
}