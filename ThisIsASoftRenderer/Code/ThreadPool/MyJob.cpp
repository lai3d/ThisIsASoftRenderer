#include "stdafx.h"
#include "ThreadPool/MyJob.h"
#include "Utility.h"
#include "Renderer.h"
#include "RenderUtil.h"
#include "RenderObject.h"
#include "GeometryDef.h"

namespace SR
{
	/////////////////////////////////////////////////////
	BOOL JobVS::OnInitialize()
	{
		return TRUE;
	}

	BOOL JobVS::Run()
	{
		JobParamVS* param = (JobParamVS*)m_JobParam;

		SR::SRenderContext context;
		context.pMaterial = param->object->m_pMaterial;
		SR::RenderUtil::ObjectTnL(*param->object, context);
		g_env.renderer->GetCurRas()->RasterizeTriangleList(context);

		return TRUE;
	}

	VOID JobVS::OnFinalize()
	{
		SAFE_DELETE(m_JobParam);
		delete this;
	}

	void JobVS::OnCancelJob()
	{
		SAFE_DELETE(m_JobParam);
		delete this;
	}

	/////////////////////////////////////////////////////
	BOOL JobRS::OnInitialize()
	{
		return TRUE;
	}

	BOOL JobRS::Run()
	{
		JobParamRS* param = (JobParamRS*)m_JobParam;

		SScanLinesData rasData;
		g_env.renderer->GetCurRas()->RasTriangleSetup(rasData, &param->v0, &param->v1, &param->v2, param->triType);
		rasData.texLod = param->texLod;
		rasData.pMaterial = param->pMaterial;

		RenderUtil::RasterizeScanLines(rasData);

		return TRUE;
	}

	VOID JobRS::OnFinalize()
	{
		SAFE_DELETE(m_JobParam);
		delete this;
	}

	void JobRS::OnCancelJob()
	{
		SAFE_DELETE(m_JobParam);
		delete this;
	}

	/////////////////////////////////////////////////////
	BOOL JobPS::OnInitialize()
	{
		return TRUE;
	}

	BOOL JobPS::Run()
	{
		JobParamPS* param = (JobParamPS*)m_JobParam;

		g_env.renderer->GetCurRas()->FragmentPS(*param->frag);

		return TRUE;
	}

	VOID JobPS::OnFinalize()
	{
		SAFE_DELETE(m_JobParam);
		delete this;
	}

	void JobPS::OnCancelJob()
	{
		SAFE_DELETE(m_JobParam);
		delete this;
	}

}