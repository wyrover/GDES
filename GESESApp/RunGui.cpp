#include <QtGui>
#include   "RunGui.h"
#include "CADHelper.h"
#include "RegDlg.h"
#include <qt_windows.h>

//#include <afxwin.h>
RunGui :: RunGui ()
{
	initDialog();
	//myTimer  =  new   QTimer (  this  );
	//connect (  myTimer ,  SIGNAL (  timeout () ),  this ,  SLOT (  updateTime () ) );
	//myTimer -> start (1000);
}

RunGui ::~ RunGui ()
{
}



void RunGui::setBtnStyle()
{

	if(m_registed) 
	{
		startNullBtn->setStyleSheet("\
									QPushButton{\
									border:none;\
									background:url(:/images/autoCAD.png);}\
									QPushButton:hover{\
									border:none;\
									background:url(:/images/startHover.png);}\
									background:url(:/images/autoCAD.png);}\
									}");

		startFileBtn->setStyleSheet("\
									QPushButton{\
									border:none;\
									background:url(:/images/open.png);}\
									QPushButton:hover{\
									border:none;\
									background:url(:/images/openHover.png);}\
									}");

		helpBtn->setStyleSheet("\
							   QPushButton{\
							   border:none;\
							   background:url(:/images/unlocked.png);}\
							   QPushButton:hover{\
							   border:none;\
							   background:url(:/images/ok.png);}\
							   }");
	}

	else
	{
		startNullBtn->setStyleSheet("\
									QPushButton{\
									border:none;\
									background:url(:/images/autoCAD1.png);}\
									QPushButton:hover{\
									border:none;\
									background:url(:/images/startHover1.png);}\
									background:url(:/images/autoCAD.png);}\
									}");

		startFileBtn->setStyleSheet("\
									QPushButton{\
									border:none;\
									background:url(:/images/open1.png);}\
									QPushButton:hover{\
									border:none;\
									background:url(:/images/openHover1.png);}\
									}");

		helpBtn->setStyleSheet("\
							   QPushButton{\
							   border:none;\
							   background:url(:/images/help.png);}\
							   QPushButton:hover{\
							   border:none;\
							   background:url(:/images/helpHover.png);}\
							   }");
	}

	//getTime();
	//windTitle = QString(tr("WindowsTitle"));
	//windTitle.append("(");
	//windTitle.append(sTime);
	//windTitle.append(")");
	//windTitle.append("     ");
	//windTitle.append(regMsgTitle);
	//setWindowTitle(windTitle);
}

void RunGui::initDialog()
{
	regHelper = new RegHelper;
	m_registed = false;
	cheakRegCode();
	QString windTitle = QString(tr("WindowsTitle"));
	setWindowTitle(windTitle);

	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	setFixedSize(830,550);
	startNullBtn = new QPushButton();
	startFileBtn = new QPushButton();
	helpBtn = new QPushButton();
	startNullBtn->setGeometry(QRect(580, 490, 50, 44));
	startFileBtn->setGeometry(QRect(660, 490, 50, 44));
	helpBtn->setGeometry(QRect(740,490,50,50));

	startNullBtn->setToolTip(tr("startNullButton"));
	startFileBtn->setToolTip(tr("startFileButton"));
	helpBtn->setToolTip(tr("help"));

	setBtnStyle();

	startNullBtn->setParent(this);
	startFileBtn->setParent(this);
	helpBtn->setParent(this);

	connect(startNullBtn,SIGNAL(clicked()),this,SLOT(startCADNull()));
	connect(startFileBtn,SIGNAL(clicked()),this,SLOT(startCADByFile()));
	connect(helpBtn,SIGNAL(clicked()),this,SLOT(about()));
}

void   RunGui :: paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/images/background"));
}

void RunGui::startCADNull()
{
	cheakRegCode();

	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("tip"));
	if (!m_registed)
	{
		msgBox.setText(tr("Has not registered!"));
		msgBox.exec();
		return;
	}

	QStringList arguments;
	QString cadPath;
	if (!isAutoCADExist(cadPath,arguments)) return;
	cadPath.append("\\acad.exe");
	QProcess *myProcess = new QProcess;
	myProcess->start(cadPath, arguments);
	//myProcess->execute( cadPath,arguments );
	//AutoCAD启动之后界面自动隐藏，关闭AuToCAD之后又显示
	this->hide();
	while(1)
	{
		if(myProcess->waitForFinished()) break;
	}
	this->show();
}

void RunGui::startCADByFile()
{
	cheakRegCode();

	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("tip"));
	if (!m_registed)
	{
		msgBox.setText(tr("Has not registered!"));
		msgBox.exec();
		return;
	}

	QDir dir;

	//conf.ini保存上次打开文件的路径
	QSettings *openFilePath = new QSettings(QDir(QApplication::applicationDirPath()).absoluteFilePath("conf.ini"), QSettings::IniFormat);
	QString openFileType;
	openFileType.append(tr("CAD Files"));
	openFileType.append("(*.dwg *.dxf);;");
	openFileType.append(tr("All File "));
	openFileType.append("(*.*)");
	QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), openFilePath->value("path").toString(), openFileType);
	if (fileName.isEmpty()) return;
	QStringList arguments;
	QString cadPath;
	arguments << fileName << "motif";
	if (!isAutoCADExist(cadPath,arguments)) return;
	openFilePath->setValue("path",fileName);
	//qDebug() << fileName;
	cadPath.append("\\acad.exe");
	QProcess *myProcess = new QProcess;
	myProcess->start(cadPath, arguments);
	this->hide();
	while(1)
	{
		if(myProcess->waitForFinished()) break;
	}
	this->show();
}

void RunGui::about()
{
	cheakRegCode();

	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("tip"));
	if (m_registed)
	{
		msgBox.setText(tr("Has been successfully registered!"));
		msgBox.exec();
	}

	else 
	{
		if ( !ShowRegDlg() )
		{
			m_registed = false;
		}
		else
		{
			m_registed = true;
		}
	}
	setBtnStyle();
}

void RunGui::getTime()
{
	QDateTime dateTime = QDateTime::currentDateTime();
	int y=dateTime.date().year();
	int m=dateTime.date().month();
	int d=dateTime.date().day();
	sTime = (QString::number(y)+"-"+QString::number(m)+"-"+QString::number(d));
}

void RunGui::updateTime()
{
	//usedTime += 1;
	//getTime();
	//timeLabel->setText(sTime);

	// 	sUsedTime = QString::number(usedTime);
	// 	usedTimeLabel->setText(sUsedTime);
}

void RunGui::closeEvent( QCloseEvent *event )
{
	if (0 == FindProcess("acad.exe"))
	{
		QMessageBox msg;
		msg.setWindowTitle(QObject::tr("reminder"));
		msg.setText(QObject::tr("CAD Process is running"));
		msg.exec();
		event->ignore();
		return;
	}
	deleteReg();
	recorverCadFile();
	QDialog::closeEvent(event);
}

//检测系统是否注册
//在系统启动时，打开CAD文件，启动CAD，以及注册前后都调用该函数
void RunGui::cheakRegCode()
{
	m_nErrorCode = InitRegProc();

	if( 0 == m_nErrorCode )
	{
		m_registed = true;
	}
}

UINT RunGui::InitRegProc()
{
	memset( m_chWinPath, '\0', MAX_BUFF );
	GetWindowsDirectory( m_chWinPath, MAX_BUFF );     // 获取系统目录

	strcat( m_chWinPath, "\\softwarwreg.ini" );          // 连接ini文件名

	if ( IsErrorOne() )                               // 是否是未注册等...
	{
		return 1;
	}

	if ( IsErrorTwo() )                               // 硬编码与用户名不符
	{
		return 2;
	}

	return 0;                                         // 已经注册
}

BOOL RunGui   ::IsErrorOne()
{
	//----------------------------------- 情况一 -------------------------------//

	// 如果注册表中无此键值或ini文件不存在
	if ( ( !m_regOpt.OpenRegTable( HKEY_CURRENT_USER, "Software\\JLRegInfo" ) ) )  
	{
		m_regOpt.CloseRegTable();                      // 关闭注册表
		return TRUE;                                   // 1: 未注册...
	}

	// 如果键下无值
	if ( !m_regOpt.ReadRegTable( "JL", (LPBYTE)&m_nRegUTime ) ) 
	{
		m_regOpt.CloseRegTable();
		DeleteFile( m_chWinPath );
		RegDeleteKey( HKEY_CURRENT_USER, "software\\JLRegInfo" );

		return TRUE;
	}

	return FALSE;

	//----------------------------------- 情况一 -------------------------------//
}

BOOL RunGui   ::IsErrorTwo()
{
	//----------------------------------- 情况二 -------------------------------//

	// 获取硬件信息
	if ( !m_hWare.GetHDNum( &m_dInfo ) )
	{
		m_hWare.GetBiosNum( &m_dInfo );
	}

	// 把硬编码转换成数字ID
	m_strUID = m_reg.DisPoseID( m_dInfo.szSystemInfo );

	return FALSE;

	//----------------------------------- 情况二 -------------------------------//
}

BOOL RunGui   ::IsErrorThree()
{
	//----------------------------------- 情况三 -------------------------------//

	m_nIniUTime = GetProfileInt( "Init", "Time", -1 );
	m_nIniSelect = GetProfileInt( "Init", "Select", -1 );

	// 如果ini文件中的使用次数或选择次数的索引值为空
	if ( -1 == m_nIniUTime || -1 == m_nIniSelect )
	{
		if ( m_regOpt.RegTbIsOpen() )
		{
			m_regOpt.CloseRegTable();
		}
		DeleteFile( m_chWinPath );
		RegDeleteKey( HKEY_CURRENT_USER, "software\\JLRegInfo" );
		return TRUE;
	}

	// 解密读出的值
	m_nRegUTime = m_reg.UnJmValue( m_nRegUTime, m_nIniSelect );

	// 如果解密出来的剩余使用次数不等于ini文件中的剩余使用次数
	if ( m_nRegUTime != m_nIniUTime )
	{
		if ( m_regOpt.RegTbIsOpen() )
		{
			m_regOpt.CloseRegTable();
		}
		DeleteFile( m_chWinPath );
		RegDeleteKey( HKEY_CURRENT_USER, "software\\JLRegInfo" );
		return TRUE;
	}
	return FALSE;

	//----------------------------------- 情况三 -------------------------------//
}

BOOL RunGui   ::IsErrorFour()
{
	//----------------------------------- 情况四 -------------------------------//

	if ( m_nIniUTime <= 0 )                                     // 如果使用次数已过
	{	
		if ( m_regOpt.RegTbIsOpen() )
		{
			m_regOpt.CloseRegTable();
		}
		RegDeleteKey( HKEY_CURRENT_USER, "software\\JLRegInfo" );   // 清除注册表中的数据
		DeleteFile( m_chWinPath );                              // 清除ini文件
		return TRUE;
	}
	return FALSE;

	//----------------------------------- 情况四 -------------------------------//
}

// 返回值为真、注册成功，否则注册失败
bool RunGui::ShowRegDlg()
{
	int       nIndex = 4;        // 存选择次数的索引值
	int       nJmTime = 0;       // 用于写入注册表中时使用
	int       nIniTime = 0;      // 用于写入ini文件时使用

	// 弹出相应注册信息
	switch ( m_nErrorCode )
	{

	case 1:
		break;
	case 2:
		MessageBox( NULL, "一个注册码只能用在一台计算机上", "错误提示",
			MB_OK | MB_ICONINFORMATION );
		break;
	}

	// 写入ini文件和注册表
	if(!m_registed)
	{
		RegDlg *regDlg = new RegDlg;
		regDlg->exec();
		if(regDlg->regSucs)
		{
			nIndex = 4;              // 保存选择次数的索引值

			WriteProfileString( "Init", "userID",regDlg->regID.toStdString().c_str() );
			WriteProfileString( "Init", "Register", regDlg->machineID.toStdString().c_str() );
			//WriteProfileInt( "Init", "Select", 4 );
			m_registed = true;
		}
		else
		{
			return false;
		}
	}

	nIniTime = 255;
	// 把使用次数写入INI文件
	nJmTime = m_reg.JmValue( nIniTime, nIndex );

	// 把加密次数写入注册表文件
	m_regOpt.CreateRegKey( HKEY_CURRENT_USER, "Software\\JLRegInfo" );
	m_regOpt.WriteRegTable( "JL", REG_DWORD, nJmTime );
	m_regOpt.CloseRegTable();

	m_bFirstReg = true;    // 是第一次运行本软件

	return true;
}
