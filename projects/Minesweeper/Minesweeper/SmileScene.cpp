#include "SmileScene.h"

#include "colors.h"
#include "constants.h"
#include "enums.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

SmileScene::SmileScene() {}

HRESULT SmileScene::CreateDeviceIndependentResources()
{
	HRESULT hr = m_pFactory->CreatePathGeometry(&m_pTileEdgeGeometry);

	if (SUCCEEDED(hr))
	{
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };

		hr = m_pTileEdgeGeometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(D2D1::Point2F(0, 0), D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(D2D1::Point2F(1. / 16., 1. / 16.));
			pSink->AddLine(D2D1::Point2F(1. / 16., 15. / 16.));
			pSink->AddLine(D2D1::Point2F(0, 1.));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			hr = pSink->Close();
		}
	}

	return hr;
};

void SmileScene::DiscardDeviceIndependentResources()
{
	m_pTileEdgeGeometry.Release();
	m_pEmojiFormat.Release();
}

HRESULT SmileScene::CreateDeviceDependentResources()
{
	HRESULT	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEdgeLightest)), &m_pTileEdgeLightestColorBrush);

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEdgeLight)), &m_pTileEdgeLightColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEdgeDark)), &m_pTileEdgeDarkColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEdgeDarkest)), &m_pTileEdgeDarkestColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileBackground)), &m_pTextColorBrush);
	}

	return hr;
}

void SmileScene::DiscardDeviceDependentResources()
{
	m_pTileEdgeLightestColorBrush.Release();
	m_pTileEdgeLightColorBrush.Release();
	m_pTileEdgeDarkColorBrush.Release();
	m_pTileEdgeDarkestColorBrush.Release();
	m_pTextColorBrush.Release();
}

void SmileScene::CalculateLayout()
{
	const D2D1_SIZE_F fSize{ m_pRenderTarget->GetSize() };

	m_pEmojiFormat.Release();
	HRESULT hr = m_pDWriteFactory->CreateTextFormat(L"Segoe UI Emoji", nullptr,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		0.6f * min(fSize.width, fSize.height),
		L"en-US", &m_pEmojiFormat);

	if (SUCCEEDED(hr))
	{
		m_pEmojiFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pEmojiFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

void SmileScene::RenderScene()
{
	m_pRenderTarget->Clear(D2D1::ColorF(RGBA(colors::tileBackground)));

	ID2D1SolidColorBrush* pLeftEdgeColorBrush{ m_pTileEdgeLightColorBrush };
	ID2D1SolidColorBrush* pTopEdgeColorBrush{ m_pTileEdgeLightestColorBrush };
	ID2D1SolidColorBrush* pRightEdgeColorBrush{ m_pTileEdgeDarkColorBrush };
	ID2D1SolidColorBrush* pBottomEdgeColorBrush{ m_pTileEdgeDarkestColorBrush };

	if (m_bClicked)
	{
		pLeftEdgeColorBrush = m_pTileEdgeDarkColorBrush;
		pTopEdgeColorBrush = m_pTileEdgeDarkestColorBrush;
		pRightEdgeColorBrush = m_pTileEdgeLightColorBrush;
		pBottomEdgeColorBrush = m_pTileEdgeLightestColorBrush;
	}

	const D2D1_SIZE_F fSize{ m_pRenderTarget->GetSize() };
	const D2D1_RECT_F drawRect{ D2D1::RectF(0, 0, fSize.width, fSize.height) };
	const D2D1_MATRIX_3X2_F scaleMatrix{ D2D1::Matrix3x2F::Scale(fSize.width, fSize.height) };

	D2D1_MATRIX_3X2_F translationMatrix{ D2D1::Matrix3x2F::Translation(drawRect.left, drawRect.top) };
	m_pRenderTarget->SetTransform(scaleMatrix * translationMatrix);
	m_pRenderTarget->FillGeometry(m_pTileEdgeGeometry, pLeftEdgeColorBrush);

	translationMatrix = D2D1::Matrix3x2F::Translation(drawRect.right, drawRect.top);
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(90.0f) * scaleMatrix * translationMatrix);
	m_pRenderTarget->FillGeometry(m_pTileEdgeGeometry, pTopEdgeColorBrush);

	translationMatrix = D2D1::Matrix3x2F::Translation(drawRect.right, drawRect.bottom);
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(180.0f) * scaleMatrix * translationMatrix);
	m_pRenderTarget->FillGeometry(m_pTileEdgeGeometry, pRightEdgeColorBrush);

	translationMatrix = D2D1::Matrix3x2F::Translation(drawRect.left, drawRect.bottom);
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(270.0f) * scaleMatrix * translationMatrix);
	m_pRenderTarget->FillGeometry(m_pTileEdgeGeometry, pBottomEdgeColorBrush);

	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	if (m_pEmojiFormat)
	{
		switch (m_smileState)
		{
		case SmileState::SMILE:
			m_pRenderTarget->DrawText(constants::SMILE_CHAR.data(), static_cast<UINT>(constants::SMILE_CHAR.size()),
				m_pEmojiFormat, drawRect, m_pTextColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
			break;
		case SmileState::SMILE_OPEN_MOUTH:
			m_pRenderTarget->DrawText(constants::SMILE_OPEN_MOUTH_CHAR.data(), static_cast<UINT>(constants::SMILE_OPEN_MOUTH_CHAR.size()),
				m_pEmojiFormat, drawRect, m_pTextColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
			break;
		case SmileState::SMILE_SUNGLASSES:
			m_pRenderTarget->DrawText(constants::SMILE_SUNGLASSES_CHAR.data(), static_cast<UINT>(constants::SMILE_SUNGLASSES_CHAR.size()),
				m_pEmojiFormat, drawRect, m_pTextColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
			break;

		case SmileState::SMILE_DEAD:
			m_pRenderTarget->DrawText(constants::SMILE_DEAD_CHAR.data(), static_cast<UINT>(constants::SMILE_DEAD_CHAR.size()),
				m_pEmojiFormat, drawRect, m_pTextColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
			break;
		}
	}
}

BOOL SmileScene::IsClicked()
{
	return m_bClicked;
}

void SmileScene::SetClicked()
{
	m_bClicked = TRUE;
}

void SmileScene::UnsetClicked()
{
	m_bClicked = FALSE;
}

void SmileScene::SetSmileState(SmileState state)
{
	m_smileState = state;
}