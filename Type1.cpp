// Type1.cpp : Implementation of CType1

#include "stdafx.h"
#include "Type1.h"
#include "com_utilities.h"
#include <codecvt>
using namespace public_nbs;
using namespace public_nbs::pcl_extension;

namespace
{

std::string utf16_to_utf8(const std::wstring& s)
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(s);
}

std::wstring utf8_to_utf16(const std::string& s)
{
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(s);
}

}

//---------------------------------------------------------------------------
// CType1
//---------------------------------------------------------------------------

std::vector<CType1::Method_call> CType1::methods_;
std::vector<CType1::Method_call> CType1::operators_new_;
const char* const CType1::PORT = "6700";

//---------------------------------------------------------------------------

/// ------- Template TODO -------
/// Add default initialization of data members here
CType1::CType1()
{
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1::callMethod(long methodIndex, IPCLArguments * arguments)
{
	COM_METHOD_START

	if ( (methodIndex < 0) || (static_cast<size_t>( methodIndex ) >= methods_.size()) )
	{
		throw public_nbs::Exception( L"Invalid index sent to CType1::callMethod" );
	}
	arguments->AddRef();
	public_nbs::COM_ptr<IPCLArguments> temp( arguments );
	(this->*(methods_[methodIndex]))( temp );
	
	COM_METHOD_END( L"CType1::callMethod" );
}

//---------------------------------------------------------------------------
	
STDMETHODIMP CType1::getType(IPCLType* * type)
{
	COM_METHOD_START

	*type = type_.get();
	type_->AddRef();

	COM_METHOD_END( L"CType1::getType" );
}

//---------------------------------------------------------------------------

STDMETHODIMP CType1::getLastError(BSTR * message)
{
	COM_METHOD_START

	*message = SysAllocString( last_error_.c_str() );

	COM_METHOD_END( L"CType1::getLastError" );
}

//---------------------------------------------------------------------------
/// ------- Template TODO -------
/// Implement your class members here
//---------------------------------------------------------------------------

void CType1::operator_new( public_nbs::pcl_extension::Arguments args )
{
	check_arg_count( args.count(), 2, L"CType1::operator_new" );

	auto ip = std::dynamic_pointer_cast<PCLString>( args.argument( 0 ) );
	auto time_out_ms = std::dynamic_pointer_cast<PCLInt>( args.argument( 1 ) );
	check_args( ip, time_out_ms, L"CType1::operator_new" );

	try
	{
		connection_.connect(unicode_to_single_byte(ip->value()), PORT, boost::posix_time::milliseconds(time_out_ms->value()));
	}
	catch (std::exception& e)
	{
		throw Exception(utf8_to_utf16(e.what()));
	}
}

//---------------------------------------------------------------------------

void CType1::send(const std::string& s)
{
	try
	{
		connection_.write(s, boost::posix_time::milliseconds(SEND_TIME_OUT_MS));
	}
	catch (std::exception& e)
	{
		throw Exception(utf8_to_utf16(e.what()));
	}
}

//---------------------------------------------------------------------------

void CType1::open_recorder( public_nbs::pcl_extension::Arguments args )
{
	check_arg_count(args.count(), 4, L"CType1::open_recorder");
	auto workspace =  std::dynamic_pointer_cast<PCLString>(args.argument(0));
	auto experiment = std::dynamic_pointer_cast<PCLString>(args.argument(1));
	auto subject =    std::dynamic_pointer_cast<PCLString>(args.argument(2));
	auto delay_ms =   std::dynamic_pointer_cast<PCLInt>(   args.argument(3));
	check_args( workspace, experiment, subject, delay_ms, L"CType1::open_recorder" );

	send("1" + utf16_to_utf8(workspace->value()));
	Sleep(delay_ms->value());
	send("2" + utf16_to_utf8(experiment->value()));
	Sleep(delay_ms->value());
	send("3" + utf16_to_utf8(subject->value()));
	Sleep(delay_ms->value());
	send("4");
	Sleep(delay_ms->value());
}

//---------------------------------------------------------------------------

void CType1::close_recorder( public_nbs::pcl_extension::Arguments args )
{
	check_arg_count(args.count(), 0, L"CType1::close_recorder");

	send("X");
}

//---------------------------------------------------------------------------

void CType1::set_impedance_check_mode(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::set_impedance_check_mode");

	send("I");
}

//---------------------------------------------------------------------------

void CType1::set_monitoring_mode(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::set_monitoring_mode");

	send("M");
}

//---------------------------------------------------------------------------

void CType1::start_recording(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::start_recording");

	send("S");
}

//---------------------------------------------------------------------------

void CType1::pause_recording(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::pause_recording");

	send("P");
}

//---------------------------------------------------------------------------

void CType1::resume_recording(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::resume_recording");

	send("C");
}

//---------------------------------------------------------------------------

void CType1::stop_recording(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::stop_recording");

	send("Q");
}

//---------------------------------------------------------------------------

void CType1::dc_reset(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::dc_reset");

	send("D");
}

//---------------------------------------------------------------------------

void CType1::send_raw_message(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 1, L"CType1::send_raw_message");
	auto message = std::dynamic_pointer_cast<PCLString>(args.argument(0));

	send(utf16_to_utf8(message->value()));
}

//---------------------------------------------------------------------------