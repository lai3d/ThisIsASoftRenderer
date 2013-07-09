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

		///test single triangle
// 		SRenderObj obj;
// 
// 		SR::SVertex v1, v2, v3;
// 		v1.pos = VEC4(-20, -15, 0, 1);
// 		v2.pos = VEC4(20, -15, 0, 1);
// 		v3.pos = VEC4(0, 15, 0, 1);
// 
// 		obj.VB.push_back(v1);
// 		obj.VB.push_back(v2);
// 		obj.VB.push_back(v3);
// 
// 		obj.IB.push_back(0);
// 		obj.IB.push_back(1);
// 		obj.IB.push_back(2);
// 
// 		obj.boundingRadius = RenderUtil::ComputeBoundingRadius(obj.VB);
// 
// 		AddRenderable(obj);
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
		m_camera.Update();

		/////////////////////////////////////////////////
		///////// ˢ�º󱸻���
		_Clear();

		//TODO:����ռ���б����޳�
		//		g_camera.GetViewPt();

		//for each object
		for (size_t iObj=0; iObj<m_renderList.size(); ++iObj)
		{
			SRenderObj& obj = m_renderList[iObj];
			obj.ResetState();

			/////////////////////////////////////////////////
			///////// ��׶�ü�
			if(m_camera.ObjectFrustumCulling(obj))
			{
				obj.m_bCull = true;
				continue;
			}

			//�����޸�����ı�������
			VertexBuffer workingVB;
			IndexBuffer workingIB;

			workingVB.assign(obj.VB.begin(), obj.VB.end());
			workingIB.assign(obj.IB.begin(), obj.IB.end());

			//transform each vertex
			for (size_t iVert=0; iVert<workingVB.size(); ++iVert)
			{
				VEC4& vertPos = workingVB[iVert].pos;

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
			///////// ��դ������
			m_curRas->RasterizeTriangleList(workingVB, workingIB);
		}

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

	void Renderer::AddRenderable(const SRenderObj& obj)
	{
		m_renderList.push_back(obj);
	}

}