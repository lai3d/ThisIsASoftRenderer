
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

//STL
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <functional>

//rapidxml
#include "rapidxml/rapidxml.hpp"       
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

//XTP
#include <XTToolkitPro.h>

//GDI+
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

// SoftRenderer library
#include <Prerequiestity.h>
#include <Renderer.h>
#include <ThreadPool/ftlFake.h>
#include <ThreadPool/ftlThreadPool.h>


