// dllmain.h : Declaration of module class.

class Cpcl_extension_templateModule : public CAtlDllModuleT< Cpcl_extension_templateModule >
{
public :
	DECLARE_LIBID(LIBID_pcl_extension_templateLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PCL_EXTENSION_TEMPLATE, "{6B5FF927-6205-4A26-8FF2-4A067C3C5486}")
};

extern class Cpcl_extension_templateModule _AtlModule;
