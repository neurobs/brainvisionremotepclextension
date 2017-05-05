// Type1.h : Declaration of the CType1

#pragma once
#include "resource.h"       // main symbols

#include "pcl_extension_template_i.h"
#include "PCLExtension.h"
#include "pcl_extension_interface_wrappers.h"
#include <vector>
#include "tcp_client.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CType1

/// ------- Template Notes -------
/// This is an example IPCLObject implementation which you may reuse.
/// It functions in conjuction with the CType1Type class
class ATL_NO_VTABLE CType1 :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CType1, &CLSID_Type1>,
	public IDispatchImpl<IPCLObject, &__uuidof(IPCLObject), &LIBID_PCLExtension>
{
public:
	CType1();

	DECLARE_REGISTRY_RESOURCEID(IDR_TYPE1)

	BEGIN_COM_MAP(CType1)
		COM_INTERFACE_ENTRY(IPCLObject)
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
	// IPCLObject Methods
	STDMETHOD(callMethod)(long methodIndex, IPCLArguments * arguments);
	STDMETHOD(getType)(IPCLType* * type);
	STDMETHOD(getLastError)(BSTR * message);

	public_nbs::COM_ptr<IPCLType> type_;

private:
	std::wstring last_error_;

	typedef void (CType1::*Method_call)( public_nbs::pcl_extension::Arguments args );
	static std::vector<Method_call> methods_;
	static std::vector<Method_call> operators_new_;
	friend class CType1Type;

	/// ------- Template TODO -------
	/// Add your method or operator new initialization method 
	/// implementations for this type here
	void operator_new(public_nbs::pcl_extension::Arguments args);

	void open_recorder( public_nbs::pcl_extension::Arguments args );
	void close_recorder( public_nbs::pcl_extension::Arguments args );

	void set_impedance_check_mode(public_nbs::pcl_extension::Arguments args);
	void set_monitoring_mode(public_nbs::pcl_extension::Arguments args);

	void start_recording( public_nbs::pcl_extension::Arguments args );
	void pause_recording( public_nbs::pcl_extension::Arguments args );
	void resume_recording( public_nbs::pcl_extension::Arguments args );
	void stop_recording( public_nbs::pcl_extension::Arguments args );
	void dc_reset( public_nbs::pcl_extension::Arguments args );
	void send_raw_message(public_nbs::pcl_extension::Arguments args);
	
	/// ------- Template TODO -------
	/// Add your object data here
	TCP_client connection_;

	void send(const std::string& s);

	static const char* const PORT;
	static const int SEND_TIME_OUT_MS = 1000;
};

OBJECT_ENTRY_AUTO(__uuidof(Type1), CType1)
