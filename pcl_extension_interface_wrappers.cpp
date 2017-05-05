#include "pcl_extension_interface_wrappers.h"
using namespace public_nbs;
using namespace public_nbs::pcl_extension;


const wchar_t* const public_nbs::pcl_extension::INT_NAME( L"int" );
const wchar_t* const public_nbs::pcl_extension::DOUBLE_NAME( L"double" );
const wchar_t* const public_nbs::pcl_extension::BOOL_NAME( L"bool" );
const wchar_t* const public_nbs::pcl_extension::STRING_NAME( L"string" );
const wchar_t* const public_nbs::pcl_extension::VOID_NAME( L"void" );
const wchar_t* const public_nbs::pcl_extension::GRAPHIC_SURFACE_NAME( L"graphic_surface" );
const wchar_t* const public_nbs::pcl_extension::WAVEFILE_NAME( L"wavefile" );

Argument_ptr Argument::create_subclass
( 
	COM_ptr<IPCLArgument> arg 
)
{
	Argument_ptr rval;
	Argument temp( arg );
	std::wstring type = temp.type();
	if (temp.rank() == 1)
	{
		if (type == INT_NAME)
		{
			rval.reset( new PCLIntArray( arg ) );
		}
		else if (type == BOOL_NAME)
		{
			rval.reset( new PCLBoolArray( arg ) );
		}
		else if (type == DOUBLE_NAME)
		{
			rval.reset( new PCLDoubleArray( arg ) );
		}
		else if (type == STRING_NAME)
		{
			rval.reset( new PCLStringArray( arg ) );
		}
		else
		{
			throw Exception( L"Unknown argument type passed to extension." );
		}
	}
	else if (temp.rank() > 1)
	{
		if ( (type == INT_NAME) || (type == BOOL_NAME) || (type == DOUBLE_NAME) || (type == STRING_NAME) )
		{
			rval.reset( new PCLArray( arg ) );
		}
		else
		{
			throw Exception( L"Unknown argument type passed to extension." );
		}
	}
	else
	{
		if (type == INT_NAME)
		{
			rval.reset( new PCLInt( arg ) );
		}
		else if (type == BOOL_NAME)
		{
			rval.reset( new PCLBool( arg ) );
		}
		else if (type == DOUBLE_NAME)
		{
			rval.reset( new PCLDouble( arg ) );
		}
		else if (type == STRING_NAME)
		{
			rval.reset( new PCLString( arg ) );
		}
		else if (type == GRAPHIC_SURFACE_NAME)
		{
			rval.reset( new PCLGraphicSurface( arg ) );
		}
		else if (type == WAVEFILE_NAME)
		{
			rval.reset( new PCLWavefile( arg ) );
		}
		else if (type == VOID_NAME)
		{
			rval.reset( new Argument( arg ) );
		}
		else
		{
			// see if it's a PCL extension type
			try
			{
				rval.reset( new PCLExtensionObject( arg ) );
			}
			catch (public_nbs::Exception&)
			{
				throw Exception( L"Type " + type + L" not supported by this extension" );
			}
		}
	}
	return rval;
}