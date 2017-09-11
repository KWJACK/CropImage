// BMPZoomView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPRE.h"
#include "BMPZoomView.h"
#include "memdc.h"	//include �ؾ� memdc ���� ���
#include "MainFrm.h"
#include <cmath>	//abs float
#include <algorithm>//swap
// CBMPZoomView
#include "SetFileNameDlg.h"		//���� �̸� ���� �ٵ��α�
#include "SetImagePathDlg.h"	//�н� ���� ���̾�α�
#include "SetResultPathDlg.h"	//��� �н� ���� ���̾�α�


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

// CBMPZoomView �׸����Դϴ�.

void CBMPZoomView::OnDraw(CDC* pDC)
{	
}


// CBMPZoomView �����Դϴ�.

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
	//MainFrame�� �ּҸ� �� MainFrame�� ���� �ִ� ListDlg�� PreviewDlg�ּҿ� ZoomView�� �ּ� ����
	CMainFrame* myFrame = (CMainFrame* )GetParentFrame();//AFXGetParentFram�� ����ص� �˴ϴ�.	
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


// CBMPZoomView �޽��� ó�����Դϴ�.
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
	
	if(m_prev_object){//���� ���� ��		
		m_select_flag = PosInObject(point.x, point.y);
		if(m_select_flag ==0)m_prev_object=FALSE;
		else{
			m_ptPrev = point;
			SetCapture();
		}
	}
	if(m_select_flag==0){//���� ������ �ƴ� �Ϲ� �׸����		
		if (!m_bSelectMode && m_pSelectedImage!=NULL //�̹��� Ŭ�� Ȯ��
			&& point.x >= m_ImageDest.x && point.y >= m_ImageDest.y)//����Ȯ��
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

		if(m_prev_object){//0 �� �ƴ϶��1,2,3,4,5,6,7,8,9 �� ���ؼ��� if�� ����
		CRect r(m_ptStart.x - 7, m_ptStart.y - 7, m_ptStart.x + 7, m_ptStart.y + 7);
		POINT pos = { a_x, a_y };
		for (int i = 0; i < 8; i++) {
			if (r.PtInRect(pos)) {
				return 2+i;
			}
			r += CPoint(half_width * (((i / 2) % 2) == 1)*(1 - ((i / 6) * 2)),//6�� �����°� ��ȣ �ٲٷ���
				half_height * (((i / 2) % 2) == 0)*(1 - ((i / 4) * 2)));// ���������� (1-2)
		}
	}

	return (m_ptStart.x <= a_x && m_ptEnd.x >= a_x &&	m_ptStart.y <= a_y && m_ptEnd.y >= a_y);//0~1 ��ȯ	
}


void CBMPZoomView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bSelectMode && !m_select_flag){
		CClientDC dc(this);	
		dc.SelectStockObject(NULL_BRUSH);
		CPen m_penDot(PS_DOT, 1, RGB(0, 0, 0));
		dc.SelectObject(&m_penDot);		
		// ������ �׸� ������ �����.
		//dc.Rectangle(m_ptStart.x,m_ptStart.y,m_ptEnd.x,m_ptEnd.y);
		// ���ο� ������ �׸���.
		dc.Rectangle(m_ptStart.x,m_ptStart.y,point.x,point.y);
		// ���� ������ ��ǥ�� �����Ѵ�.
		m_ptEnd = point;

		dc.SelectStockObject(NULL_BRUSH);
		dc.SelectStockObject(BLACK_PEN);
		
		int half_width = (m_ptEnd.x - m_ptStart.x)>>1;
		int half_height = (m_ptEnd.y - m_ptStart.y)>>1;

		CRect r(m_ptStart.x - 3, m_ptStart.y - 3, m_ptStart.x + 3, m_ptStart.y + 3);

		for (int i = 0; i < 8; i++) {
				dc.Rectangle(r);
				r += CPoint(half_width * ((i / 2) % 2 ==1)*(1-(i / 6)*2),//6�� �����°� ��ȣ �ٲٷ���
				half_height * ((i / 2) % 2 == 0)*(1 -(i / 4)*2));// ���������� (1-2)
			}
	}	

	if(m_pSelectedImage!=NULL){		
		CPoint pt = point + GetScrollPosition();
		int m_nZoom= 1;//�̹��� Ȯ�� ����
		pt.x /= m_nZoom;
		pt.y /= m_nZoom;
		ShowImageInfo(pt);
		Invalidate(FALSE);
		//���콺 ����		
	}	
	if(m_select_flag){
		MovePos(point.x - m_ptPrev.x, point.y - m_ptPrev.y);
		m_ptPrev = point;//mousemove�ÿ� ������ ������Ʈ
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
		//if(m_ptStart.x > m_ptEnd.x)	std::swap(m_ptStart.x, m_ptEnd.x);//��ǥ ü�谡 �ٸ��ٸ� �����մϴ�
		//if(m_ptStart.y > m_ptEnd.y) std::swap(m_ptStart.y, m_ptEnd.y);//�ڵ� ����
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

		if(m_ptStart.x > m_ptEnd.x)	std::swap(m_ptStart.x, m_ptEnd.x);//��ǥ ü�谡 �ٸ��ٸ� �����մϴ�
		if(m_ptStart.y > m_ptEnd.y) std::swap(m_ptStart.y, m_ptEnd.y);//�ڵ� ����

		if(m_bSelectMode && m_ptEnd.x <= m_ExpandDest.x+m_ImageDest.x // ���� ���� x,y Ȯ��
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

			if( Width<3 || Height<5)return;//�簢���� �ʹ� ������ �������� �ʴ´�	

			Gdiplus::RectF bmpRect(0.0f, 0.0f, Width, Height);		
			Bitmap *pCloneBmp = m_pSelectedImage->Clone(pStartW, pStartH, Width,Height, m_pSelectedImage->GetPixelFormat());
			Graphics *pGraphics = Graphics::FromImage(pCloneBmp);
			pGraphics->DrawImage(pCloneBmp, bmpRect);	

			OnSaveCropImageFile();//�����̸����� �Է��Ѱ� �´��� �˻� & ���� �ڵ��ϼ� �Լ�
			if(m_IDOK){//��ȭ���ڿ��� OK�� �������� üũ�ϴ� �ɼ� ��
				UINT num,size;
				ImageCodecInfo* pImageCodecInfo;
				GetImageEncodersSize(&num,&size);
				pImageCodecInfo =(ImageCodecInfo *)malloc(size);
				GetImageEncoders(num,size,pImageCodecInfo);
				pCloneBmp->Save(m_sPath+m_fileName+L".bmp",&pImageCodecInfo[0].Clsid);

				//FormBMP(���� ���� â)�� ����ȭ�� ����� �׸� �� �ֵ��� �մϴ�.
				writetoFormBMP(&(m_sPath+m_fileName+L".bmp"));//FormBMP Ʈ���� �Լ�
				free(pImageCodecInfo);			
			}
			delete pCloneBmp;
			delete pGraphics;		
		}
	}
	CZoomView::OnLButtonUp(nFlags, point);
}

void CBMPZoomView::writetoFormBMP(CString* a_filepath){
	//���� ������ �ִٸ� �����մϴ�.
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
	// ���¹ٿ� ���콺 ��ǥ ǥ��
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

	// ���¹ٿ� �̹��� ���� ǥ��	
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
		//��� ���
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
			
			//Rect ���� ��ȭ�ϸ� ��� ����� ���� �� �� �ִ�.?
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
			//��ũ�ѹ� �̵��� ��ǥ����� ���� ����
			::Rectangle(lpDrawItemStruct->hDC, 
				m_ptStart.x-m_ImageDest.x,
				m_ptStart.y-m_ImageDest.y,
				m_ptEnd.x - m_ImageDest.x,
				m_ptEnd.y-m_ImageDest.y);					
			delete pMemDC;
		}		
	}
	//CZoomView::OnDrawItem(nIDCtl, lpDrawItemStruct); ���ƾ� ���� ���� �ʽ��ϴ�.
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
			m_ImageDest.x       = (nWidth-m_ExpandDest.x)/2;//�̹��� �߾����� ���
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

//�ڸ� �̹��� ����� �ܾ� �˻�, �ڵ��ϼ� ���ִ� �κ�
void CBMPZoomView::OnSaveCropImageFile()
{
	CSetFileNameDlg dlg;
	dlg.m_fileName = m_oldFileName;//Dlg�� ���� �ֱ� ��m_pSelectedImage���̸� ���
	if (dlg.DoModal() == IDOK)
	{
		if(dlg.m_fileName.GetLength() < 8){
			m_IDOK = FALSE;
			return;//adxxxx_ �� �߰��� �Է��Ѱ� �־�� ����
		}

		m_fileName = m_oldFileName =dlg.m_fileName;//������ �ۼ��� �����̸��� ���߿��� ����մϴ�.
		CString Address = wcstok((TCHAR*)(LPCTSTR)m_fileName, L"_");
		CString Word = wcstok(NULL, L"_");		
		TCHAR* Index = wcstok(NULL, L"_");
		if(Index ==NULL)return;//����ڰ� �߸� �Է��� ��� ó��

		int tempIndex = _wtoi(Index);		 
		CString newIndex;
		newIndex.Format(L"%d", tempIndex+1);		
		m_fileName  = Address+L"_"+Word+L"_"+Index+L"_"+Word.GetAt(tempIndex);
		m_oldFileName = Address+L"_"+Word+L"_"+ newIndex +L"_"+Word.GetAt(++tempIndex);		

		if(tempIndex  >= Word.GetLength()){
			m_oldFileName = Address+L"_";//�Է¹��ڿ��� �ʰ��ϸ� up)
		}		
		m_IDOK = TRUE;
		return;
		//ó�� ���� �̸� �Է��� ��� �ε��� �� �ڵ����� �����ϰ� �ϸ� �ɵ�
	}
	else m_IDOK = FALSE;
}

//�ڸ� ���� ���� ��ġ ����
void CBMPZoomView::OnSetImagePath()
{
	SetImagePathDlg dlg;	
	if (dlg.DoModal() == IDOK)
	{
		m_sPath = dlg.m_path +L"\\";		
	}	
}

//�̹��� ���� ��� ���� �Լ�
void CBMPZoomView::OnSetResultPath()
{
	CSetResultPathDlg dlg;
	if(dlg.DoModal() == IDOK){
		m_result_path = dlg.m_reslut_path;
	}	
}

//����� ������ ����ȭ
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
		FindClose(hFind);//handle ��ȯ				
	}
	ShellExecute(NULL, L"open", L"explorer", m_result_path, NULL, SW_SHOW);
}

//1bpp �ķ�Ʈ ���� �Լ�
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
		FindClose(hFind);//handle ��ȯ
		CString m_convert_path = L".\\convert_1bpp";
		ShellExecute(NULL, L"open", L"explorer", m_convert_path, NULL, SW_SHOW);
		return;
	}
	return;
}


//������Ʈ ������ �����
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
			r += CPoint(half_width * ((i / 2) % 2 ==1)*(1-(i / 6)*2),//6�� �����°� ��ȣ �ٲٷ���
			half_height * ((i / 2) % 2 == 0)*(1 -(i / 4)*2));// ���������� (1-2)
		}
	}
}


void CBMPZoomView::OnSaveKey()
{
	m_bSelectMode = TRUE;	
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	PostMessage(WM_LBUTTONUP,0,MAKELPARAM(m_ptEnd.x, m_ptEnd.y));
}

//bitmap24bit->1bpp Ȱ��ȭ �Լ�
void CBMPZoomView::OnUpdate32775(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(true);
}

//1bpp �����ڵ� �޴� Ȱ��ȭ �Լ�
void CBMPZoomView::OnUpdate32776(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(true);
}

