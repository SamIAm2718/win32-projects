#include "CounterScene.h"

#include <string>

#include "colors.h"
#include "constants.h"

/*
*	==========================
*	===== Helper Methods =====
*	==========================
*/

inline D2D1_POINT_2F NormalizePoint(float x, float y, float scale)
{
	return D2D1::Point2F(x / scale, y / scale);
}

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

CounterScene::CounterScene() {}

/*
*	Here we initialize the array of digit 'LEDs' to the
*	appropriate geometry object. The array is formed so
*	to match the labelled 'LED' diagram below.
*						 ---0---
*						|		|
*						1		2
*						|		|
*						 ---3---
*						|		|
*						4		5
*						|		|
*						 ---6---
*/
HRESULT CounterScene::CreateDeviceIndependentResources()
{
	HRESULT hr = m_pFactory->CreatePathGeometry(&m_aDigitLEDs[0]);

	if (SUCCEEDED(hr))
	{
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };

		hr = m_aDigitLEDs[0]->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(NormalizePoint(3, 4, 32), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(NormalizePoint(4, 3, 32));
			pSink->AddLine(NormalizePoint(12, 3, 32));
			pSink->AddLine(NormalizePoint(13, 4, 32));
			pSink->AddLine(NormalizePoint(12, 5, 32));
			pSink->AddLine(NormalizePoint(4, 5, 32));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreatePathGeometry(&m_aDigitLEDs[1]);
	}

	if (SUCCEEDED(hr))
	{
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };

		hr = m_aDigitLEDs[1]->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(NormalizePoint(2, 5, 32), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(NormalizePoint(3, 6, 32));
			pSink->AddLine(NormalizePoint(3, 14, 32));
			pSink->AddLine(NormalizePoint(2, 15, 32));
			pSink->AddLine(NormalizePoint(1, 14, 32));
			pSink->AddLine(NormalizePoint(1, 6, 32));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreatePathGeometry(&m_aDigitLEDs[2]);
	}

	if (SUCCEEDED(hr))
	{
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };

		hr = m_aDigitLEDs[2]->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(NormalizePoint(14, 5, 32), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(NormalizePoint(15, 6, 32));
			pSink->AddLine(NormalizePoint(15, 14, 32));
			pSink->AddLine(NormalizePoint(14, 15, 32));
			pSink->AddLine(NormalizePoint(13, 14, 32));
			pSink->AddLine(NormalizePoint(13, 6, 32));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreatePathGeometry(&m_aDigitLEDs[3]);
	}

	if (SUCCEEDED(hr))
	{
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };

		hr = m_aDigitLEDs[3]->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(NormalizePoint(3, 16, 32), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(NormalizePoint(4, 15, 32));
			pSink->AddLine(NormalizePoint(12, 15, 32));
			pSink->AddLine(NormalizePoint(13, 16, 32));
			pSink->AddLine(NormalizePoint(12, 17, 32));
			pSink->AddLine(NormalizePoint(4, 17, 32));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreatePathGeometry(&m_aDigitLEDs[4]);
	}

	if (SUCCEEDED(hr))
	{
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };

		hr = m_aDigitLEDs[4]->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(NormalizePoint(2, 17, 32), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(NormalizePoint(3, 18, 32));
			pSink->AddLine(NormalizePoint(3, 26, 32));
			pSink->AddLine(NormalizePoint(2, 27, 32));
			pSink->AddLine(NormalizePoint(1, 26, 32));
			pSink->AddLine(NormalizePoint(1, 18, 32));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreatePathGeometry(&m_aDigitLEDs[5]);
	}

	if (SUCCEEDED(hr))
	{
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };

		hr = m_aDigitLEDs[5]->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(NormalizePoint(14, 17, 32), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(NormalizePoint(15, 18, 32));
			pSink->AddLine(NormalizePoint(15, 26, 32));
			pSink->AddLine(NormalizePoint(14, 27, 32));
			pSink->AddLine(NormalizePoint(13, 26, 32));
			pSink->AddLine(NormalizePoint(13, 18, 32));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreatePathGeometry(&m_aDigitLEDs[6]);
	}

	if (SUCCEEDED(hr))
	{
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };

		hr = m_aDigitLEDs[6]->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(NormalizePoint(3, 28, 32), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(NormalizePoint(4, 27, 32));
			pSink->AddLine(NormalizePoint(12, 27, 32));
			pSink->AddLine(NormalizePoint(13, 28, 32));
			pSink->AddLine(NormalizePoint(12, 29, 32));
			pSink->AddLine(NormalizePoint(4, 29, 32));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}

	return hr;
}

void CounterScene::DiscardDeviceIndependentResources()
{
	for (auto& digitLED : m_aDigitLEDs)
	{
		digitLED.Release();
	}
}

HRESULT CounterScene::CreateDeviceDependentResources()
{
	HRESULT	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::digitLEDOff)), &m_pDigitLEDOffColorBrush);

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::digitLEDOn)), &m_pDigitLEDOnColorBrush);
	}

	return hr;
}

void CounterScene::DiscardDeviceDependentResources()
{
	m_pDigitLEDOffColorBrush.Release();
	m_pDigitLEDOnColorBrush.Release();
}

void CounterScene::CalculateLayout() {}

void CounterScene::RenderScene()
{
	m_pRenderTarget->Clear(D2D1::ColorF(RGBA(colors::counterBackground)));
	const D2D1_SIZE_F fSize{ m_pRenderTarget->GetSize() };

	const D2D1_MATRIX_3X2_F scaleMatrix{ D2D1::Matrix3x2F::Scale(fSize.height, fSize.height) };
	const D2D1_MATRIX_3X2_F translationMatrix{ D2D1::Matrix3x2F::Translation(fSize.height / 2, 0) };
	D2D1_MATRIX_3X2_F currentTransformMatrix = scaleMatrix;

	for (int digit{ 0 }; digit < constants::COUNTER_SIZE; ++digit)
	{
		m_pRenderTarget->SetTransform(currentTransformMatrix);

		for (int index{ 0 }; index < m_aDigitLEDs.size(); ++index)
		{
			if (m_aDigitStates[digit] & 1 << index)
			{
				m_pRenderTarget->FillGeometry(m_aDigitLEDs[index], m_pDigitLEDOnColorBrush);
			}
			else
			{
				m_pRenderTarget->FillGeometry(m_aDigitLEDs[index], m_pDigitLEDOffColorBrush);
			}
		}

		currentTransformMatrix = currentTransformMatrix * translationMatrix;
	}

	m_pRenderTarget->SetTransform(D2D1::IdentityMatrix());
}

void CounterScene::SetCounter(INT32 count)
{
	// Convert int to string for easier processing.
	std::string countString{ std::to_string(count) };

	/*
	*	Initialize all digits to zero. Note that a 'digit' in
	*	the array m_aDigitStates is a bit string 0b6543210
	*	where the bit 'x' being on implies digit LED 'x' is on
	*/
	m_aDigitStates.fill(constants::DIGIT_STATES[0]);

	/*
	*	We check for a negative sign at position 0 in the
	*	string. If one exists we set the first digit to '-'
	*	and set the starting index to process the actual
	*	number to 1 instead of the default value of 0
	*/
	int startingIndex{ 0 };
	if (countString[0] == '-')
	{
		m_aDigitStates[0] = constants::DIGIT_STATES[10];
		startingIndex = 1;
	}

	/*
	*	If the length of the string is greater than the number
	*	of displayable digits, then we must set all the
	*	remaining digits to be 9. Otherwise we set the digit
	*	at  sizeDifference + i to the digit at i in our	number
	*/
	if (countString.size() > m_aDigitStates.size())
	{
		for (int index{ startingIndex }; index < m_aDigitStates.size(); ++index)
		{
			m_aDigitStates[index] = constants::DIGIT_STATES[9];
		}
	}
	else
	{
		size_t sizeDifference = m_aDigitStates.size() - countString.size();

		for (int index{ startingIndex }; index < countString.size(); ++index)
		{
			m_aDigitStates[sizeDifference + index] = constants::DIGIT_STATES[countString[index] - '0'];
		}
	}
}