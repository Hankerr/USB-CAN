#define MBCS_Support_Deprecated_In_MFC
// DemoCANDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DemoCAN.h"
#include "DemoCANDlg.h"
#include "ControlCAN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString sFile;
unsigned long nextrow;
int StopFlag=0;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoCANDlg dialog

CDemoCANDlg::CDemoCANDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoCANDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoCANDlg)
	m_nSendFrameFormat = 0;
	m_nSendFrameType = 0;
	m_strSendData = _T("11 22 33 44 55 66 77 88");
	m_strSendID = _T("00 00 00 88");
	m_radioIDFormat = 1;
	m_bCanRxEn = FALSE;
	m_nCanIndex = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoCANDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoCANDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_CBIndex(pDX, IDC_COMBO_SENDFRAMEFORMAT, m_nSendFrameFormat);
	DDX_CBIndex(pDX, IDC_COMBO_SENDFRAMETYPE, m_nSendFrameType);
	DDX_Text(pDX, IDC_EDIT_SEND_DATA, m_strSendData);
	DDX_Text(pDX, IDC_EDIT_SEND_ID, m_strSendID);
	DDX_Radio(pDX, IDC_RADIO_ID_FORMAT, m_radioIDFormat);
	DDX_Check(pDX, IDC_CHECK_CANRX_EN, m_bCanRxEn);
	DDX_CBIndex(pDX, IDC_COMBO_CAN_INDEX, m_nCanIndex);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemoCANDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoCANDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DEVICE, OnButtonOpenDevice)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_CHECK_CANRX_EN, OnCheckCanrxEn)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_DEVICE, OnButtonCloseDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoCANDlg message handlers

BOOL CDemoCANDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
    //信息显示列表初始化
	m_list.InsertColumn(0,"Seq");
	m_list.SetColumnWidth(0,40);
	m_list.InsertColumn(1,"Time");
	m_list.SetColumnWidth(1,85);
	m_list.InsertColumn(2,"CANIndex");
	m_list.SetColumnWidth(2,60);
	m_list.InsertColumn(3,"Tx-Rx");
	m_list.SetColumnWidth(3,60);
	m_list.InsertColumn(4," ID ");
	m_list.SetColumnWidth(4,60);
	m_list.InsertColumn(5,"Frame");
	m_list.SetColumnWidth(5,50);
	m_list.InsertColumn(6,"Type");
	m_list.SetColumnWidth(6,70);
	m_list.InsertColumn(7,"DLC");
	m_list.SetColumnWidth(7,30);
	m_list.InsertColumn(8,"Data");
	m_list.SetColumnWidth(8,160);
	//信息显示列表初始化完毕

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemoCANDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemoCANDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoCANDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//一位十六进制转换为十进制
int HexChar(char c)
{
	if((c>='0') && (c<='9'))
		return c-0x30;
	else if((c>='A') && (c<='F'))
		return c-'A'+10;
	else if((c>='a') && (c<='f'))
		return c-'a'+10;
	else
		return 0x10;
}
//两位十六进制数转换为十进制
int Str2Hex(CString str)
{	
	int len = str.GetLength();
	if(len == 2)
	{
		int a= HexChar(str[0]);
		int b =HexChar(str[1]);
		if(a==16 || b==16 )
		{
			AfxMessageBox("Format error");
			return 256;
		}
		else
		{
			return a*16+b;
			
		}
		
	}
	else
	{
		AfxMessageBox("input length must be 2");
		return 256;
	}
}
//打开设备
void CDemoCANDlg::OnButtonOpenDevice() 
{

	UpdateData(TRUE);
	m_DevIndex=0;	
	m_DevType=DEV_USBCAN;
	DWORD Reserved=0;
	//打开设备
	if(VCI_OpenDevice(m_DevType,m_DevIndex,Reserved)!=1)
	{
		MessageBox("open failed");
		return;
	}
	VCI_INIT_CONFIG InitInfo[1];
	InitInfo->kCanBaud=15;
	InitInfo->Timing0=0x00;
	InitInfo->Timing1=0x14;
	InitInfo->Filter=0;
	InitInfo->AccCode=0x80000008;
	InitInfo->AccMask=0xFFFFFFFF;
	InitInfo->Mode=0;
	InitInfo->CanRx_IER=1;
    //初始化通道0
	if(VCI_InitCAN(m_DevType,m_DevIndex, 0,InitInfo)!=1)	//can-0
	{
		MessageBox("Init-CAN failed!");
		return;
	}
	Sleep(100);
    //初始化通道1
	if(VCI_StartCAN(m_DevType,m_DevIndex, 0)!=1)	//can-0
	{
		MessageBox("Start-CAN failed!");
		return;
	}

	
	MessageBox("Open successfule!\n Start CAN OK!");	

}
//关闭设备
void CDemoCANDlg::OnButtonCloseDevice() 
{
	if(VCI_CloseDevice(m_DevType,m_DevIndex)!=1)
	{
		MessageBox("Close failed！");
		return;
		
	}
	
	MessageBox("Close successful!");		
}
//发送信息
void CDemoCANDlg::OnButtonSend() 
{


	VCI_CAN_STATUS pCANStatus1[1];
	
	int kk=VCI_ReadCANStatus(2,0,0, pCANStatus1);
	kk=kk+1;

   //从界面获取发送信息
	VCI_CAN_OBJ sendbuf[1];

	UpdateData(TRUE);
	CString str5[32];
	BYTE buf[50];
	BYTE SendID[10];
	CString strtemp,strtemp1;
	CString str1;
	int len,datanum=0,IDnum=0,newflag=1;

	len=m_strSendID.GetLength();	//from dialog
	for(int i=0; i<len;i++)
	{
		strtemp=m_strSendID.GetAt(i);
		if(strtemp==" ")
			newflag=1;
		else if(newflag==1)
		{
			newflag=0;
			strtemp=m_strSendID.GetAt(i);
			if(i==(len-1))
			{
				str5[IDnum]="0"+strtemp;

			}
			else
			{
				strtemp1=m_strSendID.GetAt(i+1);

				if(strtemp1 == " ")
					str5[IDnum]="0"+strtemp;
				else
					str5[IDnum]=strtemp+strtemp1;
			}

			SendID[IDnum]=Str2Hex(str5[IDnum]);
			IDnum++;
			if(IDnum>=4)	
				break;

		}

	}

	newflag=1;
	len=m_strSendData.GetLength();
	int i;
	for(i=0; i<len;i++)
	{
		
		strtemp=m_strSendData.GetAt(i);
		if(strtemp==" ")
			newflag=1;
		else if(newflag==1)
		{	

			newflag=0;
			strtemp=m_strSendData.GetAt(i);		
			if(i==(len-1))
			{
				str5[datanum]="0"+strtemp;

			}
			else
			{
				strtemp1=m_strSendData.GetAt(i+1);

				if(strtemp1 == " ")
				{
					str5[datanum]="0"+strtemp;
				
				}
				else
					str5[datanum]=strtemp+strtemp1;
			
			}
			buf[datanum]=Str2Hex(str5[datanum]);
			datanum++;
			if(datanum>=8)
				break;
		}
	}
	sendbuf->ExternFlag=m_nSendFrameType;
	sendbuf->DataLen=datanum;
	sendbuf->RemoteFlag=m_nSendFrameFormat;
	if(m_nSendFrameFormat==1)//if remote frame, data area is invalid
		for(i=0;i<datanum;i++)
			buf[i]=0;
	BYTE SendID2[4];
	
	if((sendbuf->ExternFlag)==1)//
	{
		if(m_radioIDFormat)//direcet id to SJA1000 format
		{
			SendID[0] &= 0x1F;
			SendID2[0]=(SendID[0]<<3)|(SendID[1]>>5);
			SendID2[1]=(SendID[1]<<3)|(SendID[2]>>5);
			SendID2[2]=(SendID[2]<<3)|(SendID[3]>>5);
			SendID2[3]=(SendID[3]<<3);
			for(i=0;i<4;i++)
				sendbuf->ID[i]=SendID2[i];
		}
		else
		{
			SendID[3]=SendID[3] & 0xF8;//the last 3bit is invalid, clear to 0
			for(i=0;i<4;i++)
				sendbuf->ID[i]=SendID[i];
		}
	}
	else//basic frame ID
	{

		if(m_radioIDFormat)//
		{
			SendID[2] &= 0x07; 
			SendID2[0]=(SendID[2]<<5)|(SendID[3]>>3);
			SendID2[1]=SendID[3]<<5;
			for(i=0;i<2;i++)
				sendbuf->ID[i]=SendID2[i];
		}
		else
		{
			SendID[1]=SendID[1] & 0xE0;//the lower 5 bits is invalid,clear to 0
			for(i=0;i<2;i++)
				sendbuf->ID[i]=SendID[i];
		}
	}

	for(i=0;i<datanum;i++)
		sendbuf->Data[i]=buf[i];
/****************************************************************************/	
/******************************从界面获取发送信息完毕***********************/
/****************************************************************************/
	int flag;

	if((m_nCanIndex==1)&&(m_DevType!=DEV_USBCAN200))
	{
		MessageBox("the device only support CAN index 0");
		m_nCanIndex=0;
	}
    //调用动态链接库发送函数
	flag=VCI_Transmit(m_DevType,m_DevIndex,m_nCanIndex,sendbuf);//CAN message send
	if(flag<1)
	{
		if(flag==-1)
			MessageBox("failed- device not open\n");
		else if(flag==0)
			MessageBox("send error\n");
		return;
	
	}

	CSize size;
	unsigned int JustnowItem;
	BYTE data;
    
    //发送信息列表显示
	CString strTime;
	SYSTEMTIME   systime;   
	GetLocalTime(&systime);   
	strTime.Format("%02d:%02d:%02d:%03d", systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);   

	
	size.cx=0;
	size.cy=50;
	CString str;
	str.Format("%d",nextrow);
	m_list.ItemColorFlag[nextrow]=1;
	JustnowItem=m_list.InsertItem(nextrow,str);	

	nextrow++;	
	m_list.SetItemText(JustnowItem,1,strTime);		
	str.Format("%d",m_nCanIndex);	
	m_list.SetItemText(JustnowItem,2,str);		
	m_list.SetItemText(JustnowItem,3,"Send");		
	str="";

	if((sendbuf->RemoteFlag)==1)
	{
		m_list.SetItemText(JustnowItem,5,"Remote");	
	}
	else
	{
		m_list.SetItemText(JustnowItem,5,"Data");	
	
	}
	if((sendbuf->ExternFlag)==1)			
	{
			for(i=0;i<4;i++)
			{
				data=SendID[i];
				str1.Format("%02X",data);
				str+=str1;
			}
			m_list.SetItemText(JustnowItem,4,str);			
			m_list.SetItemText(JustnowItem,6,"Extended");		//	
	
	}
	else									
	{
			for(i=0;i<2;i++)
			{			
				if(m_radioIDFormat)//direct id to sja1000 format
					data=SendID[i+2];
				else
					data=SendID[i];
				str1.Format("%02X",data);
				str+=str1;
			}
			m_list.SetItemText(JustnowItem,4,str);		
		
			m_list.SetItemText(JustnowItem,6,"Standard");		//
		
		
	}
	str.Format("%d",sendbuf->DataLen);	
	m_list.SetItemText(JustnowItem,7,str);	
	
	str="";
	for(i=0;i<sendbuf->DataLen;i++)		
	{
		data=sendbuf->Data[i];
		str1.Format("%02X",data);
		str+=str1+" ";
	
	}
	m_list.SetItemText(JustnowItem,8,str);	

	m_list.Scroll(size); 
	//发送信息列表显示完毕



		
}


UINT CDemoCANDlg::ReceiveThread(LPVOID v)
{
	CDemoCANDlg *dlg=(CDemoCANDlg*) AfxGetApp()->GetMainWnd();	
	int k=0;
	while(1)
	{
		int NumValue;
		int i;
		VCI_CAN_OBJ pCanObj[200];

		CString strbuf[200],str1;
		int num=0;

		CSize size;
		unsigned int JustnowItem;
		BYTE ReceivedID[4];

		size.cx=0;
		size.cy=50;
		CString str;
		int Len=0;

        //调用动态链接看接收函数
		NumValue=VCI_Receive(dlg->m_DevType,dlg->m_DevIndex,0,pCanObj);
        //接收信息列表显示
		k++;
		CString strTime;
		SYSTEMTIME   systime;   
		GetLocalTime(&systime);   
		strTime.Format("%02d:%02d:%02d:%03d", systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);   

		for(num=0;num<NumValue;num++)
		{
			if(nextrow==59999)
			{
					dlg->m_list.DeleteAllItems();
					nextrow=0;
			}


			dlg->m_list.ItemColorFlag[nextrow]=0;

			str.Format("%d",nextrow);
			JustnowItem=dlg->m_list.InsertItem(nextrow,str);	
			nextrow++;	
			
			dlg->m_list.SetItemText(JustnowItem,1,strTime);	
			
		//	str.Format("%d",nCanIndex);
			dlg->m_list.SetItemText(JustnowItem,2,"0");	
			
			dlg->m_list.SetItemText(JustnowItem,3,"Receive");	
			str="";

			if((pCanObj[num].RemoteFlag)==1)
			{
				dlg->m_list.SetItemText(JustnowItem,5,"Remote");	
			}
			else
			{
				dlg->m_list.SetItemText(JustnowItem,5,"Data");	
			
			}
			if((pCanObj[num].ExternFlag)==1)		
			{
				for(i=0;i<4;i++)
					ReceivedID[i]=pCanObj[num].ID[i];
				if(dlg->m_radioIDFormat)//from sja1000 format to direct id
				{

						ReceivedID[3]=(ReceivedID[2]<<5)|(ReceivedID[3]>>3);
						ReceivedID[2]=(ReceivedID[1]<<5)|(ReceivedID[2]>>3);
						ReceivedID[1]=(ReceivedID[0]<<5)|(ReceivedID[1]>>3);
						ReceivedID[0]=ReceivedID[0]>>3;
				}
				for(i=0;i<4;i++)
				{
						str1.Format("%02X",ReceivedID[i]);
						str+=str1;
				}
				dlg->m_list.SetItemText(JustnowItem,4,str);	//ID信息	
						
				dlg->m_list.SetItemText(JustnowItem,6,"Extended");		//	
					
			}
			else									//标准帧
			{
				for(i=0;i<2;i++)
						ReceivedID[i]=pCanObj[num].ID[i];
				if(dlg->m_radioIDFormat)//将CAN2.0B格式进行转换到直接ID值
				{

						ReceivedID[1]=(ReceivedID[1]>>5)|(ReceivedID[0]<<3);
						ReceivedID[0]=ReceivedID[0]>>5;
				}
				for(i=0;i<2;i++)
				{
					str1.Format("%02X",ReceivedID[i]);
					str+=str1;
				}
				dlg->m_list.SetItemText(JustnowItem,4,str);	//ID信息	
						
				dlg->m_list.SetItemText(JustnowItem,6,"Standard");		//
						
						
			}
			str.Format("%d",pCanObj[num].DataLen);	//长度信息
			dlg->m_list.SetItemText(JustnowItem,7,str);	
					
			str="";
			for(i=0;i<(pCanObj[num].DataLen);i++)			//数据信息
			{
						str1.Format("%02X ",pCanObj[num].Data[i]);
						str+=str1;
					
			}

			dlg->m_list.SetItemText(JustnowItem,8,str);	
			dlg->m_list.Scroll(size); 	
			 //接收信息列表显示完毕

      
		}
		Sleep(10);	


		if(StopFlag==1)
			return 0;	
	}

	return 1;
}

void CDemoCANDlg::OnCheckCanrxEn() 
{
	UpdateData(TRUE);
	if(m_bCanRxEn)
	{
		StopFlag=0;
		//开启接收线程
		AfxBeginThread(ReceiveThread,0);	
	}
	else
		StopFlag=1;

}
//清空信息显示列表
void CDemoCANDlg::OnButtonClear() 
{
	m_list.DeleteAllItems();	
}

/////////////////////////////////////////////////////////////////////////////





