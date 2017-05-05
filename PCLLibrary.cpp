// PCLLibrary.cpp : Implementation of CPCLLibrary

#include "stdafx.h"
#include "PCLLibrary.h"
#include "com_utilities.h"
#include "Type1Type.h"
using namespace public_nbs;
using namespace public_nbs::pcl_extension;

//---------------------------------------------------------------------------
// CPCLLibrary
//---------------------------------------------------------------------------

/// ------- Template TODO -------
/// Define your own namespace name here
const wchar_t* const CPCLLibrary::NAMESPACE_NAME = L"brain_products";

//---------------------------------------------------------------------------

namespace
{

template <class T>
public_nbs::COM_ptr<IPCLType> make_type( IPCLLibrary* lib )
{
	T* temp = 0;
	public_nbs::COM_ptr<IPCLType> temp_com = create_com_object<IPCLType>( temp );

	temp->library_ = lib;
	lib->AddRef();

	return temp_com;
}

} // unnamed namespace

CPCLLibrary::CPCLLibrary()
{
	/// ------- Template TODO -------
	/// Add each of your IPCLType coclasses here
	type_functions_.push_back( &make_type<CType1Type> );
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getName(BSTR * name)
{
	COM_METHOD_START

	/// ------- Template TODO -------
	/// Define your extension name here
	*name = SysAllocString( L"Brain Products Remote Recorder PCL Extension Library" );

	COM_METHOD_END( L"CPCLLibrary::getName" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getAuthor(BSTR * author)
{
	COM_METHOD_START

	/// ------- Template TODO -------
	/// Define your name here
	*author = SysAllocString( L"Brain Products" );

	COM_METHOD_END( L"CPCLLibrary::getAuthor" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getVersion(BSTR * ver)
{
	COM_METHOD_START

	/// ------- Template TODO -------
	/// Put your version number here
	*ver = SysAllocString( L"1" );

	COM_METHOD_END( L"CPCLLibrary::getVersion" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getDescription(BSTR * desc)
{
	COM_METHOD_START

	/// ------- Template TODO -------
	/// Put a description of the extension here
	*desc = SysAllocString( L"PCL extension for remotely controlling BrainVision Recorder" );

	COM_METHOD_END( L"CPCLLibrary::getDescription" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getPresentationMinimumVersion(double* ver)
{
	COM_METHOD_START

	/// ------- Template TODO -------
	/// If you use PCL extension features released after the initial release,
	/// change this to the Presentation version number required
	/// (otherwise leave 0)
	*ver = 0;

	COM_METHOD_END( L"CPCLLibrary::getPresentationMinimumVersion" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getNamespaceName(BSTR * ns)
{
	COM_METHOD_START

	*ns = SysAllocString( NAMESPACE_NAME );

	COM_METHOD_END( L"CPCLLibrary::getNamespaceName" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getTypeCount(long * count)
{
	COM_METHOD_START

	*count = type_functions_.size();

	COM_METHOD_END( L"CPCLLibrary::getTypeCount" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getType(long index, IPCLType * * type)
{
	COM_METHOD_START

	if ( (index < 0) || (static_cast<size_t>( index ) >= type_functions_.size()) )
	{
		throw public_nbs::Exception( L"Invalid type index sent to extension." );
	}
	public_nbs::COM_ptr<IPCLType> temp = type_functions_[index]( this );
	temp->AddRef();
	*type = temp.get();

	COM_METHOD_END( L"CPCLLibrary::getType" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CPCLLibrary::getLastError(BSTR * message)
{
	COM_METHOD_START

	*message = SysAllocString( last_error_.c_str() );

	COM_METHOD_END( L"CPCLLibrary::getLastError" ) 
}

//---------------------------------------------------------------------------
