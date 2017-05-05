// PCLLibrary.h : Declaration of the CPCLLibrary

#pragma once
#include "resource.h"       // main symbols

#include "pcl_extension_template_i.h"
#include "pcl_extension_interface_wrappers.h"
#include "PCLExtension.h"
#include <vector>
#include <string>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CPCLLibrary

/// ------- Template Notes -------
/// This is your IPCLLibrary implementation
class ATL_NO_VTABLE CPCLLibrary :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPCLLibrary, &CLSID_PCLLibrary>,
	public IDispatchImpl<IPCLLibrary, &__uuidof(IPCLLibrary), &LIBID_PCLExtension>
{
public:
	CPCLLibrary();

	DECLARE_REGISTRY_RESOURCEID(IDR_PCLLIBRARY)

	BEGIN_COM_MAP(CPCLLibrary)
		COM_INTERFACE_ENTRY(IPCLLibrary)
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
	// IPCLLibrary Methods
	STDMETHOD(getName)(BSTR * name);
	STDMETHOD(getAuthor)(BSTR * author);
	STDMETHOD(getVersion)(BSTR * ver);
	STDMETHOD(getDescription)(BSTR * desc);
	STDMETHOD(getPresentationMinimumVersion)(double* ver);
	STDMETHOD(getNamespaceName)(BSTR * ns);
	STDMETHOD(getTypeCount)(long * count);
	STDMETHOD(getType)(long index, IPCLType * * type);
	STDMETHOD(getLastError)(BSTR * message);

	/// ------- Template Notes -------
	/// This is the namespace for your extension
	static const wchar_t* const NAMESPACE_NAME;

private:
	std::wstring last_error_;
	typedef public_nbs::COM_ptr<IPCLType> (*Type_function)( IPCLLibrary* lib );
	std::vector<Type_function> type_functions_;
};

OBJECT_ENTRY_AUTO(__uuidof(PCLLibrary), CPCLLibrary)
