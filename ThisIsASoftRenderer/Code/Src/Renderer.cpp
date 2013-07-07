#include "stdafx.h"
#include "Renderer.h"
#include "PixelBox.h"

namespace SR
{
	Renderer::Renderer()
		:m_hwnd(nullptr)
		,m_curRas(nullptr)
	{
		//��ʼ�����й�դ��
		m_rasLib.insert(std::make_pair(eRasterizeType_FlatWire, new RasWireFrame));

		//�����󱸻���
		m_backBuffer.reset(new Common::PixelBox(SCREEN_WIDTH, SCREEN_HEIGHT, 2));
	}

	Renderer::~Renderer()
	{
		for(auto iter=m_rasLib.begin(); iter!=m_rasLib.end(); ++iter)
			delete iter->second;
		m_rasLib.clear();
	}

	void Renderer::SetRasterizeType( eRasterizeType type )
	{
		auto iter = m_rasLib.find(type);
		assert(iter != m_rasLib.end());

		m_curRas = iter->second;
	}

	void Renderer::RenderOneFrame()
	{
		/////////////////////////////////////////////////
		///////// ˢ�º󱸻���
		_Clear();

		//TODO:���������Χ��������ռ����culling

		//TODO:����ռ���б����޳�
		//		g_camera.GetViewPt();

		//���RenderList
		m_renderList.verts.clear();
		m_renderList.verts.assign(m_VB.begin(), m_VB.end());

		m_renderList.indexes.clear();
		m_renderList.indexes.assign(m_IB.begin(), m_IB.end());
		

		m_camera.Update();

		//transform each vertex
		for (size_t i=0; i<m_renderList.verts.size(); ++i)
		{
			VEC4& vertPos = m_renderList.verts[i].pos;

			/////////////////////////////////////////////////
			///////// ����任
			auto matView = m_camera.GetViewMatrix();
			vertPos = Common::Transform_Vec4_By_Mat44(vertPos, matView);

			/////////////////////////////////////////////////
			///////// ͸��ͶӰ�任
			auto matProj = m_camera.GetProjMatrix();
			vertPos = Common::Transform_Vec4_By_Mat44(vertPos, matProj);

			/////////////////////////////////////////////////
			///////// ��γ���
			float inv_w = 1 / vertPos.w;
			vertPos.x *= inv_w;
			vertPos.y *= inv_w;

			/////////////////////////////////////////////////
			///////// �ӿ�ӳ�� [-1,1] -> [0, Viewport W/H]
			float a = 0.5f * SCREEN_WIDTH;
			float b = 0.5f *SCREEN_HEIGHT;

			vertPos.x = a + a * vertPos.x;
			vertPos.y = b - b * vertPos.y;
		}

		/////////////////////////////////////////////////
		///////// ��դ��
		m_curRas->RasterizeTriangleList(m_renderList);

		/////////////////////////////////////////////////
		///////// ������swap
		_Present();
	}

	void Renderer::_Present()
	{
		HDC dc = ::GetDC(m_hwnd);
		assert(dc);

		RECT rect;
		GetClientRect(m_hwnd, &rect);
		const void* memory = m_backBuffer->GetDataPointer();

		BITMAPV4HEADER bi;
		ZeroMemory (&bi, sizeof(bi));
		bi.bV4Size = sizeof(BITMAPINFOHEADER);
		bi.bV4BitCount = m_backBuffer->GetBitsPerPixel();
		bi.bV4Planes = 1;
		bi.bV4Width = m_backBuffer->GetWidth();
		bi.bV4Height = -m_backBuffer->GetHeight();		//���ı�ʾY������,��MSDN
		bi.bV4V4Compression = BI_RGB;
		// 			bi.bV4AlphaMask = 0x1 << 15;
		// 			bi.bV4RedMask = 0x1f << 10;
		// 			bi.bV4GreenMask = 0x1f << 5;
		// 			bi.bV4BlueMask = 0x1f;

		StretchDIBits(dc, 0,0, rect.right, rect.bottom,
			0, 0, m_backBuffer->GetWidth(), m_backBuffer->GetHeight(),
			memory, (const BITMAPINFO*)(&bi), DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(m_hwnd, dc);
	}

	void Renderer::_Clear()
	{
		int color = 0;
		int bufBytes = m_backBuffer->GetWidth() * m_backBuffer->GetHeight() * m_backBuffer->GetBytesPerPixel();
		memset(m_backBuffer->GetDataPointer(), color, bufBytes);
	}

	void Renderer::AddRenderable(const VertexBuffer& vb, const IndexBuffer& ib)
	{
		m_VB = vb;
		m_IB = ib;
	}

}