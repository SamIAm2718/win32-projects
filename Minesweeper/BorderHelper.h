#include <atlbase.h>
#include <d2d1.h>
#include <windef.h>

namespace BorderHelper
{
	HRESULT DrawBorder(const RECT& boundingBox, const FLOAT strokeSize, ID2D1Factory* pFactory, ID2D1RenderTarget* pRenderTarget,
		ID2D1Brush* pLeftEdgeBrush, ID2D1Brush* pTopEdgeBrush, ID2D1Brush* pRightEdgeBrush, ID2D1Brush* pBottomEdgeBrush)
	{
		// Create resources needed for making border geometry.
		CComPtr<ID2D1GeometrySink> pSink{ nullptr };
		HRESULT hr = S_OK;
		const FLOAT rLeftF{ static_cast<FLOAT>(boundingBox.left) };
		const FLOAT rTopF{ static_cast<FLOAT>(boundingBox.top) };
		const FLOAT rRightF{ static_cast<FLOAT>(boundingBox.right) };
		const FLOAT rBottomF{ static_cast<FLOAT>(boundingBox.bottom) };

		CComPtr<ID2D1PathGeometry> pEdgeTriangleA{ nullptr };
		D2D1_RECT_F edgeRectangle{};
		CComPtr<ID2D1PathGeometry> pEdgeTriangleB{ nullptr };

		// We start by drawing the left border
		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreatePathGeometry(&pEdgeTriangleA);
		}

		if (SUCCEEDED(hr))
		{
			hr = pEdgeTriangleA->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(D2D1::Point2F(rLeftF, rTopF), D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddLine(D2D1::Point2F(rLeftF - strokeSize, rTopF - strokeSize));
				pSink->AddLine(D2D1::Point2F(rLeftF - strokeSize, rTopF));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				pSink->Close();
			}

			pSink.Release();
		}

		if (SUCCEEDED(hr))
		{
			edgeRectangle = D2D1::RectF(rLeftF - strokeSize, rTopF, rLeftF, rBottomF);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreatePathGeometry(&pEdgeTriangleB);
		}

		if (SUCCEEDED(hr))
		{
			hr = pEdgeTriangleB->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(D2D1::Point2F(rLeftF, rBottomF), D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddLine(D2D1::Point2F(rLeftF - strokeSize, rBottomF + strokeSize));
				pSink->AddLine(D2D1::Point2F(rLeftF - strokeSize, rBottomF));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				pSink->Close();
			}

			pSink.Release();
		}

		if (SUCCEEDED(hr))
		{
			pRenderTarget->FillGeometry(pEdgeTriangleA, pLeftEdgeBrush);
			pRenderTarget->FillRectangle(edgeRectangle, pLeftEdgeBrush);
			pRenderTarget->FillGeometry(pEdgeTriangleB, pLeftEdgeBrush);
			pEdgeTriangleA.Release();
			pEdgeTriangleB.Release();
		}

		// Now we draw the top border
		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreatePathGeometry(&pEdgeTriangleA);
		}

		if (SUCCEEDED(hr))
		{
			hr = pEdgeTriangleA->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(D2D1::Point2F(rRightF, rTopF), D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddLine(D2D1::Point2F(rRightF + strokeSize, rTopF - strokeSize));
				pSink->AddLine(D2D1::Point2F(rRightF, rTopF - strokeSize));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				pSink->Close();
			}

			pSink.Release();
		}

		if (SUCCEEDED(hr))
		{
			edgeRectangle = D2D1::RectF(rLeftF, rTopF - strokeSize, rRightF, rBottomF);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreatePathGeometry(&pEdgeTriangleB);
		}

		if (SUCCEEDED(hr))
		{
			hr = pEdgeTriangleB->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(D2D1::Point2F(rLeftF, rTopF), D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddLine(D2D1::Point2F(rLeftF - strokeSize, rTopF - strokeSize));
				pSink->AddLine(D2D1::Point2F(rLeftF, rTopF - strokeSize));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				pSink->Close();
			}

			pSink.Release();
		}

		if (SUCCEEDED(hr))
		{
			pRenderTarget->FillGeometry(pEdgeTriangleA, pTopEdgeBrush);
			pRenderTarget->FillRectangle(edgeRectangle, pTopEdgeBrush);
			pRenderTarget->FillGeometry(pEdgeTriangleB, pTopEdgeBrush);
			pEdgeTriangleA.Release();
			pEdgeTriangleB.Release();
		}

		// Next we draw the right border
		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreatePathGeometry(&pEdgeTriangleA);
		}

		if (SUCCEEDED(hr))
		{
			hr = pEdgeTriangleA->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(D2D1::Point2F(rRightF, rBottomF), D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddLine(D2D1::Point2F(rRightF + strokeSize, rBottomF + strokeSize));
				pSink->AddLine(D2D1::Point2F(rRightF + strokeSize, rBottomF));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				pSink->Close();
			}

			pSink.Release();
		}

		if (SUCCEEDED(hr))
		{
			edgeRectangle = D2D1::RectF(rRightF, rTopF, rRightF + strokeSize, rBottomF);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreatePathGeometry(&pEdgeTriangleB);
		}

		if (SUCCEEDED(hr))
		{
			hr = pEdgeTriangleB->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(D2D1::Point2F(rRightF, rTopF), D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddLine(D2D1::Point2F(rRightF + strokeSize, rTopF - strokeSize));
				pSink->AddLine(D2D1::Point2F(rRightF + strokeSize, rTopF));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				pSink->Close();
			}

			pSink.Release();
		}

		if (SUCCEEDED(hr))
		{
			pRenderTarget->FillGeometry(pEdgeTriangleA, pRightEdgeBrush);
			pRenderTarget->FillRectangle(edgeRectangle, pRightEdgeBrush);
			pRenderTarget->FillGeometry(pEdgeTriangleB, pRightEdgeBrush);
			pEdgeTriangleA.Release();
			pEdgeTriangleB.Release();
		}

		// Finally we draw the bottom border
		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreatePathGeometry(&pEdgeTriangleA);
		}

		if (SUCCEEDED(hr))
		{
			hr = pEdgeTriangleA->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(D2D1::Point2F(rLeftF, rBottomF), D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddLine(D2D1::Point2F(rLeftF - strokeSize, rBottomF + strokeSize));
				pSink->AddLine(D2D1::Point2F(rLeftF, rBottomF + strokeSize));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				pSink->Close();
			}

			pSink.Release();
		}

		if (SUCCEEDED(hr))
		{
			edgeRectangle = D2D1::RectF(rLeftF, rBottomF, rRightF, rBottomF + strokeSize);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFactory->CreatePathGeometry(&pEdgeTriangleB);
		}

		if (SUCCEEDED(hr))
		{
			hr = pEdgeTriangleB->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(D2D1::Point2F(rRightF, rBottomF), D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddLine(D2D1::Point2F(rRightF + strokeSize, rBottomF + strokeSize));
				pSink->AddLine(D2D1::Point2F(rRightF, rBottomF + strokeSize));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				pSink->Close();
			}

			pSink.Release();
		}

		if (SUCCEEDED(hr))
		{
			pRenderTarget->FillGeometry(pEdgeTriangleA, pBottomEdgeBrush);
			pRenderTarget->FillRectangle(edgeRectangle, pBottomEdgeBrush);
			pRenderTarget->FillGeometry(pEdgeTriangleB, pBottomEdgeBrush);
			pEdgeTriangleA.Release();
			pEdgeTriangleB.Release();
		}

		return hr;
	}
}