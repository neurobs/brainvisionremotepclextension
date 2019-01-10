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

//---------------------------------------------------------------------------


#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>

std::ofstream file;

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
	auto port = std::dynamic_pointer_cast<PCLString>( args.argument( 1 ) );
	check_args( ip, port, L"CType1::operator_new" );


	try
	{
		connection_.connect(unicode_to_single_byte(ip->value()), utf16_to_utf8(port->value()), boost::posix_time::milliseconds(10000));
	}
	catch (std::exception& e)
	{
		throw Exception(utf8_to_utf16(e.what()));
	}

	timeout_ = 10000;
	
	send("VM");
	check_response({ "VM:2\r" }, {});
}

void CType1::check_response(std::vector<std::string> desired_responses, std::vector<std::string> ignored_responses) {
	int max_lines = desired_responses.size() + ignored_responses.size();
	for (int i = 0; i<max_lines; i++) {
		if (desired_responses.size() == 0) return;
		
		std::string received = connection_.read_line(boost::posix_time::millisec(timeout_));
		if(received.size() == 0) continue;

		bool ignore = false;
		for (auto &ignored_response : ignored_responses) {
			if(received.compare(ignored_response) == 0) {
				ignore = true;
				break;
			}
		}
		if(ignore) continue;

		bool found = false;
		for (std::vector<std::string>::reverse_iterator i = desired_responses.rbegin();	i != desired_responses.rend(); ++i) {
			std::string desired_response = *i;
			if (desired_response.find(':') != std::string::npos) {
				std::istringstream iss(desired_response);
				std::string token;
				std::getline(iss, token, ':');

				//if received starts with desired token
				if (received.compare(0, token.length(), token) == 0) {
					//if matches
					if (received.compare(desired_response) == 0) {
						desired_responses.erase(std::next(i).base());
						found = true;
						break;
					} else {
						throw public_nbs::Exception(L"Expected BrainVision message: " + utf8_to_utf16(desired_response) + L" Received: " + utf8_to_utf16(received));
					}
				}
			} else {
				throw public_nbs::Exception(L"Desired BrainVision response: " + utf8_to_utf16(desired_response) + L" is not of the proper form");
			}
		}

		if(found) continue;
		
		throw public_nbs::Exception(L"Unexpected BrainVision message recieved: " + utf8_to_utf16(received));
	}

	if (desired_responses.size() > 0) {
		throw public_nbs::Exception(L"Expected BrainVision message not received: " + utf8_to_utf16(desired_responses[0]) + L". Try increasing the timeout in open_recorder command.");
	}

}



//---------------------------------------------------------------------------

void CType1::send(const std::string& s)
{
	try
	{
		connection_.write(s + "\r", boost::posix_time::milliseconds(SEND_TIME_OUT_MS));
	}
	catch (std::exception& e)
	{
		throw public_nbs::Exception(utf8_to_utf16(e.what()));
	}
}

//---------------------------------------------------------------------------

void CType1::open_recorder( public_nbs::pcl_extension::Arguments args )
{

	check_arg_count(args.count(), 4, L"CType1::open_recorder");
	auto workspace =  std::dynamic_pointer_cast<PCLString>(args.argument(0));
	auto experiment = std::dynamic_pointer_cast<PCLString>(args.argument(1));
	auto subject =    std::dynamic_pointer_cast<PCLString>(args.argument(2));
	auto timeout_ms =   std::dynamic_pointer_cast<PCLInt>(   args.argument(3));
	check_args( workspace, experiment, subject, timeout_ms, L"CType1::open_recorder" );
	timeout_ = timeout_ms->value();

	send("O");
	check_response({ "O:OK\r", "AP:1\r", "RS:0\r" }, { "AQ:0\r" });

	send("1:" + utf16_to_utf8(workspace->value()));
	check_response({"1:" + utf16_to_utf8(workspace->value()) + ":OK\r"}, {});
	send("2:" + utf16_to_utf8(experiment->value()));
	check_response({"2:" + utf16_to_utf8(experiment->value()) + ":OK\r"}, {});
	send("3:" + utf16_to_utf8(subject->value()));
	check_response({"3:" + utf16_to_utf8(subject->value()) + ":OK\r"}, {});

}

//---------------------------------------------------------------------------

void CType1::close_recorder( public_nbs::pcl_extension::Arguments args )
{
	check_arg_count(args.count(), 0, L"CType1::close_recorder");

	send("X");
	check_response({ "X:OK\r", "AP:0\r" }, {"AQ:0\r", "RS:0\r"});

}

//---------------------------------------------------------------------------

void CType1::set_impedance_check_mode(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::set_impedance_check_mode");
	auto state = get_recorder_state();
	if (state == recorder_state::IMPEDANCE_CHECK) {
		return;
	} 
	else if (state == recorder_state::SAVING_RECORDING) {
		send("I");
		check_response({ "I:OK\r", "RS:8\r", "AQ:1\r" }, { "AQ:0\r" });
	} 
	else {
		send("I");
		check_response({ "I:OK\r", "RS:3\r", "AQ:1\r" }, {"AQ:0\r", "RS:0\r"});
	}
}

//---------------------------------------------------------------------------

void CType1::set_view_test_mode(public_nbs::pcl_extension::Arguments args)
{
	if(get_recorder_state() != recorder_state::CALIBRATION) {
		check_arg_count(args.count(), 0, L"CType1::set_view_test_mode");

		send("T");
		check_response({ "T:OK\r", "RS:2\r", "AQ:1\r" }, {"AQ:0\r"});
	}

}

//---------------------------------------------------------------------------

void CType1::set_monitoring_mode(public_nbs::pcl_extension::Arguments args)
{
	if (get_recorder_state() != recorder_state::MONITORING) {
		check_arg_count(args.count(), 0, L"CType1::set_monitoring_mode");

		send("M");
		check_response({ "M:OK\r", "RS:1\r", "AQ:1\r" }, { "AQ:0\r" });
	}
}

//---------------------------------------------------------------------------

void CType1::stop_viewing(public_nbs::pcl_extension::Arguments args)
{
	recorder_state state = get_recorder_state();
	if (state == recorder_state::MONITORING || state == recorder_state::CALIBRATION || state == recorder_state::IMPEDANCE_CHECK || state == recorder_state::SAVING_RECORDING || state == recorder_state::PAUSE_RECORDING) {
		check_arg_count(args.count(), 0, L"CType1::stop_viewing");

		send("SV");
		check_response({ "SV:OK\r", "RS:0\r", "AQ:0\r" }, {});
	}
}

//---------------------------------------------------------------------------

void CType1::start_recording(public_nbs::pcl_extension::Arguments args)
{
	
	recorder_state state = get_recorder_state();
	if (state == recorder_state::SAVING_RECORDING || state == recorder_state::SAVING_CALIBRATION) {
		return;
	}

	check_arg_count(args.count(), 0, L"CType1::start_recording");

	if(state == recorder_state::MONITORING) {	
		send("S");
		check_response({ "S:OK\r", "RS:4\r" }, {});
	}
	else if (state == recorder_state::CALIBRATION) {
		check_response({ "S:OK\r", "RS:5\r" }, {});
	}
}

//---------------------------------------------------------------------------

void CType1::pause_recording(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::pause_recording");
	recorder_state state = get_recorder_state();
	if (state == recorder_state::SAVING_RECORDING) {
		send("P");
		check_response({ "P:OK\r", "RS:6\r" }, {});
	}
	else if (state == recorder_state::SAVING_CALIBRATION) {
		send("P");
		check_response({ "P:OK\r", "RS:7\r" }, {});
	}
}

//---------------------------------------------------------------------------

void CType1::resume_recording(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::resume_recording");
	recorder_state state = get_recorder_state();
	if(state == recorder_state::PAUSE_RECORDING) {
		send("C");
		check_response({ "C:OK\r", "RS:4\r" }, {});
	}
	else if (state == recorder_state::PAUSE_CALIBRATION) {
		send("C");
		check_response({ "C:OK\r", "RS:5\r" }, {});
	} 
	else if (state == recorder_state::PAUSE_IMPEDANCE_CHECK) {
		send("C");
		check_response({ "C:OK\r", "RS:4\r", "AQ:1\r" }, {"AQ:0\r"});
	}

}

//---------------------------------------------------------------------------

void CType1::stop_recording(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::stop_recording");
	recorder_state state = get_recorder_state();
	if(state == recorder_state::SAVING_RECORDING || state == recorder_state::PAUSE_RECORDING) {
		send("Q");
		check_response({ "Q:OK\r", "RS:1\r" }, {});
	}
	else if (state == recorder_state::SAVING_CALIBRATION || state == recorder_state::PAUSE_CALIBRATION) {
		send("Q");
		check_response({ "Q:OK\r", "RS:2\r" }, {});
	}
}

//---------------------------------------------------------------------------

void CType1::dc_reset(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 0, L"CType1::dc_reset");

	send("D");
	check_response({ "D:OK\r"}, {});
}

//---------------------------------------------------------------------------

void CType1::send_raw_message(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 3, L"CType1::send_raw_message");
	auto message = std::dynamic_pointer_cast<PCLString>(args.argument(0));
	auto max_responses = std::dynamic_pointer_cast<PCLInt>(args.argument(1));
	auto responses = std::dynamic_pointer_cast<PCLStringArray>(args.argument(2));
	
	std::vector<std::string> responses_local;
	std::string response = "";
	send(utf16_to_utf8(message->value()));

	for(int i=0; i<max_responses->value(); i++) {
		response = connection_.read_line(boost::posix_time::millisec(timeout_));
		if(response == "") break;
		else responses_local.push_back(response);
	}

	responses->resize(responses_local.size());
	for (unsigned int i = 0; i < responses_local.size(); i++) {
		responses->set_value(i, utf8_to_utf16(responses_local[i]));
	}

}

//---------------------------------------------------------------------------

void CType1::set_overwrite_protection(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 1, L"CType1::set_overwrite_protection");
	auto p = std::dynamic_pointer_cast<PCLBool>(args.argument(0));
	send(p->value() ? "OW:1" : "OW:0");

	std::vector<std::string> ow1 = { "OW:1:OK\r" };
	std::vector<std::string> ow0 = { "OW:0:OK\r" };
	check_response(p->value() ? ow1 : ow0, {});
}

//---------------------------------------------------------------------------

CType1::application_state CType1::get_app_state()
{
	application_state state;
	send("AP");
	try {
		std::string received = connection_.read_line(boost::posix_time::millisec(timeout_));
		state = static_cast<application_state>(std::stoi(received.substr(3, received.size() - 1)));
	}
	catch (...) {
		throw public_nbs::Exception(L"Unable to determine BrainVision application state");
	}
	
	return state;

}

//---------------------------------------------------------------------------

CType1::recorder_state CType1::get_recorder_state()
{
	recorder_state state;
	send("RS");
	try {
		std::string received = connection_.read_line(boost::posix_time::millisec(timeout_));
		state = static_cast<recorder_state>(std::stoi(received.substr(3, received.size() - 1)));
	}
	catch (...) {
		throw public_nbs::Exception(L"Unable to determine BrainVision recorder state");
	}

	return state;

}

//---------------------------------------------------------------------------

CType1::acquisition_state CType1::get_acquisition_state()
{
	acquisition_state state;
	send("AQ");
	try {
		std::string received = connection_.read_line(boost::posix_time::millisec(timeout_));
		state = static_cast<acquisition_state>(std::stoi(received.substr(3, received.size() - 1)));
	}
	catch (...) {
		throw public_nbs::Exception(L"Unable to determine BrainVision aquistion state");
	}

	return state;

}

//---------------------------------------------------------------------------


void CType1::send_annotation(public_nbs::pcl_extension::Arguments args)
{
	check_arg_count(args.count(), 2, L"CType1::send_annotation");
	std::string description = utf16_to_utf8(std::dynamic_pointer_cast<PCLString>(args.argument(0))->value());
	std::string type = utf16_to_utf8(std::dynamic_pointer_cast<PCLString>(args.argument(1))->value());
	std::string message ="AN:" + description + ";" + type;
	
	send(message);
	check_response({ "AN:" + description + ";" + type + ":OK\r" }, {});

}
