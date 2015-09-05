#include "StdAfx.h"
#include "CustomProperties.h"

SyncFunctor::SyncFunctor(AcPropertyArray& pProps) : m_pProps(pProps)
{
}

void CustomGridProperty::setSyncFun(SyncFunctor* fn)
{
	m_fn = fn;
}

CustomGridProperty::CustomGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr)
	:CMFCPropertyGridProperty( strName, varValue, lpszDescr ), m_bHighlight(false)
{
	m_valueRect.SetRectEmpty();
	m_nameRect.SetRectEmpty();
	m_fn = 0;
}

CustomGridProperty::~CustomGridProperty()
{
	if(m_fn)
	{
		delete m_fn;
	}
}

void CustomGridProperty::OnDrawName(CDC* pDC, CRect rect)
{
	//�������Ծ��ο��С
	m_nameRect = rect;

	COLORREF clrBackground, clrText, clrGroupBackground, clrGroupText, clrDescriptionBackground, clrDescriptionText, clrLine;
	m_pWndList->GetCustomColors(clrBackground, clrText, clrGroupBackground, clrGroupText, clrDescriptionBackground, clrDescriptionText, clrLine);

	//if(m_bHighlight)
	//{
	//	m_pWndList->SetCustomColors( RGB(255,0,0), RGB(255,0,0), clrGroupBackground, RGB(255,0,0), clrDescriptionBackground, clrDescriptionText, clrLine);
	//}

	CMFCPropertyGridProperty::OnDrawName(pDC, rect);

	//if(m_bHighlight)
	//{
	//	m_pWndList->SetCustomColors(clrBackground, clrText, clrGroupBackground, clrGroupText, clrDescriptionBackground, clrDescriptionText, clrLine);
	//}
}

void CustomGridProperty::OnDrawValue(CDC* pDC, CRect rect)
{
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(pDC);

	//�������Ծ��ο��С
	m_valueRect = rect;

	//���û����OnDrawValue
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);

	//���Ƹ���Ч��
	if(m_bHighlight)
	{
		CRect rectFill = rect;
		//��䱳��ɫ
		//CBrush br(RGB(135, 206, 206));
		//pDC->FillRect(rectFill, &br);

		//�޸�������ɫ
		COLORREF clrTextOld = pDC->GetTextColor();
		CString strVal = FormatProperty();
		rect.DeflateRect(AFX_TEXT_MARGIN, 0);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->DrawText(strVal, rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);
		pDC->SetTextColor(clrTextOld);
	}
}

static void ChangeDatas(CMFCPropertyGridCtrl* propertyDataList,CustomGridProperty* pProp, COleVariant& changedValue)
{
	int groupCount = propertyDataList->GetPropertyCount();
	for( int i = 0; i < groupCount; i++)
	{
		CMFCPropertyGridProperty* pGroup = propertyDataList->GetProperty(i);
		int propCount = pGroup->GetSubItemsCount();
		//�������о�����ͬ���ֵ��ֶΣ����Ұ�ֵ�޸���
		for( int j = 0; j < propCount; j++ )
		{
			CMFCPropertyGridProperty* pPro = pGroup->GetSubItem(j);
			CString changedName = pPro->GetName();
			CString changeName = pProp->GetName();
			if (changedName == changeName && pPro != pProp)
			{
				pPro->SetValue(changedValue);
			}
		}
	}
}
BOOL CustomGridProperty::OnDblClk(CPoint point)
{
	ASSERT_VALID(m_pWndList);

	//acutPrintf(_T("\nx:%d, y:%d, widht:%d, height:%d"), m_valueRect.left, m_valueRect.bottom, m_valueRect.Width(), m_valueRect.Height());
	//acutPrintf(_T("\npx:%d, py:%d"), point.x, point.y);
	if (m_bEnabled && m_valueRect.PtInRect(point))
	{
		if(m_fn != 0 && m_varValue.vt == VT_R8)
		{
			COleVariant v(m_fn->caculate());
			this->SetValue(v);
			ChangeDatas(m_pWndList,this,v);
			m_pWndList->InvalidateRect(m_valueRect);
		}
	}
	return CMFCPropertyGridProperty::OnDblClk(point);
}

void CustomGridProperty::highlight(bool bHighlight /*= true*/)
{
	m_bHighlight = bHighlight;
	Redraw();
}

//COleVariant(short nSrc, VARTYPE vtSrc = VT_I2);
//COleVariant(long lSrc, VARTYPE vtSrc = VT_I4);
//COleVariant(double dblSrc);
//COleVariant(const COleDateTime& timeSrc);

// �ж��ַ����Ƿ���һ���Ϸ���������
// ��ʽ[+/-][0-9]
static bool IsInteger( LPCTSTR pSrc )
{
    if( *pSrc == _T( '+' ) || *pSrc == _T( '-' ) ) pSrc++;
    for( ; *pSrc != _T( '\0' ) && _istdigit( *pSrc ); pSrc++ );
    return ( *pSrc == _T( '\0' ) );
}

// �ж��ַ����Ƿ���һ���Ϸ��ĸ�������
// ��ʽ[+/-][0-9][.][0-9]
static bool IsNumeric( LPCTSTR pSrc )
{
    if( *pSrc == _T( '+' ) || *pSrc == _T( '-' ) ) pSrc++;
    for( bool bp = false; _istdigit( *pSrc ) || ( *pSrc == _T( '.' ) && bp == false ); )
    {
        if( *pSrc++ == _T( '.' ) ) bp = true;
    }
    return ( *pSrc == _T( '\0' ) );
}

CIntProp::CIntProp( const CString& strName, const COleVariant& varValue, int nMinValue, int nMaxValue, LPCTSTR lpszDescr ) :
    CustomGridProperty( strName, varValue, lpszDescr )
{
    m_nMinValue = nMinValue;
    m_nMaxValue = nMaxValue;
}

BOOL CIntProp::OnUpdateValue()
{
    ASSERT_VALID( this );
    ASSERT_VALID( m_pWndInPlace );
    ASSERT_VALID( m_pWndList );
    ASSERT( ::IsWindow( m_pWndInPlace->GetSafeHwnd() ) );

    BOOL bRet = TRUE;
    CString strText;
    m_pWndInPlace->GetWindowText( strText );

    BOOL bIsChanged = FormatProperty() != strText;
    if ( bIsChanged )
    {
        // �����Ƿ��ַ�����������
        if( !IsInteger( strText ) )
        {
            static BOOL bRecursedHere = FALSE;
            if ( bRecursedHere ) return TRUE;
            bRecursedHere = TRUE;

            CString msg;
            msg.Format( _T( "�����Ƿ��ַ�\n��ȷ��ʽΪ[+/-][0-9]\n[]��ʾ��ѡ\n����-1024, +104, 20" ) );
            AfxMessageBox( msg );

            bRecursedHere = FALSE;
            return FALSE;
        }
        else
        {
            // ת��������
            // ����ʹ��_stscanf_s����,�ο�msdn�Լ�afxpropertygridctrl.cpp��TextVar()�����е�ʵ��
            // msdn����atoi
            int nItem = _ttoi( strText );
            if ( ( nItem < m_nMinValue ) || ( nItem > m_nMaxValue ) )
            {
                static BOOL bRecursedHere = FALSE;
                if ( bRecursedHere ) return TRUE;
                bRecursedHere = TRUE;

                CString strMessage;
                strMessage.Format( _T( "����ȡֵ��Χ��[%d, %d]" ), m_nMinValue, m_nMaxValue );
                AfxMessageBox( strMessage );
                bRecursedHere = FALSE;
                return FALSE;
            }
        }

        bRet = CMFCPropertyGridProperty::OnUpdateValue();

        if ( m_pParent != NULL )
        {
            m_pWndList->OnPropertyChanged( m_pParent );
        }
    }

    return bRet;
}


CNumericProp::CNumericProp( const CString& strName, const COleVariant& varValue, double dMinValue, double dMaxValue, unsigned short precise, LPCTSTR lpszDescr )
    : CustomGridProperty( strName, varValue, lpszDescr )
{
    m_dMinValue = dMinValue;
    m_dMaxValue = dMaxValue;
    m_precise = precise;
}

BOOL CNumericProp::OnUpdateValue()
{
    ASSERT_VALID( this );
    ASSERT_VALID( m_pWndInPlace );
    ASSERT_VALID( m_pWndList );
    ASSERT( ::IsWindow( m_pWndInPlace->GetSafeHwnd() ) );

    BOOL bRet = TRUE;
    CString strText;
    m_pWndInPlace->GetWindowText( strText );

    BOOL bIsChanged = FormatProperty() != strText;
    if ( bIsChanged )
    {
        // ���ǺϷ��ĸ������ַ���
        if( !IsNumeric( strText ) )
        {
            static BOOL bRecursedHere = FALSE;
            if ( bRecursedHere ) return TRUE;
            bRecursedHere = TRUE;

            CString msg;
            msg.Format( _T( "�����Ƿ��ַ�\n��ȷ��ʽΪ[+/-][0-9][.][0-9]\n[]��ʾ��ѡ\n����-10.24, +100.4, 20.5, 0.410" ) );
            AfxMessageBox( msg );

            bRecursedHere = FALSE;
            return FALSE;
        }
        else
        {
            // ת���ɸ�����
            // ����ʹ��_stscanf_s����,�ο�msdn�Լ�afxpropertygridctrl.cpp��TextVar()�����е�ʵ��
            // msdn����atof, �鿴����"How to convert a string to a floating-point number in C++"
            // ��atof��sscanf�����м򵥵ĶԱ�˵��
            double d = _tstof( strText );
            if ( ( d < m_dMinValue ) || ( d > m_dMaxValue ) )
            {
                static BOOL bRecursedHere = FALSE;
                if ( bRecursedHere ) return TRUE;
                bRecursedHere = TRUE;

                CString msg;
                msg.Format( _T( "����ȡֵ��Χ��[%f, %f]" ), m_dMinValue, m_dMaxValue );
                AfxMessageBox( msg );

                bRecursedHere = FALSE;
                return FALSE;
            }
        }

        bRet = CMFCPropertyGridProperty::OnUpdateValue();

        if ( m_pParent != NULL )
        {
            m_pWndList->OnPropertyChanged( m_pParent );
        }
    }

    return bRet;
}

CString CNumericProp::FormatProperty()
{
	ASSERT_VALID( this );
	CString fmt;
	fmt.Format(_T("%s%df"), _T("%."), m_precise);
	CString val;
	val.Format(fmt, (double)GetValue().dblVal);
	//acutPrintf(_T("\nm_precise:%d\tfmt:%s"),m_precise,fmt);
	return val;
}


IntStrProp::IntStrProp( const CString& name, int value, const AcDbIntArray& intValues, const AcStringArray& strValues, LPCTSTR lpszDescr )
    : CustomGridProperty( name, ( long )value, lpszDescr )
{
    m_intValues.append( intValues );
    m_strValues.append( strValues );
    int len = m_strValues.length();
    for( int i = 0; i < len; i++ )
    {
        AddOption( m_strValues[i].kACharPtr() );
    }
}

CString IntStrProp::Int2Str( int intValue )
{
    ASSERT( m_intValues.length() == m_strValues.length() );

    int index = m_intValues.find( intValue );
    if( index < 0 ) index = 0;
    return m_strValues[index].kACharPtr();
}

int IntStrProp::Str2Int( const CString& strValue )
{
    ASSERT( m_intValues.length() == m_strValues.length() );

    int index = m_strValues.find( strValue );
    if( index < 0 ) index = 0;
    return m_intValues[index];
}

CString IntStrProp::FormatProperty()
{
    ASSERT_VALID( this );
    return Int2Str( (int)GetValue().lVal );
}

BOOL IntStrProp::OnUpdateValue()
{
    ASSERT_VALID( this );
	//acutPrintf(_T("\nthisָ���ַ:%ld"),(long)this);
    ASSERT_VALID( m_pWndList );
    ASSERT_VALID( m_pWndInPlace );
    ASSERT( ::IsWindow( m_pWndInPlace->GetSafeHwnd() ) );

	CString strText;
	m_pWndInPlace->GetWindowText( strText );

	BOOL bIsChanged = FormatProperty() != strText;
	if ( bIsChanged )
	{
		// ��������
		SetValue( ( long )Str2Int( strText ) );
		m_pWndList->OnPropertyChanged( this );
	}

    return TRUE;
}

BEGIN_MESSAGE_MAP( MyDateTimeCtrl, CDateTimeCtrl )
    ON_NOTIFY_REFLECT( DTN_DATETIMECHANGE, &MyDateTimeCtrl::OnDtnDatetimechange )
    //ON_NOTIFY_REFLECT(NM_KILLFOCUS, &MyDateTimeCtrl::OnNMKillfocus)
    //ON_NOTIFY_REFLECT(DTN_CLOSEUP, &MyDateTimeCtrl::OnDtnCloseup)
END_MESSAGE_MAP()

void MyDateTimeCtrl::OnDtnDatetimechange( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>( pNMHDR );

    //MessageBox(_T("OnDtnDatetimechange"));
    m_pProp->OnUpdateValue();
    m_pProp->Redraw();
    *pResult = 0;
}

//void MyDateTimeCtrl::OnNMKillfocus(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	//MessageBox(_T("OnNMKillfocus"));
//	*pResult = 0;
//}
//
//void MyDateTimeCtrl::OnDtnCloseup(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	//MessageBox(_T("OnDtnCloseup"));
//	*pResult = 0;
//}

DateTimeProperty::DateTimeProperty( const CString& strName, const COleDateTime& timeSrc, LPCTSTR lpszDescr /*= NULL*/ )
    : CustomGridProperty( strName, ( COleVariant )timeSrc, lpszDescr )
{
    AllowEdit( FALSE );
}

CString DateTimeProperty::FormatProperty()
{
    COleDateTime dt( GetValue().date );
    // ��������˺��֣����ؿմ�
    //return dt.Format(_T("%Y��%B��%d��"));
    //return dt.Format(VAR_DATEVALUEONLY);
    CString str;
    str.Format( _T( "%d��%d��%d��" ), dt.GetYear(), dt.GetMonth(), dt.GetDay() );
    return str;
}

CWnd* DateTimeProperty::CreateInPlaceEdit( CRect rectEdit, BOOL& bDefaultFormat )
{
    MyDateTimeCtrl* pDateTimeCtrl = new MyDateTimeCtrl( this );
    rectEdit.right += 5;
    rectEdit.bottom += 5;
    // ʹ��DTS_LONGDATEFORMAT��ʽ��FormatProperty()��ʵ�ֱ���һ��
    pDateTimeCtrl->Create( WS_VISIBLE | WS_CHILD | DTS_LONGDATEFORMAT, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE );
    pDateTimeCtrl->SetTime( GetValue().date );
    bDefaultFormat = TRUE;
    return pDateTimeCtrl;
}

BOOL DateTimeProperty::OnUpdateValue()
{
    ASSERT_VALID( this );
    ASSERT_VALID( m_pWndInPlace );
    ASSERT_VALID( m_pWndList );
    ASSERT( ::IsWindow( m_pWndInPlace->GetSafeHwnd() ) );

    COleDateTime oldDate( GetValue().date ); // �ɵ�����

    MyDateTimeCtrl* pDateTimeCtrl = ( MyDateTimeCtrl* ) m_pWndInPlace;
    COleDateTime newDate;
    pDateTimeCtrl->GetTime( newDate );
    SetValue( newDate ); // ��������
    //CMFCPropertyGridProperty::OnUpdateValue();

    if ( oldDate != newDate )
    {
        m_pWndList->OnPropertyChanged( this );
    }

    return TRUE;
}

void DateTimeProperty::OnDrawDescription( CDC* pDC, CRect rect )
{
    CDrawingManager dm( *pDC );
    dm.FillGradient2( rect, RGB( 102, 200, 238 ), RGB( 0, 129, 185 ), 45 );

    CFont* pOldFont = pDC->SelectObject( &afxGlobalData.fontBold );

    // 2��DrawText�����ӰЧ��
    CString str;
    str.Format( _T( "%s: %s" ), GetDescription(), FormatProperty() );

    pDC->SetTextColor( RGB( 0, 65, 117 ) );
    pDC->DrawText( str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

    rect.OffsetRect( -2, -2 );

    pDC->SetTextColor( RGB( 155, 251, 255 ) );
    pDC->DrawText( str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

    pDC->SelectObject( pOldFont );
}

DataObjectProperty::DataObjectProperty( const CString& name, const CString& value, const AcStringArray& strValues, LPCTSTR lpszDescr /*= NULL*/ )
    : CustomGridProperty( name, ( COleVariant )value, lpszDescr )
{
    int len = strValues.length();
    for( int i = 0; i < len; i++ )
    {
        AddOption( strValues[i].kACharPtr() );
    }
}