// Type1Type.cpp : Implementation of CType1Type

#include "stdafx.h"
#include "Type1Type.h"
#include "com_utilities.h"
#include "PCLLibrary.h"
#include "Type1.h"
using namespace public_nbs;
using namespace public_nbs::pcl_extension;


//---------------------------------------------------------------------------
// CType1Type
//---------------------------------------------------------------------------

/// ------- Template TODO -------
/// Define your type name here
const wchar_t* const CType1Type::TYPE_NAME = L"remote";

const std::wstring CType1Type::complete_type_name_( 
	std::wstring( CPCLLibrary::NAMESPACE_NAME ) + L"::" + CType1Type::TYPE_NAME );

//---------------------------------------------------------------------------

CType1Type::CType1Type()
{
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1Type::getName(BSTR * name)
{
	COM_METHOD_START

	*name = SysAllocString( TYPE_NAME );

	COM_METHOD_END( L"CType1Type::getName" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1Type::getParentName(BSTR * parent)
{
	COM_METHOD_START

	/// ------- Template TODO -------
	/// If your type has a parent type, return it here.
	/// (Note that you will need to make method call dispatch changes
	/// to other code in this template to support inherited types.)
	*parent = SysAllocString( L""  );

	COM_METHOD_END( L"CType1Type::getParentName" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1Type::getMethods(IPCLMethodList * list)
{
	COM_METHOD_START

	list->AddRef();
	COM_ptr<IPCLMethodList> temp( list );
	Method_list mlist( temp );

	CType1::operators_new_.clear();
	/// ------- Template TODO -------
	/// add information about any new operators for the type
	{
		// add the initialization method which implements this to a local list
		CType1::operators_new_.push_back( &CType1::operator_new);
		// tell Presentation about the method
		Method m = mlist.add_new();
		m.set_description( L"Creates a Brain Products Remote Control Client" );
		m.add_argument( L"ip_address", STRING_NAME, 0, false, L"IP address of the recording computer" );
		m.add_argument(L"port", STRING_NAME, 0, false, L"Port for the recording computer");
	}
	
	CType1::methods_.clear();
	/// ------- Template TODO -------
	/// add information about the methods of this type
	{
		// add the method which implements this to a local list
		CType1::methods_.push_back( &CType1::open_recorder);
		// tell Presentation about the method
		Method m = mlist.add_method();
		m.set_name( L"open_recorder" );
		m.set_description( L"Starts and initializes BrainVision Recorder" );
		m.add_argument( L"path_to_workspace", STRING_NAME, 0, false, L"Path to the workspace to load" );
		m.add_argument(L"experiment_number", STRING_NAME, 0, false, L"Experiment name or number so that the saved .eeg file is named correctly according to conventions.");
		m.add_argument(L"subject_id", STRING_NAME, 0, false, L"Unique test subject number so that the saved .eeg file is named correctly according to conventions");
		m.add_argument(L"timeout", INT_NAME, 0, false, L"Timeout in milliseconds per command expected to be received from the BrainVision device.");
		
		mlist.add_predefined_variable(L"int", L"OVERWRITE_PROTECTION_OFF", L"0");
		mlist.add_predefined_variable(L"int", L"OVERWRITE_PROTECTION_ON", L"1");
	}
	{
		CType1::methods_.push_back( &CType1::close_recorder);
		Method m = mlist.add_method();
		m.set_name( L"close_recorder" );
		m.set_description( L"Closes the Recorder and deletes all the variables passed with initialization commands (e.g. for a new test subject when an experiment has been completed)." );
	}
	{
		CType1::methods_.push_back(&CType1::set_impedance_check_mode);
		Method m = mlist.add_method();
		m.set_name(L"set_impedance_check_mode");
		m.set_description(L"Switches the BrainVision Recorder to impedance check mode");
	}
	{
		CType1::methods_.push_back(&CType1::set_view_test_mode);
		Method m = mlist.add_method();
		m.set_name(L"set_view_test_mode");
		m.set_description(L"Switches the BrainVision Recorder to view test signal mode");
	}
	{
		CType1::methods_.push_back(&CType1::set_monitoring_mode);
		Method m = mlist.add_method();
		m.set_name(L"set_monitoring_mode");
		m.set_description(L"Switches the BrainVision Recorder to monitoring mode");
	}
	{
		CType1::methods_.push_back(&CType1::stop_viewing);
		Method m = mlist.add_method();
		m.set_name(L"stop_viewing");
		m.set_description(L"Stops the viewing or monitoring");
	}
	{
		CType1::methods_.push_back(&CType1::start_recording);
		Method m = mlist.add_method();
		m.set_name(L"start_recording");
		m.set_description(L"Starts the recording");
	}
	{
		CType1::methods_.push_back(&CType1::pause_recording);
		Method m = mlist.add_method();
		m.set_name(L"pause_recording");
		m.set_description(L"Pauses the recording");
	}
	{
		CType1::methods_.push_back(&CType1::resume_recording);
		Method m = mlist.add_method();
		m.set_name(L"resume_recording");
		m.set_description(L"Resumes the recording");
	}
	{
		CType1::methods_.push_back(&CType1::stop_recording);
		Method m = mlist.add_method();
		m.set_name(L"stop_recording");
		m.set_description(L"Stops the recording");
	}
	{
		CType1::methods_.push_back(&CType1::dc_reset);
		Method m = mlist.add_method();
		m.set_name(L"dc_reset");
		m.set_description(L"Performs a DC reset");
	}
	{
		CType1::methods_.push_back(&CType1::send_raw_message);
		Method m = mlist.add_method();
		m.set_name(L"send_raw_message");
		m.set_description(L"Sends a message you specify to the BrainVision server. It must be a supported message formatted correctly, or the recorder could be put in an unintended state. Only use this if the action you are performing is not already provided by another method.");
		m.add_argument(L"message", STRING_NAME, 0, false, L"Text to be sent to the BrainVision server.");
		m.add_argument(L"max responses", INT_NAME, 0, false, L"The maximum number of responses to listen for. If this is too high, the listener will timeout, causing a delay in the application (but no other harmful effects). If it is too low, leftover messages will confuse future commands.");
		m.add_argument(L"responses", STRING_NAME, 1, false, L"The responses returned by the server. Check the size of the array to determene the number of detected responses.");
	}
	{
		CType1::methods_.push_back(&CType1::set_overwrite_protection);
		Method m = mlist.add_method();
		m.set_name(L"set_overwrite_protection");
		m.set_description(L"Sets overwrite protection");
		m.add_argument(L"value", BOOL_NAME, 0, false, L"Overwrite protection value.");
	}
	{
		CType1::methods_.push_back(&CType1::send_annotation);
		Method m = mlist.add_method();
		m.set_name(L"send_annotation");
		m.set_description(L"Sends an annotation to BrainVision server (and recorder).");
		m.add_argument(L"description", STRING_NAME, 0, false, L"Description to be sent to the BrainVision server.");
		m.add_argument(L"type", STRING_NAME, 0, false, L"Type to be sent to the BrainVision server.");

	}

	COM_METHOD_END( L"CType1Type::getMethods" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1Type::createObject(long index, IPCLArguments* arguments, IPCLObject* * object)
{
	COM_METHOD_START

	if ( (index < 0) || (static_cast<size_t>( index ) >= CType1::operators_new_.size()) )
	{
		throw public_nbs::Exception( L"Invalid index sent to CType1Type::createObject" );
	}

	CType1* temp = 0;
	COM_ptr<IPCLObject> temp_com = create_com_object<IPCLObject>( temp );

	temp->type_.reset( this );
	AddRef();

	public_nbs::COM_ptr<IPCLArguments> args( arguments );
	args->AddRef();
	(temp->*(CType1::operators_new_[index]))( args );

	temp_com->AddRef();
	*object = temp_com.get();

	COM_METHOD_END( L"CType1Type::createObject" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1Type::getLibrary(IPCLLibrary* * lib)
{
	COM_METHOD_START

	*lib = library_.get();
	library_->AddRef();

	COM_METHOD_END( L"CType1Type::getLibrary" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1Type::getLastError(BSTR * message)
{
	COM_METHOD_START

	*message = SysAllocString( last_error_.c_str() );

	COM_METHOD_END( L"CType1Type::getLastError" ) 
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1Type::getDescription(BSTR * desc)
{
	COM_METHOD_START

	/// ------- Template TODO -------
	/// Define a description of your type here
	*desc = SysAllocString( L"This type handles a connection to the Brain Vision Remote Contorl Server 2.0"  );

	COM_METHOD_END( L"CType1Type::getDescription" ) 
}

//---------------------------------------------------------------------------