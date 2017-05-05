// Type1Type.h : Declaration of the CType1Type

#pragma once
#include "resource.h"       // main symbols

#include "pcl_extension_template_i.h"
#include "PCLExtension.h"
#include "pcl_extension_interface_wrappers.h"
#include <string>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CType1Type

/// ------- Template Notes -------
/// This is an example IPCLType implementation which you may reuse.
/// It functions in conjuction with the CType1 class
class ATL_NO_VTABLE CType1Type :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CType1Type, &CLSID_Type1Type>,
	public IDispatchImpl<IPCLType, &__uuidof(IPCLType), &LIBID_PCLExtension>
{
public:
	CType1Type();

	DECLARE_REGISTRY_RESOURCEID(IDR_TYPE1TYPE)

	BEGIN_COM_MAP(CType1Type)
		COM_INTERFACE_ENTRY(IPCLType)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// IPCLType Methods
	STDMETHOD(getName)(BSTR * name);
	STDMETHOD(getDescription)(BSTR * desc);
	STDMETHOD(getParentName)(BSTR* parent);
	STDMETHOD(getMethods)(IPCLMethodList * list);
	STDMETHOD(createObject)(long index, IPCLArguments* arguments, IPCLObject* * object);
	STDMETHOD(getLibrary)(IPCLLibrary* * lib);
	STDMETHOD(getLastError)(BSTR * message);

	/// ------- Template Notes -------
	/// This is the name of your type
	static const wchar_t* const TYPE_NAME;

	/// ------- Template Notes -------
	/// This is the name of your type with "namespace::" prepended
	static const std::wstring complete_type_name_;

	public_nbs::COM_ptr<IPCLLibrary> library_;

private:
	std::wstring last_error_;
};

OBJECT_ENTRY_AUTO(__uuidof(Type1Type), CType1Type)
