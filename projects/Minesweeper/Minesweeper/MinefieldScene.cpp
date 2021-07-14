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
	m_pTextFormat.Release();
	m_pEmojiFormat.Release();
}

HRESULT MinefieldScene::CreateDeviceDependentResources()
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
		hr = LoadImageFromResource(IDB_X_MARK, TEXT("PNG"), 1024, 1024, &m_pXMarkBitmap);
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

	return hr;
}

void MinefieldScene::DiscardDeviceDependentResources()
{
	m_pTileEdgeLightestColorBrush.Release();
	m_pTileEdgeLightColorBrush.Release();
	m_pTileEdgeDarkColorBrush.Release();
	m_pTileEdgeDarkestColorBrush.Release();
	m_pXMarkBitmap.Release();
	m_pOneColorBrush.Release();
	m_pTwoColorBrush.Release();
	m_pThreeColorBrush.Release();
	m_pFourColorBrush.Release();
	m_pFiveColorBrush.Release();
	m_pSixColorBrush.Release();
	m_pSevenColorBrush.Release();
	m_pEightColorBrush.Release();
	m_pQuestionMarkColorBrush.Release();
}

void MinefieldScene::CalculateLayout()
{
	const D2D1_SIZE_F fSize{ m_pRenderTarget->GetSize() };
	const float tileWidth{ fSize.width / m_pMinefield->GetMinefieldWidth() };
	const float tileHeight{ fSize.height / m_pMinefield->GetMinefieldHeight() };

	for (UINT x{ 0 }; x < m_pMinefield->GetMinefieldWidth(); ++x)
	{
		for (UINT y{ 0 }; y < m_pMinefield->GetMinefieldHeight(); ++y)
		{
			(*m_pMinefield)(x, y).SetDrawRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
		}
	}

	m_pTextFormat.Release();
	HRESULT hr = m_pDWriteFactory->CreateTextFormat(L"Cambria Math", nullptr,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		min(fSize.width / m_pMinefield->GetMinefieldWidth(), fSize.height / m_pMinefield->GetMinefieldHeight()),
		L"en-US", &m_pTextFormat);

	if (SUCCEEDED(hr))
	{
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	m_pEmojiFormat.Release();
	hr = m_pDWriteFactory->CreateTextFormat(L"Segoe UI Emoji", nullptr,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		0.6f * min(fSize.width / m_pMinefield->GetMinefieldWidth(), fSize.height / m_pMinefield->GetMinefieldHeight()),
		L"en-US", &m_pEmojiFormat);

	if (SUCCEEDED(hr))
	{
		m_pEmojiFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pEmojiFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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
			
			if (m_pTextFormat && m_pEmojiFormat)
			{
				DrawTileContents((*m_pMinefield)(x, y));
			}
		}
	}
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
					m_pRenderTarget->DrawText(constants::FLAG_CHAR.data(), static_cast<UINT>(constants::FLAG_CHAR.size()),
						m_pEmojiFormat, tile.GetDrawRect(), m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
				}
				break;
			case::TileMark::FLAG:
				m_pRenderTarget->DrawText(constants::FLAG_CHAR.data(), static_cast<UINT>(constants::FLAG_CHAR.size()),
					m_pEmojiFormat, tile.GetDrawRect(), m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
				break;
			case TileMark::QUESTION_MARK:
				m_pRenderTarget->DrawText(constants::QUESTION_MARK_CHAR.data(), static_cast<UINT>(constants::QUESTION_MARK_CHAR.size()),
					m_pTextFormat, tile.GetDrawRect(), m_pQuestionMarkColorBrush);
				break;
			}
		}
		else if (tile.GetTileContent() == TileContent::MINE)
		{
			switch (tile.GetTileMark())
			{
			case TileMark::NONE:
				m_pRenderTarget->DrawText(constants::BOMB_CHAR.data(), static_cast<UINT>(constants::BOMB_CHAR.size()),
					m_pEmojiFormat, tile.GetDrawRect(), m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
				break;
			case::TileMark::FLAG:
				m_pRenderTarget->DrawText(constants::FLAG_CHAR.data(), static_cast<UINT>(constants::FLAG_CHAR.size()),
					m_pEmojiFormat, tile.GetDrawRect(), m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
				break;
			case TileMark::QUESTION_MARK:
				m_pRenderTarget->DrawText(constants::BOMB_CHAR.data(), static_cast<UINT>(constants::BOMB_CHAR.size()),
					m_pEmojiFormat, tile.GetDrawRect(), m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
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
				m_pRenderTarget->DrawText(constants::BOMB_CHAR.data(), static_cast<UINT>(constants::BOMB_CHAR.size()),
					m_pEmojiFormat, tile.GetDrawRect(), m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
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
			m_pRenderTarget->DrawText(constants::ONE_CHAR.data(), static_cast<UINT>(constants::ONE_CHAR.size()),
				m_pTextFormat, tile.GetDrawRect(), m_pOneColorBrush);
			break;
		case TileContent::TWO:
			m_pRenderTarget->DrawText(constants::TWO_CHAR.data(), static_cast<UINT>(constants::TWO_CHAR.size()),
				m_pTextFormat, tile.GetDrawRect(), m_pTwoColorBrush);
			break;
		case TileContent::THREE:
			m_pRenderTarget->DrawText(constants::THREE_CHAR.data(), static_cast<UINT>(constants::THREE_CHAR.size()),
				m_pTextFormat, tile.GetDrawRect(), m_pThreeColorBrush);
			break;
		case TileContent::FOUR:
			m_pRenderTarget->DrawText(constants::FOUR_CHAR.data(), static_cast<UINT>(constants::FOUR_CHAR.size()),
				m_pTextFormat, tile.GetDrawRect(), m_pFourColorBrush);
			break;
		case TileContent::FIVE:
			m_pRenderTarget->DrawText(constants::FIVE_CHAR.data(), static_cast<UINT>(constants::FIVE_CHAR.size()),
				m_pTextFormat, tile.GetDrawRect(), m_pFiveColorBrush);
			break;
		case TileContent::SIX:
			m_pRenderTarget->DrawText(constants::SIX_CHAR.data(), static_cast<UINT>(constants::SIX_CHAR.size()),
				m_pTextFormat, tile.GetDrawRect(), m_pSixColorBrush);
			break;
		case TileContent::SEVEN:
			m_pRenderTarget->DrawText(constants::SEVEN_CHAR.data(), static_cast<UINT>(constants::SEVEN_CHAR.size()),
				m_pTextFormat, tile.GetDrawRect(), m_pSevenColorBrush);
			break;
		case TileContent::EIGHT:
			m_pRenderTarget->DrawText(constants::EIGHT_CHAR.data(), static_cast<UINT>(constants::EIGHT_CHAR.size()),
				m_pTextFormat, tile.GetDrawRect(), m_pEightColorBrush);
			break;
		case TileContent::MINE:
			m_pRenderTarget->DrawText(constants::BOMB_CHAR.data(), static_cast<UINT>(constants::BOMB_CHAR.size()),
				m_pEmojiFormat, tile.GetDrawRect(), m_pQuestionMarkColorBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
			break;
		}
		break;
	}
}