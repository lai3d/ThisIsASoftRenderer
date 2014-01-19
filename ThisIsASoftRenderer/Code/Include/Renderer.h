/********************************************************************
	created:	30:6:2013   23:46
	filename	Renderer.h
	author:		maval

	purpose:	����Ⱦ��������
*********************************************************************/
#ifndef Renderer_h__
#define Renderer_h__

#include "Prerequiestity.h"
#include "Camera.h"
#include "MathDef.h"
#include "Rasterizer.h"
#include "GeometryDef.h"
#include "RenderObject.h"

namespace SR
{
	/////////////////////////////////////////////////////////////
	struct SRenderContext 
	{
		SRenderContext():pMaterial(nullptr),texLod(0) {}

		VertexBuffer	verts;
		FaceList		faces;
		SMaterial*		pMaterial;
		int				texLod;		// Mip level for texture
	};

	/////////////////////////////////////////////////////////////
	class Renderer
	{
		friend class RenderUtil;
	public:
		Renderer();
		~Renderer();

		Camera			m_camera;
		SDirectionLight	m_testLight;		//���Է����
		SColor			m_ambientColor;
		SFragment*		m_fragmentBuffer;	//����Ǹ��ڴ��

	public:
		void	Init();
		void	OnFrameMove();
		Rasterizer* GetRasterizer(eRasterizeType type);
		Rasterizer*	GetCurRas() { return m_curRas; }
		void	SetEnableZTest(bool bEnable) { m_bZTestEnable = bEnable; }
		void	SetEnableZWrite(bool bEnable) { m_bZWriteEnable = bEnable; }
		bool	GetEnableZTest() const { return m_bZTestEnable; }
		bool	GetEnableZWrite() const { return m_bZWriteEnable; }
		PixelBox*	GetCurZBuffer()	{ return m_zBuffer[m_iCurOutputZBuffer].get(); }
		PixelBox*	GetAnotherZBuffer() { return m_zBuffer[(m_iCurOutputZBuffer+1)%2].get(); }
		//�л����Գ���
		void	ToggleScene();
		//��Ⱦ����
		void	RenderOneFrame();
		//����ǰ�󻺳�
		void	Present();
		//��Ӳ���
		void	AddMaterial(const STRING& name, SMaterial* mat);
		//��ȡ����
		SMaterial*	GetMaterial(const STRING& name);
		// RTT
		void	SetRenderTarget(PixelBox* pRT, int iZBuffer = 0);
		// Set z-test func, NB: we have two depth buffer
		void	SetZfunc(int i, eZFunc func);
		eZFunc	GetCurZFunc() const { return m_zFunc[m_iCurOutputZBuffer]; }
		eZFunc	GetAnotherZFunc() const { return m_zFunc[(m_iCurOutputZBuffer+1)%2]; }

	private:
		void	_InitAllScene();
		void	_FlushRenderList(RenderList& renderList);
		void	_RenderTransparency_OIT();
		//���֡����,z-buffer
		void	_Clear(const SColor& color, float depth);
		void	_ClearBufferImpl(PixelBox* pBuffer, DWORD val);

	private:
		std::unique_ptr<Gdiplus::Bitmap>	m_bmBackBuffer;
		std::unique_ptr<PixelBox>			m_backBuffer;	
		std::unique_ptr<PixelBox>			m_zBuffer[2];	// Second one for OIT
		eZFunc								m_zFunc[2];
		bool								m_bZTestEnable;
		bool								m_bZWriteEnable;		

		// Use for depth-peeling OIT
		PixelBox*							m_frameBuffer;
		int									m_iCurOutputZBuffer;
		std::unique_ptr<PixelBox>			m_backBuffer_OIT[OIT_LAYER];

		std::unordered_map<eRasterizeType, Rasterizer*>	m_rasLib;		//���п���shader
		Rasterizer*							m_curRas;					//��ǰʹ��shader

		std::unordered_map<STRING, SMaterial*>	m_matLib;				//���ʿ�

		std::vector<Scene*>					m_scenes;					//���в��Գ���
		size_t								m_curScene;					//��ǰ��������
	};
}

#endif // Renderer_h__