// This is the main DLL file.

#include "stdafx.h"
#include"guids.h"
#include "Filter.h"

EXTERN_C const GUID CLSID_RendererSkelet;

CFilter::CFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr) :
    CBaseRenderer(CLSID_RendererSkelet, tszName, punk, phr)
{
}
CFilter::~CFilter()
{
}

CUnknown * WINAPI CFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
	//ASSERT(phr);    
	/*CFilter *pNewObject = new CFilter(NAME("fv Add Graph To ROT Sample Filter"), punk, phr);
    if (pNewObject == NULL) {
        if (phr)
            *phr = E_OUTOFMEMORY;
    }

    return pNewObject;*/
	return NULL;
}

HRESULT CFilter::CheckMediaType(const CMediaType *pmt)
{
	return NOERROR;
}

HRESULT CFilter::DoRenderSample(IMediaSample *pMediaSample)
{
	return NOERROR;
}
