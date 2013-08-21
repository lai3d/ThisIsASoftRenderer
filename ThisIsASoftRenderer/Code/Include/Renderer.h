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

namespace SR
{
	/////////////////////////////////////////////////////////////
	struct SRenderContext 
	{
		SRenderContext():pMaterial(nullptr) {}

		VertexBuffer	verts;
		FaceList		faces;
		SMaterial*		pMaterial;
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
		void	SetRasterizeType(eRasterizeType type);
		void	OnFrameMove();
		Rasterizer*	GetCurRas() { return m_curRas; }
		bool	IsCurSponzaScene();
		//�л���Ⱦģʽ(wireframe, flat, gouraud, phong)
		void	ToggleShadingMode();
		//��ȡ��ǰ��Ⱦģʽ����
		const char*	GetCurShadingModeName() const;
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

	private:
		void	_InitAllScene();
		//���֡����,z-buffer
		void	_Clear(const SColor& color, float depth);

	private:
		std::unique_ptr<Gdiplus::Bitmap>	m_bmBackBuffer;
		std::unique_ptr<SR::PixelBox>	m_backBuffer;
		std::unique_ptr<SR::PixelBox>	m_zBuffer;

		std::unordered_map<eRasterizeType, Rasterizer*>	m_rasLib;		//���п���shader
		Rasterizer*							m_curRas;					//��ǰʹ��shader

		std::unordered_map<STRING, SMaterial*>	m_matLib;				//���ʿ�

		std::vector<Scene*>					m_scenes;					//���в��Գ���
		size_t								m_curScene;					//��ǰ��������
	};
}

#endif // Renderer_h__