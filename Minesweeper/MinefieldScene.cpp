#include "MinefieldScene.h"

#include <string>

#include "colors.h"
#include "constants.h"
#include "enums.h"
#include "MinefieldWindow.h"
#include "MineTile.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

MinefieldScene::MinefieldScene() {}

HRESULT MinefieldScene::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	m_pMinefield = reinterpret_cast<MinefieldWindow*>(GetWindowLongPtr(m_hOwnerWnd, GWLP_USERDATA));
	hr = (m_pMinefield ? S_OK : E_FAIL);

	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreatePathGeometry(&m_pTileEdgeGeometry);
	}

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

void MinefieldScene::DiscardDeviceIndependentResources()
{
	m_pTileEdgeGeometry.Release();

}

HRESULT MinefieldScene::CreateDeviceDependentResources()
{
	const D2D1_SIZE_F fSize{ m_pRenderTarget->GetSize() };
	const FLOAT tileWidth{ fSize.width / m_pMinefield->GetMinefieldWidth() };
	const FLOAT tileHeight{ fSize.height / m_pMinefield->GetMinefieldHeight() };

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
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileOne)), &m_pOneColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileTwo)), &m_pTwoColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileThree)), &m_pThreeColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileFour)), &m_pFourColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileFive)), &m_pFiveColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileSix)), &m_pSixColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileSeven)), &m_pSevenColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEight)), &m_pEightColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileQuestionMark)), &m_pQuestionMarkColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDWriteFactory->CreateTextFormat(constants::FONT_NUMBER.data(), nullptr, DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, min(tileWidth, tileHeight), L"en-US", &m_pTextFormat);

		if (SUCCEEDED(hr))
		{
			m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
	}

	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(hr))
		{
			hr = m_pDWriteFactory->CreateTextFormat(constants::FONT_EMOJI.data(), nullptr, DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 0.6f * min(tileWidth, tileHeight), L"en-US", &m_pEmojiFormat);

			if (SUCCEEDED(hr))
			{
				m_pEmojiFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_pEmojiFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			}
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_ONE.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pOneColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pOneBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_TWO.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pTwoColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pTwoBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_THREE.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pThreeColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pThreeBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_FOUR.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pFourColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pFourBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_FIVE.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pFiveColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pFiveBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_SIX.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pSixColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pSixBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_SEVEN.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pSevenColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pSevenBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_EIGHT.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pEightColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pEightBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::CHAR_QUESTION_MARK.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pTextFormat, m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, &m_pQuestionMarkBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::EMOJI_BOMB.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pEmojiFormat, m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT, &m_pBombBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateCharacterBitmap(constants::EMOJI_FLAG.data(), static_cast<UINT>(tileWidth + 1), static_cast<UINT>(tileHeight + 1),
			m_pEmojiFormat, m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT, &m_pFlagBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = LoadImageFromResource(IDB_X_MARK, TEXT("PNG"), 1024, 1024, &m_pXMarkBitmap);
	}

	return hr;
}

void MinefieldScene::DiscardDeviceDependentResources()
{
	m_pTileEdgeLightestColorBrush.Release();
	m_pTileEdgeLightColorBrush.Release();
	m_pTileEdgeDarkColorBrush.Release();
	m_pTileEdgeDarkestColorBrush.Release();

	m_pOneColorBrush.Release();
	m_pTwoColorBrush.Release();
	m_pThreeColorBrush.Release();
	m_pFourColorBrush.Release();
	m_pFiveColorBrush.Release();
	m_pSixColorBrush.Release();
	m_pSevenColorBrush.Release();
	m_pEightColorBrush.Release();
	m_pQuestionMarkColorBrush.Release();

	m_pTextFormat.Release();
	m_pEmojiFormat.Release();

	m_pOneBitmap.Release();
	m_pTwoBitmap.Release();
	m_pThreeBitmap.Release();
	m_pFourBitmap.Release();
	m_pFiveBitmap.Release();
	m_pSixBitmap.Release();
	m_pSevenBitmap.Release();
	m_pEightBitmap.Release();
	m_pQuestionMarkBitmap.Release();
	m_pBombBitmap.Release();
	m_pFlagBitmap.Release();
	m_pXMarkBitmap.Release();
}

void MinefieldScene::CalculateLayout()
{
	const D2D1_SIZE_F fSize{ m_pRenderTarget->GetSize() };
	const FLOAT tileWidth{ fSize.width / m_pMinefield->GetMinefieldWidth() };
	const FLOAT tileHeight{ fSize.height / m_pMinefield->GetMinefieldHeight() };

	for (UINT x{ 0 }; x < m_pMinefield->GetMinefieldWidth(); ++x)
	{
		for (UINT y{ 0 }; y < m_pMinefield->GetMinefieldHeight(); ++y)
		{
			(*m_pMinefield)(x, y).SetDrawRect(x * tileWidth, y * tileHeight, tileHeight, tileHeight);
		}
	}
}

void MinefieldScene::RenderScene()
{
	m_pRenderTarget->Clear(D2D1::ColorF(RGBA(colors::tileBackground)));

	for (UINT x{ 0 }; x < m_pMinefield->GetMinefieldWidth(); ++x)
	{
		for (UINT y{ 0 }; y < m_pMinefield->GetMinefieldHeight(); ++y)
		{
				DrawTile((*m_pMinefield)(x, y));
				DrawTileContents((*m_pMinefield)(x, y));
		}
	}
}

HRESULT MinefieldScene::CreateCharacterBitmap(const WCHAR* pChar, const UINT width, const UINT height, IDWriteTextFormat* pTextFormat, 
	ID2D1Brush* pFillBrush, D2D1_DRAW_TEXT_OPTIONS drawTextOptions, ID2D1Bitmap** destBitmap)
{
	CComPtr<ID2D1BitmapRenderTarget> pCompatibleRenderTarget{ nullptr };
	CComPtr<ID2D1Bitmap> pSourceBitmap{ nullptr };
	const D2D1_POINT_2U destPoint{ D2D1::Point2U(0,0) };
	const D2D1_RECT_U srcRect{ D2D1::RectU(0, 0, width, height) };
	const D2D1_RECT_F drawRect{ D2D1::RectF(0, 0, width, height) };
	const D2D1_SIZE_U destSize{ D2D1::SizeU(width, height) };
	FLOAT dpiX{ 0.0f };
	FLOAT dpiY{ 0.0f };

	HRESULT hr = m_pRenderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(width, height), &pCompatibleRenderTarget);

	if (SUCCEEDED(hr))
	{
		pCompatibleRenderTarget->BeginDraw();
		pCompatibleRenderTarget->Clear();
		pCompatibleRenderTarget->DrawText(pChar, static_cast<UINT>(sizeof(TCHAR)), pTextFormat, drawRect, pFillBrush, drawTextOptions);
		pCompatibleRenderTarget->EndDraw();

		hr = pCompatibleRenderTarget->GetBitmap(&pSourceBitmap);

		if (SUCCEEDED(hr))
		{
			pSourceBitmap->GetDpi(&dpiX, &dpiY);
			hr = m_pRenderTarget->CreateBitmap(destSize, D2D1::BitmapProperties(pSourceBitmap->GetPixelFormat(), dpiX, dpiY), destBitmap);

			if (SUCCEEDED(hr))
			{
				hr = (*destBitmap)->CopyFromBitmap(&destPoint, pSourceBitmap, &srcRect);
			}

			pSourceBitmap.Release();
		}

		pCompatibleRenderTarget.Release();
	}

	return hr;
}

void MinefieldScene::DrawTile(const MineTile& tile)
{
	ID2D1SolidColorBrush* pLeftEdgeColorBrush{ m_pTileEdgeLightColorBrush };
	ID2D1SolidColorBrush* pTopEdgeColorBrush{ m_pTileEdgeLightestColorBrush };
	ID2D1SolidColorBrush* pRightEdgeColorBrush{ m_pTileEdgeDarkColorBrush };
	ID2D1SolidColorBrush* pBottomEdgeColorBrush{ m_pTileEdgeDarkestColorBrush };

	switch (tile.GetTileState())
	{
	case TileState::HIDDEN:
	{
		if (m_pMinefield->IsGameLost())
		{
			if (tile.GetTileContent() == TileContent::MINE && tile.GetTileMark() != TileMark::FLAG)
			{
				pLeftEdgeColorBrush = m_pTileEdgeDarkColorBrush;
				pTopEdgeColorBrush = m_pTileEdgeDarkestColorBrush;
				pRightEdgeColorBrush = m_pTileEdgeLightColorBrush;
				pBottomEdgeColorBrush = m_pTileEdgeLightestColorBrush;
			}
			else if (tile.GetTileContent() != TileContent::MINE && tile.GetTileMark() == TileMark::FLAG)
			{
				pLeftEdgeColorBrush = m_pTileEdgeDarkColorBrush;
				pTopEdgeColorBrush = m_pTileEdgeDarkestColorBrush;
				pRightEdgeColorBrush = m_pTileEdgeLightColorBrush;
				pBottomEdgeColorBrush = m_pTileEdgeLightestColorBrush;
			}
		}
		else if (m_pMinefield->IsGameWon())
		{
			if (tile.GetTileContent() == TileContent::MINE)
			{
				CComPtr<ID2D1SolidColorBrush> pFill{ nullptr };
				HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileBackgroundMineGameWin)), &pFill);
				if (SUCCEEDED(hr))
				{
					m_pRenderTarget->FillRectangle(tile.GetDrawRect(), pFill);
				}
			}
		}
	}
	break;

	case TileState::CLICKED:
	{
		pLeftEdgeColorBrush = m_pTileEdgeDarkColorBrush;
		pTopEdgeColorBrush = m_pTileEdgeDarkestColorBrush;
		pRightEdgeColorBrush = m_pTileEdgeLightColorBrush;
		pBottomEdgeColorBrush = m_pTileEdgeLightestColorBrush;
	}
	break;

	case TileState::REVEALED:
	{
		pLeftEdgeColorBrush = m_pTileEdgeDarkColorBrush;
		pTopEdgeColorBrush = m_pTileEdgeDarkestColorBrush;
		pRightEdgeColorBrush = m_pTileEdgeLightColorBrush;
		pBottomEdgeColorBrush = m_pTileEdgeLightestColorBrush;

		if (tile.GetTileContent() == TileContent::MINE)
		{
			CComPtr<ID2D1SolidColorBrush> pFill{ nullptr };
			HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileBackgroundMineReveal)), &pFill);
			if (SUCCEEDED(hr))
			{
				m_pRenderTarget->FillRectangle(tile.GetDrawRect(), pFill);
			}
		}
	}
	break;
	}

	const D2D1_RECT_F drawRect{ tile.GetDrawRect() };
	const float tileWidth{ drawRect.right - drawRect.left };
	const float tileHeight{ drawRect.bottom - drawRect.top };
	const D2D1_MATRIX_3X2_F scaleMatrix{ D2D1::Matrix3x2F::Scale(tileWidth, tileHeight) };

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
}

void MinefieldScene::DrawTileContents(const MineTile& tile)
{
	switch (tile.GetTileState())
	{
	case TileState::HIDDEN:
		if (!(m_pMinefield->IsGameLost()))
		{
			switch (tile.GetTileMark())
			{
			case TileMark::NONE:
				if (m_pMinefield->IsGameWon() && tile.GetTileContent() == TileContent::MINE)
				{
					m_pRenderTarget->DrawBitmap(m_pFlagBitmap, tile.GetDrawRect());
				}
				break;
			case::TileMark::FLAG:
				m_pRenderTarget->DrawBitmap(m_pFlagBitmap, tile.GetDrawRect());
				break;
			case TileMark::QUESTION_MARK:
				m_pRenderTarget->DrawBitmap(m_pQuestionMarkBitmap, tile.GetDrawRect());
				break;
			}
		}
		else if (tile.GetTileContent() == TileContent::MINE)
		{
			switch (tile.GetTileMark())
			{
			case TileMark::NONE:
				m_pRenderTarget->DrawBitmap(m_pBombBitmap, tile.GetDrawRect());
				break;
			case::TileMark::FLAG:
				m_pRenderTarget->DrawBitmap(m_pFlagBitmap, tile.GetDrawRect());
				break;
			case TileMark::QUESTION_MARK:
				m_pRenderTarget->DrawBitmap(m_pBombBitmap, tile.GetDrawRect());
				break;
			}
		}
		else
		{
			switch (tile.GetTileMark())
			{
			case TileMark::NONE:
				break;
			case::TileMark::FLAG:
				m_pRenderTarget->DrawBitmap(m_pBombBitmap, tile.GetDrawRect());
				m_pRenderTarget->DrawBitmap(m_pXMarkBitmap, tile.GetDrawRect());
				break;
			case TileMark::QUESTION_MARK:
				break;
			}
		}
		break;
	case TileState::CLICKED:
		break;
	case TileState::REVEALED:
		switch (tile.GetTileContent())
		{
		case TileContent::ONE:
			m_pRenderTarget->DrawBitmap(m_pOneBitmap, tile.GetDrawRect());
			break;
		case TileContent::TWO:
			m_pRenderTarget->DrawBitmap(m_pTwoBitmap, tile.GetDrawRect());
			break;
		case TileContent::THREE:
			m_pRenderTarget->DrawBitmap(m_pThreeBitmap, tile.GetDrawRect());
			break;
		case TileContent::FOUR:
			m_pRenderTarget->DrawBitmap(m_pFourBitmap, tile.GetDrawRect());
			break;
		case TileContent::FIVE:
			m_pRenderTarget->DrawBitmap(m_pFiveBitmap, tile.GetDrawRect());
			break;
		case TileContent::SIX:
			m_pRenderTarget->DrawBitmap(m_pSixBitmap, tile.GetDrawRect());
			break;
		case TileContent::SEVEN:
			m_pRenderTarget->DrawBitmap(m_pSevenBitmap, tile.GetDrawRect());
			break;
		case TileContent::EIGHT:
			m_pRenderTarget->DrawBitmap(m_pEightBitmap, tile.GetDrawRect());
			break;
		case TileContent::MINE:
			m_pRenderTarget->DrawBitmap(m_pBombBitmap, tile.GetDrawRect());
			break;
		}
		break;
	}
}