#pragma once


template<class COMPointer>
void SafeRelease(COMPointer& _p)
{
	if (_p)
	{
		_p->Release();
		_p = nullptr;
	}
}

// �덷�l
constexpr float EPSILON = 1e-8f;