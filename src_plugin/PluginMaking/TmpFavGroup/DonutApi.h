// DonutApi.h: CDonutApi �N���X�̃C���^�[�t�F�C�X
// unDonut��API�@�\�𗘗p���邽�߂̃N���X
// �ʂɑ債�����̂ł͂Ȃ�
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DONUTAPI_H__851C7E72_32C0_4A68_BDEA_C66010DB818E__INCLUDED_)
#define AFX_DONUTAPI_H__851C7E72_32C0_4A68_BDEA_C66010DB818E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDonutApi  
{
public:
	BOOL IsNull();
	IAPI3 *m_pDonut;
	CDonutApi();
	virtual ~CDonutApi();

	operator IAPI3*() const
	{
		return (IAPI3*)m_pDonut;
	}
	IAPI3& operator*() const
	{
		ASSERT(m_pDonut!=NULL);
		return *m_pDonut;
	}

};

#endif // !defined(AFX_DONUTAPI_H__851C7E72_32C0_4A68_BDEA_C66010DB818E__INCLUDED_)
