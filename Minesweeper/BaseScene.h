#pragma once
#include <cassert>

#include <atlbase.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <Windows.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

class BaseScene
{
protected:
	// Handle to window owning the scene
	HWND m_hOwnerWnd{ nullptr };

	// D2D Resources
	CComPtr<ID2D1Factory>           m_pFactory;
	CComPtr<ID2D1HwndRenderTarget>  m_pRenderTarget;
	CComPtr<IWICImagingFactory>     m_pWICFactory;
	CComPtr<IDWriteFactory>			m_pDWriteFactory;

protected:

	// Derived class must implement these methods.
	virtual HRESULT CreateDeviceIndependentResources() = 0;
	virtual void    DiscardDeviceIndependentResources() = 0;
	virtual HRESULT CreateDeviceDependentResources() = 0;
	virtual void    DiscardDeviceDependentResources() = 0;
	virtual void    CalculateLayout() = 0;
	virtual void    RenderScene() = 0;

protected:

	HRESULT CreateGraphicsResources()
	{
		HRESULT hr = S_OK;
		if (m_pRenderTarget == nullptr)
		{
			RECT rc;
			GetClientRect(m_hOwnerWnd, &rc);

			D2D1_SIZE_U size{ D2D1::SizeU(rc.right, rc.bottom) };

			hr = m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hOwnerWnd, size), &m_pRenderTarget);

			if (SUCCEEDED(hr))
			{
				hr = CreateDeviceDependentResources();
			}
			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
		return hr;
	}

	HRESULT LoadImageFromFile(LPCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap,
		WICBitmapInterpolationMode scalingMode = WICBitmapInterpolationModeLinear)
	{
		CComPtr<IWICBitmapDecoder> pDecoder{ nullptr };

		HRESULT hr = m_pWICFactory->CreateDecoderFromFilename(uri, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad,
			&pDecoder);

		if (SUCCEEDED(hr))
		{
			hr = LoadImageFromDecoder(pDecoder, destinationWidth, destinationHeight, ppBitmap, scalingMode);
		}

		return hr;
	}

	HRESULT LoadImageFromResource(UINT resourceID, LPCTSTR typeID, UINT destinationWidth, UINT destinationHeight,
		ID2D1Bitmap** ppBitmap, WICBitmapInterpolationMode scalingMode = WICBitmapInterpolationModeLinear)
	{
		CComPtr<IWICStream> pIWICStream{ nullptr };
		CComPtr<IWICBitmapDecoder> pDecoder{ nullptr };

		HRSRC hImageRes{ nullptr };
		HGLOBAL hImageResData{ nullptr };
		void* pImageFile{ nullptr };
		DWORD imageFileSize = 0;

		hImageRes = FindResource(nullptr, MAKEINTRESOURCE(resourceID), typeID);
		HRESULT hr = (hImageRes ? S_OK : E_FAIL);

		if (SUCCEEDED(hr))
		{
			hImageResData = LoadResource(nullptr, hImageRes);
			hr = (hImageResData ? S_OK : E_FAIL);
		}

		if (SUCCEEDED(hr))
		{
			pImageFile = LockResource(hImageResData);
			hr = (pImageFile ? S_OK : E_FAIL);
		}

		if (SUCCEEDED(hr)) {
			imageFileSize = SizeofResource(nullptr, hImageRes);
			hr = (imageFileSize ? S_OK : E_FAIL);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateStream(&pIWICStream);
		}

		if (SUCCEEDED(hr))
		{
			hr = pIWICStream->InitializeFromMemory(reinterpret_cast<BYTE*>(pImageFile), imageFileSize);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateDecoderFromStream(pIWICStream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadImageFromDecoder(pDecoder, destinationWidth, destinationHeight, ppBitmap, scalingMode);
		}

		return hr;
	}

private:
	HRESULT LoadImageFromDecoder(IWICBitmapDecoder* pDecoder, UINT destinationWidth, UINT destinationHeight,
		ID2D1Bitmap** ppBitmap, WICBitmapInterpolationMode scalingMode)
	{
		CComPtr<IWICBitmapFrameDecode> pSource{ nullptr };
		CComPtr<IWICStream> pStream{ nullptr };
		CComPtr<IWICFormatConverter> pConverter{ nullptr };
		CComPtr<IWICBitmapScaler> pScaler{ nullptr };

		HRESULT	hr = pDecoder->GetFrame(0, &pSource);

		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateFormatConverter(&pConverter);
		}

		if (SUCCEEDED(hr))
		{
			hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f,
				WICBitmapPaletteTypeMedianCut);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory->CreateBitmapScaler(&pScaler);
		}

		if (SUCCEEDED(hr))
		{
			hr = pScaler->Initialize(pConverter, destinationWidth, destinationHeight, WICBitmapInterpolationModeLinear);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pScaler, nullptr, ppBitmap);
		}

		return hr;
	}

public:
	BaseScene() {}
	virtual ~BaseScene() {}

	HRESULT Initialize(HWND hWnd)
	{
		m_hOwnerWnd = hWnd;

		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);

		if (SUCCEEDED(hr))
		{
			hr = m_pWICFactory.CoCreateInstance(CLSID_WICImagingFactory);
		}

		if (SUCCEEDED(hr))
		{
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory),
				reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
		}

		if (SUCCEEDED(hr))
		{
			CreateDeviceIndependentResources();
		}
		return hr;
	}

	void Render()
	{
		HRESULT hr = CreateGraphicsResources();
		if (FAILED(hr))
		{
			return;
		}

		assert(m_pRenderTarget != nullptr);

		m_pRenderTarget->BeginDraw();

		RenderScene();

		hr = m_pRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			DiscardDeviceDependentResources();
			m_pRenderTarget.Release();
		}
	}

	HRESULT Resize(int x, int y)
	{
		HRESULT hr = S_OK;
		if (m_pRenderTarget)
		{
			D2D1_SIZE_U size{ D2D1::SizeU(x, y) };
			hr = m_pRenderTarget->Resize(size);
			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
		return hr;
	}

	void CleanUp()
	{
		DiscardDeviceDependentResources();
		DiscardDeviceIndependentResources();
	}
};