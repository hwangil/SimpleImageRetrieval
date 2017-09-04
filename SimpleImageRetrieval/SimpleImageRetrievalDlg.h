
// SimpleImageRetrievalDlg.h : header file
//

#pragma once
#include "Jpegfile.h"		// Ãß°¡
#include <cstringt.h>
#include <vector>
#include "afxwin.h"
#include "opencv2\opencv.hpp"
using namespace cv;
using namespace std;


#define NUM_DB_FRAMES	10
#define COLOR_DIV	13
#define GRID_NUM	16
#define FULL_FEAT_DIM	(COLOR_DIV*GRID_NUM + COLOR_DIV*COLOR_DIV*GRID_NUM)			// ToDo. color, texture
typedef struct Img {
	CString filename;
	double* feature;
	float distance;
};

// CSimpleImageRetrievalDlg dialog
class CSimpleImageRetrievalDlg : public CDialogEx
{
// Construction 
public:
	CSimpleImageRetrievalDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLEIMAGERETRIEVAL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoadDbimgs();
	afx_msg void OnBnClickedButtonIndexDbimgs();
	afx_msg void OnBnClickedButtonLoadQuery();
	afx_msg void OnBnClickedButtonRankingQuery();
	CStatic* mQueryImg;
	CStatic** mDBImgs;
	CFileFind finder;
	Img queryImg;
	vector<Img> dbImgs;
	double* featureExtraction(CString filename);
	HBITMAP convertIplImage2HBITMAP(IplImage* img, CRect wrect);
	HBITMAP convertIplImage2HBITMAPWithComment(IplImage* origin, CRect wrect, CString comment);
	void CSimpleImageRetrievalDlg::OnLoadJpeg(CString filename);
	void CSimpleImageRetrievalDlg::OnRgbtohsiChange(CString filename);
	double CSimpleImageRetrievalDlg::getCosineSimilarity(double* a, double* b);
};
