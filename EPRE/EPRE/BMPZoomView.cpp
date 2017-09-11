// BMPZoomView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "BMPZoomView.h"
#include "memdc.h"	//include 해야 memdc 정상 기능
#include "MainFrm.h"
#include <cmath>	//abs float
#include <algorithm>//swap
// CBMPZoomView
#include "SetFileNameDlg.h"		//파일 이름 지정 다디얼로그
#include "SetImagePathDlg.h"	//패스 지정 다이얼로그
#include "SetResultPathDlg.h"	//결과 패스 지정 다이얼로그


IMPLEMENT_DYNCREATE(CBMPZoomView, CView)
using namespace Gdiplus; 
CBMPZoomView::CBMPZoomView() : m_create_canvas(FALSE), m_select_flag(0)
{
	m_bSelectMode = FALSE;	
	m_prev_object = FALSE;
	m_pSelectedImage = NULL;
	m_fResolution_H=0.0f;m_fResolution_W=0.0f;
	HDC hdc = ::GetDC(m_hWnd);
	m_IDOK = FALSE;
	m_sPath = L".\\result\\";//default	
	m_result_path = L".\\images";
	//you can change this in runtime
	m_BMPclass = new MyBMPclass;
}

CBMPZoomView::~CBMPZoomView()
{
	if(m_BMPclass){
		delete m_BMPclass;
		m_BMPclass = nullptr;
	}
}

BEGIN_MESSAGE_MAP(CBMPZoomView, CView)
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()	
	ON_COMMAND(ID_32774, &CBMPZoomView::OnSaveCropImageFile)
	ON_COMMAND(ID_32777, &CBMPZoomView::OnSetImagePath)
	ON_COMMAND(ID_32775, &CBMPZoomView::On24bitBMPto1bitBinarization)
	ON_COMMAND(ID_32776, &CBMPZoomView::OnConvert1bpp)
	ON_COMMAND(ID_SET_RESULT_PATH, &CBMPZoomView::OnSetResultPath)		
	ON_WM_RBUTTONUP()	
	ON_COMMAND(ID_SAVE_KEY, &CBMPZoomView::OnSaveKey)
	ON_UPDATE_COMMAND_UI(ID_32775, &CBMPZoomView::OnUpdate32775)
	ON_UPDATE_COMMAND_UI(ID_32776, &CBMPZoomView::OnUpdate32776)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CBMPZoomView::OnDestroy()
{
	CZoomView::OnDestroy();
	if(m_BMPclass){
		delete m_BMPclass;
		m_BMPclass = nullptr;
	}
}

// CBMPZoomView 그리기입니다.

void CBMPZoomView::OnDraw(CDC* pDC)
{	
}


// CBMPZoomView 진단입니다.

#ifdef _DEBUG
void CBMPZoomView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBMPZoomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CBMPZoomView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CZoomView::PreCreateWindow(cs);
}


void CBMPZoomView::OnInitialUpdate()
{
	CZoomView::OnInitialUpdate();
	CRect rt; GetClientRect( &rt );
	//MainFrame의 주소를 얻어서 MainFrame이 갖고 있는 ListDlg의 PreviewDlg주소에 ZoomView의 주소 연결
	CMainFrame* myFrame = (CMainFrame* )GetParentFrame();//AFXGetParentFram을 사용해도 됩니다.	
	myFrame->m_pFormListDlg->m_cListCtrlRem.m_preViewDlg = this;

	if(m_create_canvas == FALSE){
		m_create_canvas= m_wndCanvas.Create( L"Image Preview Canvas", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW, rt, this, 80 );
	}
}


BOOL CBMPZoomView::OnPreparePrinting(CPrintInfo* pInfo)
{	
	return CZoomView::OnPreparePrinting(pInfo);
}


void CBMPZoomView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{	
	CZoomView::OnBeginPrinting(pDC, pInfo);
}


void CBMPZoomView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CZoomView::OnEndPrinting(pDC, pInfo);
}


// CBMPZoomView 메시지 처리기입니다.
void CBMPZoomView::OnViewZoomin() 
{
	SetZoomScale(m_zoomFactor + 1.0f);
}

void CBMPZoomView::OnViewZoomout() 
{
	SetZoomScale(m_zoomFactor - 1.0f);
}

void CBMPZoomView::OnLButtonDown(UINT nFlags, CPoint point)
{		
	
	if(m_prev_object){//영역 선택 시		
		m_select_flag = PosInObject(point.x, point.y);
		if(m_select_flag ==0)m_prev_object=FALSE;
		else{
			m_ptPrev = point;
			SetCapture();
		}
	}
	if(m_select_flag==0){//영역 선택이 아닌 일반 그리기시		
		if (!m_bSelectMode && m_pSelectedImage!=NULL //이미지 클릭 확인
			&& point.x >= m_ImageDest.x && point.y >= m_ImageDest.y)//범위확인
		{		
			m_bSelectMode = TRUE;
			m_select_flag =0;
			m_ptStart = m_ptEnd = point;
			SetCapture();
		}
	}
}

int CBMPZoomView::PosInObject(int a_x, int a_y){
	int half_width = (m_ptEnd.x - m_ptStart.x) >>1;
	int half_height =(m_ptEnd.y - m_ptStart.y) >>1;

		if(m_prev_object){//0 이 아니라면1,2,3,4,5,6,7,8,9 에 대해서는 if문 수행
		CRect r(m_ptStart.x - 7, m_ptStart.y - 7, m_ptStart.x + 7, m_ptStart.y + 7);
		POINT pos = { a_x, a_y };
		for (int i = 0; i < 8; i++) {
			if (r.PtInRect(pos)) {
				return 2+i;
			}
			r += CPoint(half_width * (((i / 2) % 2) == 1)*(1 - ((i / 6) * 2)),//6을 나누는건 부호 바꾸려고
				half_height * (((i / 2) % 2) == 0)*(1 - ((i / 4) * 2)));// 음수만들기는 (1-2)
		}
	}

	return (m_ptStart.x <= a_x && m_ptEnd.x >= a_x &&	m_ptStart.y <= a_y && m_ptEnd.y >= a_y);//0~1 반환	
}


void CBMPZoomView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bSelectMode && !m_select_flag){
		CClientDC dc(this);	
		dc.SelectStockObject(NULL_BRUSH);
		CPen m_penDot(PS_DOT, 1, RGB(0, 0, 0));
		dc.SelectObject(&m_penDot);		
		// 이전에 그린 직선을 지운다.
		//dc.Rectangle(m_ptStart.x,m_ptStart.y,m_ptEnd.x,m_ptEnd.y);
		// 새로운 직선을 그린다.
		dc.Rectangle(m_ptStart.x,m_ptStart.y,point.x,point.y);
		// 직선 끝점의 좌표를 갱신한다.
		m_ptEnd = point;

		dc.SelectStockObject(NULL_BRUSH);
		dc.SelectStockObject(BLACK_PEN);
		
		int half_width = (m_ptEnd.x - m_ptStart.x)>>1;
		int half_height = (m_ptEnd.y - m_ptStart.y)>>1;

		CRect r(m_ptStart.x - 3, m_ptStart.y - 3, m_ptStart.x + 3, m_ptStart.y + 3);

		for (int i = 0; i < 8; i++) {
				dc.Rectangle(r);
				r += CPoint(half_width * ((i / 2) % 2 ==1)*(1-(i / 6)*2),//6을 나누는건 부호 바꾸려고
				half_height * ((i / 2) % 2 == 0)*(1 -(i / 4)*2));// 음수만들기는 (1-2)
			}
	}	

	if(m_pSelectedImage!=NULL){		
		CPoint pt = point + GetScrollPosition();
		int m_nZoom= 1;//이미지 확대 배율
		pt.x /= m_nZoom;
		pt.y /= m_nZoom;
		ShowImageInfo(pt);
		Invalidate(FALSE);
		//마우스 무빙		
	}	
	if(m_select_flag){
		MovePos(point.x - m_ptPrev.x, point.y - m_ptPrev.y);
		m_ptPrev = point;//mousemove시에 기준점 업데이트
		Invalidate(FALSE);
	}
	CZoomView::OnMouseMove(nFlags, point);
}

char g_rect_table[9][4] = {
	{ 1,1,1,1 },
	{ 1,1,0,0 },
	{ 1,0,0,0 },
	{ 1,0,0,1 },
	{ 0,0,0,1 },
	{ 0,0,1,1 },
	{ 0,0,1,0 },
	{ 0,1,1,0 },
	{ 0,1,0,0 },
};

void CBMPZoomView::MovePos(int a_interval_x, int a_interval_y){
	if(m_select_flag == 1){
		m_ptStart.x += a_interval_x;
		m_ptStart.y += a_interval_y;
		m_ptEnd.x += a_interval_x;
		m_ptEnd.y += a_interval_y;
	}
	else {
		//if(m_ptStart.x > m_ptEnd.x)	std::swap(m_ptStart.x, m_ptEnd.x);//좌표 체계가 다르다면 스왑합니다
		//if(m_ptStart.y > m_ptEnd.y) std::swap(m_ptStart.y, m_ptEnd.y);//코드 절약
		if (g_rect_table[m_select_flag - 1][0])m_ptStart.x += a_interval_x;
		if (g_rect_table[m_select_flag - 1][1])m_ptStart.y += a_interval_y;
		if (g_rect_table[m_select_flag - 1][2])m_ptEnd.x += a_interval_x;
		if (g_rect_table[m_select_flag - 1][3])m_ptEnd.y += a_interval_y;
	}
}


void CBMPZoomView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bSelectMode){
		m_ptEnd = point;

		if(m_ptStart.x > m_ptEnd.x)	std::swap(m_ptStart.x, m_ptEnd.x);//좌표 체계가 다르다면 스왑합니다
		if(m_ptStart.y > m_ptEnd.y) std::swap(m_ptStart.y, m_ptEnd.y);//코드 절약

		if(m_bSelectMode && m_ptEnd.x <= m_ExpandDest.x+m_ImageDest.x // 끝점 범위 x,y 확인
			&& m_ptEnd.y <= m_ExpandDest.y+m_ImageDest.y){				
			m_bSelectMode = FALSE;
			m_prev_object = TRUE;
			CClientDC dc(this);		
			::ReleaseCapture();

			float pStartW = float(m_ptStart.x-m_ImageDest.x)/m_fResolution_W;
			float pStartH = float(m_ptStart.y-m_ImageDest.y)/m_fResolution_H;
			float pEndW = float(m_ptEnd.x-m_ImageDest.x)/m_fResolution_W; 
			float pEndH = float(m_ptEnd.y-m_ImageDest.y)/m_fResolution_H;
			float Width = pEndW-pStartW;
			float Height = pEndH-pStartH;

			if( Width<3 || Height<5)return;//사각형이 너무 작으면 저장하지 않는다	

			Gdiplus::RectF bmpRect(0.0f, 0.0f, Width, Height);		
			Bitmap *pCloneBmp = m_pSelectedImage->Clone(pStartW, pStartH, Width,Height, m_pSelectedImage->GetPixelFormat());
			Graphics *pGraphics = Graphics::FromImage(pCloneBmp);
			pGraphics->DrawImage(pCloneBmp, bmpRect);	

			OnSaveCropImageFile();//파일이름으로 입력한게 맞는지 검사 & 글자 자동완성 함수
			if(m_IDOK){//대화상자에서 OK를 눌렀는지 체크하는 옵션 값
				UINT num,size;
				ImageCodecInfo* pImageCodecInfo;
				GetImageEncodersSize(&num,&size);
				pImageCodecInfo =(ImageCodecInfo *)malloc(size);
				GetImageEncoders(num,size,pImageCodecInfo);
				pCloneBmp->Save(m_sPath+m_fileName+L".bmp",&pImageCodecInfo[0].Clsid);

				//FormBMP(왼쪽 구석 창)에 이진화된 결과를 그릴 수 있도록 합니다.
				writetoFormBMP(&(m_sPath+m_fileName+L".bmp"));//FormBMP 트리거 함수
				free(pImageCodecInfo);			
			}
			delete pCloneBmp;
			delete pGraphics;		
		}
	}
	CZoomView::OnLButtonUp(nFlags, point);
}

void CBMPZoomView::writetoFormBMP(CString* a_filepath){
	//이전 파일이 있다면 삭제합니다.
	CMainFrame* myFrame = (CMainFrame *)AfxGetMainWnd();	

	CString resultTemp = m_result_path+L"\\temp.bmp";
	CFileFind hFileFind; CFile hFile;
	if( hFileFind.FindFile(resultTemp.AllocSysString()) ){				
		if(myFrame->m_pFormBMP->m_pSelectedImage){
			delete myFrame->m_pFormBMP->m_pSelectedImage;
			myFrame->m_pFormBMP->m_pSelectedImage = nullptr;			
		}
		hFile.Remove(resultTemp);		
	}
	if(! m_BMPclass->runMake1bpp(*a_filepath, L"temp.bmp", bit24))
		return;	
	
	myFrame->m_pFormBMP->m_pSelectedImage  = Bitmap::FromFile(resultTemp.AllocSysString());		
	myFrame->m_pFormBMP->m_saveFlag = 1;
	::SendMessage(myFrame->m_pFormBMP->m_hWnd, WM_PAINT, 0,0);		
}

void CBMPZoomView::setColorStyle(CClientDC &dc, CPen &pen, CBrush &brush){
	pen.CreatePen(PS_DOT, 1, RGB(0,0,0));
 	dc.SelectObject(&pen);
	dc.SelectStockObject(NULL_BRUSH);	
}


void CBMPZoomView::ShowImageInfo(CPoint a_point)
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	
	int w = m_pSelectedImage->GetWidth();
	int h = m_pSelectedImage->GetHeight();
	CString strText;	
	// 상태바에 마우스 좌표 표시
	if (a_point.x >= 0 && a_point.y >= 0 &&
		a_point.x > m_ImageDest.x &&
		a_point.y > m_ImageDest.y &&
		a_point.x < m_ExpandDest.x+m_ImageDest.x &&
		a_point.y < m_ExpandDest.y+m_ImageDest.y)
	{
		int w = float(a_point.x-m_ImageDest.x)/m_fResolution_W; 
		int h = float(a_point.y-m_ImageDest.y)/m_fResolution_H;
		strText.Format(_T("(X:%d, y:%d)"), w, h);
		pFrame->m_wndStatusBar.SetPaneText(0, strText);
	}

	// 상태바에 이미지 정보 표시	
	strText.Format(_T("w:%d  h:%d  c:16M"), w, h);	
	pFrame->m_wndStatusBar.SetPaneText(1, strText);
}


void CBMPZoomView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CZoomView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CBMPZoomView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CZoomView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBMPZoomView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
   return DoMouseWheel(nFlags, zDelta, pt);
	//return CZoomView::OnMouseWheel(nFlags, zDelta, pt);
}


void CBMPZoomView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE )
	{
		m_Canvas_Rect = lpDrawItemStruct->rcItem;
		CMMemDC *pMemDC = NULL;
		pMemDC = new CMMemDC( lpDrawItemStruct->hDC );
		lpDrawItemStruct->hDC = pMemDC->m_hDC;
		//배경 흰색
		CRect rect; GetClientRect(rect);
		HBRUSH hBrush = ::CreateSolidBrush( RGB(255, 255, 255) );		
		::FillRect( lpDrawItemStruct->hDC, rect, hBrush );		
		DeleteObject( hBrush );
		
		if(!m_pSelectedImage)return;		
		
		if(m_pSelectedImage != NULL)
		{
			Gdiplus::Graphics graphics(lpDrawItemStruct->hDC );
			p_graphics = &graphics;
			graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			
			//Rect 값을 변화하면 출력 사이즈를 조절 할 수 있다.?
			graphics.DrawImage(m_pSelectedImage,	
				               Gdiplus::Rect( lpDrawItemStruct->rcItem.left, 
											  lpDrawItemStruct->rcItem.top,
											  lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
											  lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top)); 
			CPen pen;
			CBrush brush;
			pen.CreatePen(PS_DOT, 1, RGB(0,0,0));
			pMemDC->SelectStockObject(NULL_BRUSH);
			pMemDC->SelectObject(&pen);
			//스크롤바 이동시 좌표계산이 맞지 않음
			::Rectangle(lpDrawItemStruct->hDC, 
				m_ptStart.x-m_ImageDest.x,
				m_ptStart.y-m_ImageDest.y,
				m_ptEnd.x - m_ImageDest.x,
				m_ptEnd.y-m_ImageDest.y);					
			delete pMemDC;
		}		
	}
	//CZoomView::OnDrawItem(nIDCtl, lpDrawItemStruct); 막아야 에러 나지 않습니다.
}


void CBMPZoomView::OnSize(UINT nType, int cx, int cy)
{
	CZoomView::OnSize(nType, cx, cy);
	CRect rc; GetClientRect( rc );	
	
	if(m_pSelectedImage==NULL)
	{
		//rc.SetRectEmpty();
		//m_wndCanvas.MoveWindow( rc );
	}
	else
	{
		const int   nWidth    = rc.Width();
		const int   nHeight   = rc.Height();
		const float fRatio    = (float)nHeight/nWidth;
		const float fImgRatio = (float)m_pSelectedImage->GetHeight()/m_pSelectedImage->GetWidth();
				
		if( fImgRatio > fRatio )
		{
			m_ExpandDest.x  = nHeight/fImgRatio;
			m_ExpandDest.y = nHeight;
			m_ImageDest.x       = (nWidth-m_ExpandDest.x)/2;//이미지 중앙으로 출력
			m_ImageDest.y       = 0;
			
		}
		else
		{			
			m_ExpandDest.x  = nWidth;
			m_ExpandDest.y = nWidth*fImgRatio;
			m_ImageDest.x       = 0;
			m_ImageDest.y       = (nHeight-m_ExpandDest.y)/2;
		}

		m_fResolution_W = float(m_ExpandDest.x)/float(m_pSelectedImage->GetWidth());
		m_fResolution_H = float(m_ExpandDest.y)/float(m_pSelectedImage->GetHeight());

		CRect rect(m_ImageDest.x, m_ImageDest.y, m_ImageDest.x+m_ExpandDest.x, m_ImageDest.y+m_ExpandDest.y);
		//Invalidate();
		m_wndCanvas.MoveWindow( rect );
		m_wndCanvas.Invalidate();
	}
}

//자른 이미지 저장시 단어 검사, 자동완성 해주는 부분
void CBMPZoomView::OnSaveCropImageFile()
{
	CSetFileNameDlg dlg;
	dlg.m_fileName = m_oldFileName;//Dlg에 보일 최근 파m_pSelectedImage일이름 출력
	if (dlg.DoModal() == IDOK)
	{
		if(dlg.m_fileName.GetLength() < 8){
			m_IDOK = FALSE;
			return;//adxxxx_ 에 추가로 입력한게 있어야 진행
		}

		m_fileName = m_oldFileName =dlg.m_fileName;//이전에 작성한 파일이름을 나중에도 사용합니다.
		CString Address = wcstok((TCHAR*)(LPCTSTR)m_fileName, L"_");
		CString Word = wcstok(NULL, L"_");		
		TCHAR* Index = wcstok(NULL, L"_");
		if(Index ==NULL)return;//사용자가 잘못 입력한 경우 처리

		int tempIndex = _wtoi(Index);		 
		CString newIndex;
		newIndex.Format(L"%d", tempIndex+1);		
		m_fileName  = Address+L"_"+Word+L"_"+Index+L"_"+Word.GetAt(tempIndex);
		m_oldFileName = Address+L"_"+Word+L"_"+ newIndex +L"_"+Word.GetAt(++tempIndex);		

		if(tempIndex  >= Word.GetLength()){
			m_oldFileName = Address+L"_";//입력문자열이 초과하면 up)
		}		
		m_IDOK = TRUE;
		return;
		//처음 파일 이름 입력한 대로 인덱스 얻어서 자동으로 지정하게 하면 될듯
	}
	else m_IDOK = FALSE;
}

//자른 파일 저장 위치 지정
void CBMPZoomView::OnSetImagePath()
{
	SetImagePathDlg dlg;	
	if (dlg.DoModal() == IDOK)
	{
		m_sPath = dlg.m_path +L"\\";		
	}	
}

//이미지 저장 경로 변경 함수
void CBMPZoomView::OnSetResultPath()
{
	CSetResultPathDlg dlg;
	if(dlg.DoModal() == IDOK){
		m_result_path = dlg.m_reslut_path;
	}	
}

//취득한 영상을 이진화
void CBMPZoomView::On24bitBMPto1bitBinarization()
{	
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(m_sPath+L"*.*", &fd);
	CString newName =L"";
	TCHAR* FileName= nullptr;		
	if (INVALID_HANDLE_VALUE != hFind)
	{		
		do {
			if (fd.cFileName[0] == '.') {//current and parent path ignore						
				continue;
			}
			else {
				CString tempFileName = fd.cFileName;
				if(! m_BMPclass->runMake1bpp(m_sPath+tempFileName, tempFileName, bit24)){					
					return;
				}
			}								
		} while (FindNextFile(hFind, &fd));		
		FindClose(hFind);//handle 반환				
	}
	ShellExecute(NULL, L"open", L"explorer", m_result_path, NULL, SW_SHOW);
}

//1bpp 파레트 반전 함수
void CBMPZoomView::OnConvert1bpp()
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(m_sPath+L"*.*", &fd);
	CString newName =L"";
	TCHAR* FileName= nullptr;		
	if (INVALID_HANDLE_VALUE != hFind)
	{			
		do {
			if (fd.cFileName[0] == '.') {//current and parent path ignore						
				continue;
			}
			else {
				CString tempFileName = fd.cFileName;
				m_BMPclass->PaletteChange(m_result_path+L"\\" +tempFileName, tempFileName);
			}								
		} while (FindNextFile(hFind, &fd));		
		FindClose(hFind);//handle 반환
		CString m_convert_path = L".\\convert_1bpp";
		ShellExecute(NULL, L"open", L"explorer", m_convert_path, NULL, SW_SHOW);
		return;
	}
	return;
}


//오브젝트 선택을 취소함
void CBMPZoomView::OnRButtonUp(UINT nFlags, CPoint point)
{	
	m_select_flag =0;
	m_bSelectMode = FALSE;	
	//m_ptEnd = m_ptStart = m_ptPrev = 0;
	ReleaseCapture();
	
	CZoomView::OnRButtonUp(nFlags, point);
}


void CBMPZoomView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(m_select_flag){

		dc.SelectStockObject(NULL_BRUSH);
		dc.SelectStockObject(BLACK_PEN);
		
		int half_width = (m_ptEnd.x - m_ptStart.x)>>1;
		int half_height = (m_ptEnd.y - m_ptStart.y)>>1;

		CRect r(m_ptStart.x - 3, m_ptStart.y - 3, m_ptStart.x + 3, m_ptStart.y + 3);

		for (int i = 0; i < 8; i++) {
			dc.Rectangle(r);
			r += CPoint(half_width * ((i / 2) % 2 ==1)*(1-(i / 6)*2),//6을 나누는건 부호 바꾸려고
			half_height * ((i / 2) % 2 == 0)*(1 -(i / 4)*2));// 음수만들기는 (1-2)
		}
	}
}


void CBMPZoomView::OnSaveKey()
{
	m_bSelectMode = TRUE;	
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	PostMessage(WM_LBUTTONUP,0,MAKELPARAM(m_ptEnd.x, m_ptEnd.y));
}

//bitmap24bit->1bpp 활성화 함수
void CBMPZoomView::OnUpdate32775(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(true);
}

//1bpp 반전코드 메뉴 활성화 함수
void CBMPZoomView::OnUpdate32776(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(true);
}

