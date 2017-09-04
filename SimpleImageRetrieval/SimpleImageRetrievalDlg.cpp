
// SimpleImageRetrievalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimpleImageRetrieval.h"
#include "SimpleImageRetrievalDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

RGBQUAD** rgbBuffer; //이미지를 저장할 변수
RGBQUAD** rgbImg;
int imgHeight, imgWidth;
int hsiHeight, hsiWidth;			// 동적할당 재거해주기 위해 정보 기억
int filteredHeight, filteredWidth;	// 동적할당 재거해주기 위해 정보 기억

float ** hueBuffer;				// H
float ** satuBuffer;			// S
float ** intenBuffer;			// I


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSimpleImageRetrievalDlg dialog



CSimpleImageRetrievalDlg::CSimpleImageRetrievalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SIMPLEIMAGERETRIEVAL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleImageRetrievalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSimpleImageRetrievalDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DBIMGS, &CSimpleImageRetrievalDlg::OnBnClickedButtonLoadDbimgs)
	ON_BN_CLICKED(IDC_BUTTON_INDEX_DBIMGS, &CSimpleImageRetrievalDlg::OnBnClickedButtonIndexDbimgs)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_QUERY, &CSimpleImageRetrievalDlg::OnBnClickedButtonLoadQuery)
	ON_BN_CLICKED(IDC_BUTTON_RANKING_QUERY, &CSimpleImageRetrievalDlg::OnBnClickedButtonRankingQuery)
END_MESSAGE_MAP()


// CSimpleImageRetrievalDlg message handlers

BOOL CSimpleImageRetrievalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	mQueryImg = (CStatic *)GetDlgItem(IDC_STATIC_QUERY_IMG);
	mDBImgs = new CStatic*[10];
	mDBImgs[0] = (CStatic *)GetDlgItem(IDC_STATIC_DB1);
	mDBImgs[1] = (CStatic *)GetDlgItem(IDC_STATIC_DB2);
	mDBImgs[2] = (CStatic *)GetDlgItem(IDC_STATIC_DB3);
	mDBImgs[3] = (CStatic *)GetDlgItem(IDC_STATIC_DB4);
	mDBImgs[4] = (CStatic *)GetDlgItem(IDC_STATIC_DB5);
	mDBImgs[5] = (CStatic *)GetDlgItem(IDC_STATIC_DB6);
	mDBImgs[6] = (CStatic *)GetDlgItem(IDC_STATIC_DB7);
	mDBImgs[7] = (CStatic *)GetDlgItem(IDC_STATIC_DB8);
	mDBImgs[8] = (CStatic *)GetDlgItem(IDC_STATIC_DB9);
	mDBImgs[9] = (CStatic *)GetDlgItem(IDC_STATIC_DB10);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimpleImageRetrievalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimpleImageRetrievalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimpleImageRetrievalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSimpleImageRetrievalDlg::OnBnClickedButtonLoadDbimgs()
{
	// TODO: Add your control notification handler code here
	if (dbImgs.size() > 0)
	{
		for (int i = 0; i < dbImgs.size(); i++)
			delete[] dbImgs[i].feature;
		dbImgs.clear();
	}

	CFileDialog dlg(TRUE, ".jpg", NULL, OFN_ALLOWMULTISELECT, "JPEG File (*.jpg)|*.jpg||");
	CString strFileList;
	const int c_cMaxFiles = 20000; //선택할 최대 파일 숫자
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;

	if (dlg.DoModal() == IDOK)
	{
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;)
		{
			// 전체삭제는 ResetContent
			Img img;
			img.distance = 0;
			img.filename = dlg.GetNextPathName(pos);
			dbImgs.push_back(img);
		}
		CString comment;
		comment.Format("전체 %d장의 DB이미지를 불러왔습니다.", dbImgs.size());
		AfxMessageBox(comment);
	}
	else
	{
		AfxMessageBox("Error: DB image files selection");
		return;
	}

	//3. 1-10번째 DB 이미지 출력
	for (int i = 0; i < 10; i++)
	{
		if (i >= dbImgs.size())
			break;

		IplImage* img = cvLoadImage(dbImgs[i].filename, CV_LOAD_IMAGE_COLOR);
		CRect wrect;
		mDBImgs[i]->GetClientRect(wrect);
		HBITMAP hb = convertIplImage2HBITMAP(img, wrect);
		mDBImgs[i]->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
		mDBImgs[i]->SetBitmap(hb);
	}
	UpdateData();
}


void CSimpleImageRetrievalDlg::OnBnClickedButtonIndexDbimgs()
{
	// TODO: Add your control notification handler code here
	if (dbImgs.size() < 0)
	{
		AfxMessageBox("Error: Null image in DB");
	}

	for (int i = 0; i < dbImgs.size(); i++)
	{
		dbImgs[i].feature = featureExtraction(dbImgs[i].filename);
	}

	CString comment;
	comment.Format("전체 %d장의 DB이미지의 특징 추출을 완료했습니다.", dbImgs.size());
	AfxMessageBox(comment);
}


void CSimpleImageRetrievalDlg::OnBnClickedButtonLoadQuery()
{
	// TODO: Add your control notification handler code here
	//1. filedialog로부터 이미지 가져오기
	CFileDialog dlg(TRUE, ".jpg", NULL, NULL, "JPEG File (*.jpg)|*.jpg||");
	if (IDOK != dlg.DoModal())
	{
		AfxMessageBox("Error: JPG file selection");
		return;
	}

	queryImg.filename = dlg.GetPathName();
	//2. queryImg 로드
	queryImg.feature = featureExtraction(queryImg.filename);
	if (!queryImg.feature)
	{
		AfxMessageBox("Error: Feature extraction");
		return;
	}

	//3. 화면에 뿌리기
	mQueryImg = (CStatic *)GetDlgItem(IDC_STATIC_QUERY_IMG);
	IplImage* img = cvLoadImage(queryImg.filename, CV_LOAD_IMAGE_COLOR);
	CRect wrect;
	mQueryImg->GetClientRect(wrect);
	HBITMAP hb = convertIplImage2HBITMAP(img, wrect);
	mQueryImg->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	mQueryImg->SetBitmap(hb);
	UpdateData();
}


void CSimpleImageRetrievalDlg::OnBnClickedButtonRankingQuery()
{
	// TODO: Add your control notification handler code here
	//거리값 계산
	for (int i = 0; i < dbImgs.size(); i++)
	{
		dbImgs[i].distance = 0;
		for (int j = 0; j < FULL_FEAT_DIM; j++) {							// ToDo. 유클라디언 or 코사인 시밀러티 사용으로 랭킹매기기, 코사인 추천
			
			dbImgs[i].distance = getCosineSimilarity(queryImg.feature , dbImgs[i].feature);
		}
		//printf("dbImg.feature : %f\n,  queryImg : %f\n", dbImgs[i].feature[1], queryImg.feature[1]);
		//printf("%f\n", dbImgs[i].distance);
	}

	//Ranking 수행
	for (int i = 0; i < dbImgs.size(); i++) {
		for (int j = 0; j < dbImgs.size(); j++)
		{
			if (dbImgs[j].distance < dbImgs[i].distance)
			{
				Img temp = dbImgs[i];
				dbImgs[i] = dbImgs[j];
				dbImgs[j] = temp;
			}
		}
	}
	

	//상위 10장 이미지 출력
	for (int i = 0; i < 10; i++)
	{
		if (i >= dbImgs.size())
			break;

		IplImage* img = cvLoadImage(dbImgs[i].filename, CV_LOAD_IMAGE_COLOR);
		CString tag = dbImgs[i].filename;
		tag = tag.Right(tag.GetLength() - tag.ReverseFind('\\') - 1);
		tag = tag.Left(tag.GetLength() - 4);
		tag.Format("%d:" + tag, i);
		CRect wrect;
		mDBImgs[i]->GetClientRect(wrect);
		Size2f fsize;
		fsize.width = (float)(10 * img->width)/(float)(wrect.Width());
		fsize.height = (float)(10 * img->height) / (float)(wrect.Height());
		HBITMAP hb = convertIplImage2HBITMAPWithComment(img, wrect, tag);

		mDBImgs[i]->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
		mDBImgs[i]->SetBitmap(hb);
	}
	UpdateData();
}

// 255개를 20개씩 13개로? 13차원 -> *9 (grid)
double* CSimpleImageRetrievalDlg::featureExtraction(CString filename)
{
	//1. JPG 파일 로딩
	OnRgbtohsiChange(filename);
	
	//2. 특징 추출
	
	double* feature = new double[FULL_FEAT_DIM];
	std::fill_n(feature, FULL_FEAT_DIM, 0);
	
	int heightGridFlag = hsiHeight / sqrt(GRID_NUM);
	int widthGridFlag = hsiWidth / sqrt(GRID_NUM);

	int wIndex = 0;
	int hIndex = 0;

	// color 특징 추출
	for (int i = 0; i < imgHeight; i++) {
		hIndex = (i / heightGridFlag) * COLOR_DIV;
		for (int j = 0; j < imgWidth; j++) {
			wIndex = (j / widthGridFlag) * COLOR_DIV;
			feature[hIndex + wIndex + (int)(hueBuffer[i][j] / 20)] +=2;
			//feature[hIndex + wIndex + (int)(satuBuffer[i][j] / 20)]+=1;
			feature[hIndex + wIndex + (int)(intenBuffer[i][j]/ 20)] +=2;
		}
	}
	// texture 특징 추출
	wIndex = 0;
	hIndex = 0;
	int textWIndex = 0;
	int textHIndex = 30;

	int textWIndex2 = 30;
	int textHIndex2 = 0;

	int textWIndex3 = 30;
	int textHIndex3 = 30;

	for (int i = 0; i < imgHeight; i++) {
		hIndex = (i / heightGridFlag) * COLOR_DIV;
		for (int j = 0; j < imgWidth; j++) {
			if((i + textHIndex) <imgHeight && (j + textWIndex) <imgWidth){
				wIndex = (j / widthGridFlag) * COLOR_DIV;
				int temp = (int)(intenBuffer[i][j] / 20);
				feature[hIndex + wIndex + (COLOR_DIV*GRID_NUM) + (temp*COLOR_DIV) + (int)(intenBuffer[i + textHIndex][j + textWIndex] / 20)] +=1;
				temp = (int)(hueBuffer[i][j] / 20);
				feature[hIndex + wIndex + (COLOR_DIV*GRID_NUM) + (temp*COLOR_DIV) + (int)(hueBuffer[i + textHIndex][j + textWIndex] / 20)] +=3;
			}
			if ((i + textHIndex2) < imgHeight && (j + textWIndex2) < imgWidth) {
				int temp = (int)(intenBuffer[i][j] / 20);
				feature[hIndex + wIndex + (COLOR_DIV*GRID_NUM) + (temp*COLOR_DIV) + (int)(intenBuffer[i + textHIndex2][j + textWIndex2] / 20)]+=1;
				temp = (int)(hueBuffer[i][j] / 20);
				feature[hIndex + wIndex + (COLOR_DIV*GRID_NUM) + (temp*COLOR_DIV) + (int)(hueBuffer[i + textHIndex2][j + textWIndex2] / 20)]+=3;
			}
			if ((i + textHIndex3) < imgHeight && (j + textWIndex3) < imgWidth) {
				int temp = (int)(intenBuffer[i][j] / 20);
				feature[hIndex + wIndex + (COLOR_DIV*GRID_NUM) + (temp*COLOR_DIV) + (int)(intenBuffer[i + textHIndex3][j + textWIndex3] / 20)]+=1;
				temp = (int)(hueBuffer[i][j] / 20);
				feature[hIndex + wIndex + (COLOR_DIV*GRID_NUM) + (temp*COLOR_DIV) + (int)(hueBuffer[i + textHIndex3][j + textWIndex3] / 20)]+=3;
			}
		}
	}

	return feature;
}

HBITMAP CSimpleImageRetrievalDlg::convertIplImage2HBITMAPWithComment(IplImage* origin, CRect wrect, CString comment)
{
	IplImage *image = cvCreateImage(cvSize(wrect.Width(), wrect.Height()), origin->depth, origin->nChannels);
	cvResize(origin, image);

	CvFont font;
	double hScale = 0.5f;
	double vScale = 0.5f;
	int    lineWidth = 2;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth);
	cvPutText(image, comment, cvPoint(10, 20), &font, cvScalar(255, 255, 255));

	int bpp = image->nChannels * 8;
	assert(image->width >= 0 && image->height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	HBITMAP hbmp = NULL;
	unsigned char buffer[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	ZeroMemory(bmih, sizeof(BITMAPINFOHEADER));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = image->width;
	bmih->biHeight = image->origin ? abs(image->height) : -abs(image->height);
	bmih->biPlanes = 1;
	bmih->biBitCount = bpp;
	bmih->biCompression = BI_RGB;

	if (bpp == 8) {
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++) {
			palette[i].rgbRed = palette[i].rgbGreen = palette[i].rgbBlue = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	void* dst_ptr = 0;
	hbmp = CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, (void **)&dst_ptr, NULL, 0);
	if (image->nChannels == 1)
		SetBitmapBits(hbmp, image->imageSize, image->imageData);
	else {
		CvMat dst;
		cvInitMatHeader(&dst, image->height, image->width, CV_8UC3, dst_ptr, (image->width * image->nChannels + 3) & -4);
		cvConvertImage(image, &dst, image->origin ? CV_CVTIMG_FLIP : 0);
	}


	cvReleaseImage(&image);
	cvReleaseImage(&origin);
	return hbmp;
}



HBITMAP CSimpleImageRetrievalDlg::convertIplImage2HBITMAP(IplImage* origin, CRect wrect)
{
	IplImage *image = cvCreateImage(cvSize(wrect.Width(), wrect.Height()), origin->depth, origin->nChannels);
	cvResize(origin, image);

	int bpp = image->nChannels * 8;
	assert(image->width >= 0 && image->height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	HBITMAP hbmp = NULL;
	unsigned char buffer[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	ZeroMemory(bmih, sizeof(BITMAPINFOHEADER));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = image->width;
	bmih->biHeight = image->origin ? abs(image->height) : -abs(image->height);
	bmih->biPlanes = 1;
	bmih->biBitCount = bpp;
	bmih->biCompression = BI_RGB;

	if (bpp == 8) {
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++) {
			palette[i].rgbRed = palette[i].rgbGreen = palette[i].rgbBlue = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	void* dst_ptr = 0;
	hbmp = CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, (void **)&dst_ptr, NULL, 0);
	if (image->nChannels == 1)
		SetBitmapBits(hbmp, image->imageSize, image->imageData);
	else {
		CvMat dst;
		cvInitMatHeader(&dst, image->height, image->width, CV_8UC3, dst_ptr, (image->width * image->nChannels + 3) & -4);
		cvConvertImage(image, &dst, image->origin ? CV_CVTIMG_FLIP : 0);
	}

	cvReleaseImage(&image);
	cvReleaseImage(&origin);
	return hbmp;
}

//*******************  Cosaine Similarity	******************* //
double CSimpleImageRetrievalDlg::getCosineSimilarity(double* a, double* b) {
	float dA = 0.0;
	float dB = 0.0;
	float pAB = 0.0;;
	for (int i = 0; i < FULL_FEAT_DIM; i++) {
		dA += (float)(a[i] * a[i]);
		dB += (float)(b[i] * b[i]);
		pAB += (float)(a[i] * b[i]);
	}
	dA = sqrt(dA);	dB = sqrt(dB);
	//printf("%f\n", pAB / (dA*dB));
	return pAB / (dA*dB);
}






//*******************  add jpeg load function	******************* //

/*
******** JPEG 이미지 로드 ******
*/
BYTE * LoadJpegFromOpenFile(FILE *fp, BITMAPINFOHEADER *pbh, UINT *pWidth, UINT *pHeight) {
	//파일로부터 JPG 정보 획득
	if (pWidth == NULL || pHeight == NULL) return NULL;
	BYTE *tmp = JpegFile::OpenFileToRGB(fp, pWidth, pHeight);
	if (!tmp) return NULL;
	JpegFile::BGRFromRGB(tmp, *pWidth, *pHeight);
	UINT dw;
	BYTE *pbuf = JpegFile::MakeDwordAlignedBuf(tmp, *pWidth, *pHeight, &dw);
	delete[] tmp;
	if (!pbuf) return NULL;
	JpegFile::VertFlipBuf(pbuf, dw, *pHeight);
	pbh->biSize = sizeof(BITMAPINFOHEADER);
	pbh->biWidth = *pWidth;
	pbh->biHeight = *pHeight;
	pbh->biPlanes = 1;
	pbh->biBitCount = 24;
	pbh->biCompression = BI_RGB;
	pbh->biSizeImage = 0;
	pbh->biXPelsPerMeter = 0;
	pbh->biYPelsPerMeter = 0;
	pbh->biClrUsed = 0;
	pbh->biClrImportant = 0;
	return pbuf;
}

void CSimpleImageRetrievalDlg::OnLoadJpeg(CString filename)
{

	FILE *fp = fopen(filename, "rb");
	BITMAPINFOHEADER pbh;
	UINT w, h;
	BYTE *pbuf = LoadJpegFromOpenFile(fp, &pbh, &w, &h);
	imgWidth = (int)w;
	imgHeight = (int)h;
	//BMP로드때와 마찬가지로, rgbBuffer를 이미지 사이즈 만큼 메모리 할당 코드 추가하기
	rgbBuffer = new RGBQUAD*[imgHeight];
	for (int i = 0; i<imgHeight; i++)
		rgbBuffer[i] = new RGBQUAD[imgWidth];

	int dw = WIDTHBYTES(imgWidth * 24);
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			rgbBuffer[imgHeight - i - 1][j].rgbBlue = pbuf[i*dw + j * 3 + 0];
			rgbBuffer[imgHeight - i - 1][j].rgbGreen = pbuf[i*dw + j * 3 + 1];
			rgbBuffer[imgHeight - i - 1][j].rgbRed = pbuf[i*dw + j * 3 + 2];
		}
	}
	delete[] pbuf;
	fclose(fp);


}

/*
******** RGV to HSI 이미지 로드 ******		이 함수가 이상해
*/
void CSimpleImageRetrievalDlg::OnRgbtohsiChange(CString filename)
{



	OnLoadJpeg(filename); //rgbBuffer에 데이터가 없는 경우, 로드 함수를 호출하여 이미지 획득


					  //2. 변수 메모리 제거 및 할당
	if (hueBuffer != NULL) {
		for (int i = 0; i < hsiHeight; i++) {
			delete[] hueBuffer[i];
		}
		delete[] hueBuffer;
	}
	if (satuBuffer != NULL) {
		for (int i = 0; i < hsiHeight; i++) {
			delete[] satuBuffer[i];
		}
		delete[] satuBuffer;
	}
	if (intenBuffer != NULL) {
		for (int i = 0; i < hsiHeight; i++) {
			delete[] intenBuffer[i];
		}
		delete[] intenBuffer;
	}

	hueBuffer = new float*[imgHeight];
	satuBuffer = new float*[imgHeight];
	intenBuffer = new float*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		hueBuffer[i] = new float[imgWidth];
		satuBuffer[i] = new float[imgWidth];
		intenBuffer[i] = new float[imgWidth];
	}
	hsiHeight = imgHeight;
	hsiWidth = imgWidth;

	//3. RGB to HSI 값 변환
	for (int i = 0; i<imgHeight; i++) {
		for (int j = 0; j<imgWidth; j++) {
			float r = rgbBuffer[i][j].rgbRed;
			float g = rgbBuffer[i][j].rgbGreen;
			float b = rgbBuffer[i][j].rgbBlue;
			intenBuffer[i][j] = (r + g + b) / (float)(3 * 255); //intensity
			float total = r + g + b;
			r = r / total; g = g / total; b = b / total;
			satuBuffer[i][j] = 1 - 3 * (r>g ? (g>b ? b : g) : (r>b ? b : r));
			if (r == g && g == b) {
				hueBuffer[i][j] = 0; satuBuffer[i][j] = 0;
			}
			else {
				total = (0.5*(r - g + r - b) / sqrt((r - g)*(r - g) + (r - b)*(g - b)));
				hueBuffer[i][j] = acos((double)total);
				if (b > g) {
					hueBuffer[i][j] = 6.28 - hueBuffer[i][j];
				}
			}
		}
	}

	//4. 출력 값 범위 정규화 : 출력 시, 값의 범위를[0, 255]로 맞춰줌
	for (int i = 0; i<imgHeight; i++) {
		for (int j = 0; j<imgWidth; j++) {
			hueBuffer[i][j] = hueBuffer[i][j] * 255 / (3.14 * 2);
			satuBuffer[i][j] = satuBuffer[i][j] * 255;
			intenBuffer[i][j] = intenBuffer[i][j] * 255;
			if (hueBuffer[i][j] < 0)
				hueBuffer[i][j] = 0;
			else if (hueBuffer[i][j] > 255)
				hueBuffer[i][j] = 255;
		}
	}
	
}
