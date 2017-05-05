#pragma once

#include "PCLExtension.h"
#include "COM_ptr.h"
#include "b_str.h"
#include <vector>
#include <memory>

namespace public_nbs 
{

namespace pcl_extension
{

extern const wchar_t* const INT_NAME;
extern const wchar_t* const DOUBLE_NAME;
extern const wchar_t* const BOOL_NAME;
extern const wchar_t* const STRING_NAME;
extern const wchar_t* const VOID_NAME;
extern const wchar_t* const GRAPHIC_SURFACE_NAME;
extern const wchar_t* const WAVEFILE_NAME;

class Argument;
typedef std::shared_ptr<Argument> Argument_ptr;

/// Wrapper around IPCLArgument
///
/// \note All methods can throw public_nbs::Exception
class Argument
{
public:
	Argument( COM_ptr<IPCLArgument> arg ) : arg_( arg ) {}

	std::wstring type()
	{
		B_str type;
		check_com_call( 
			arg_->getType( type.assignee() ), 
			arg_.get(), L"IPCLArgument::getType" );
		return type.str();
	}

	long rank()
	{
		long rank;
		check_com_call( 
			arg_->getRank( &rank ), 
			arg_.get(), L"IPCLArgument::getRank" );
		return rank;
	}

	bool is_reference()
	{
		long is_reference;
		check_com_call( 
			arg_->getIsReference( &is_reference ), 
			arg_.get(), L"IPCLArgument::getIsReference" );
		return is_reference != 0;
	}

	static Argument_ptr create_subclass( COM_ptr<IPCLArgument> arg );

	virtual ~Argument() {}

private:
	COM_ptr<IPCLArgument> arg_;
};

/// Wrapper around IPCLWavefileLock
///
/// \note All methods can throw public_nbs::Exception
class PCLWavefileLock
{
public:
	PCLWavefileLock( COM_ptr<IPCLWavefileLock> lock ) : lock_( lock ) {}

	byte* get_data_ptr()
	{
		byte* ptr;
		check_com_call( lock_->getDataPtr( &ptr ),
			lock_.get(), L"IPCLWavefileLock::getDataPtr" );
		return ptr;
	}

	void get_parameters( PCLAudioParameters& parameters )
	{
		check_com_call( lock_->getParameters( &parameters ),
			lock_.get(), L"IPCLWavefileLock::getParameters" );
	}

private:
	COM_ptr<IPCLWavefileLock> lock_;
};

/// Wrapper around IPCLWavefile
///
/// \note All methods can throw public_nbs::Exception
class PCLWavefile : public Argument
{
public:
	PCLWavefile( COM_ptr<IPCLArgument> arg ) : Argument( arg ), 
		wave_( query_interface<IPCLWavefile>( arg, IID_IPCLWavefile ) ) {}

	bool is_null()
	{
		long null = 0;
		check_com_call( wave_->getIsNull( &null ),
			wave_.get(), L"IPCLWavefile::getIsNull" );
		return null != 0;
	}

	void get_parameters( PCLAudioParameters& parameters )
	{
		check_com_call( wave_->getParameters( &parameters ),
			wave_.get(), L"IPCLWavefile::getParameters" );
	}

	void resize( long frame_count, long channel_count )
	{
		check_com_call( wave_->resize( frame_count, channel_count ),
			wave_.get(), L"IPCLWavefile::resize" );
	}

	PCLWavefileLock lock()
	{
		COM_ptr<IPCLWavefileLock> lock;
		check_com_call( wave_->lock( lock.assignee() ),
			wave_.get(), L"IPCLWavefile::lock" );
		return PCLWavefileLock( lock );
	}

	void replace( const PCLAudioParameters& parameters, byte* data )
	{
		check_com_call( wave_->replace( parameters, data ),
			wave_.get(), L"IPCLWavefile::replace" );
	}

private:
	COM_ptr<IPCLWavefile> wave_;
};

typedef std::shared_ptr<PCLWavefile> PCLWavefile_ptr;

/// Wrapper around IPCLGraphicSurfaceLock
///
/// \note All methods can throw public_nbs::Exception
class PCLGraphicSurfaceLock
{
public:
	PCLGraphicSurfaceLock( COM_ptr<IPCLGraphicSurfaceLock> lock ) : lock_( lock ) {}

	byte* get_data_ptr()
	{
		byte* ptr;
		check_com_call( lock_->getDataPtr( &ptr ),
			lock_.get(), L"IPCLGraphicSurfaceLock::getDataPtr" );
		return ptr;
	}

	void get_parameters( PCLGraphicParameters& parameters )
	{
		check_com_call( lock_->getParameters( &parameters ),
			lock_.get(), L"IPCLGraphicSurfaceLock::getParameters" );
	}

private:
	COM_ptr<IPCLGraphicSurfaceLock> lock_;
};

/// Wrapper around IPCLGraphicSurface
///
/// \note All methods can throw public_nbs::Exception
class PCLGraphicSurface : public Argument
{
public:
	PCLGraphicSurface( COM_ptr<IPCLArgument> arg ) : Argument( arg ), 
		surface_( query_interface<IPCLGraphicSurface>( arg, IID_IPCLGraphicSurface ) ) {}

	bool is_null()
	{
		long null = 0;
		check_com_call( surface_->getIsNull( &null ),
			surface_.get(), L"IPCLGraphicSurface::getIsNull" );
		return null != 0;
	}

	void get_size( long& width_pixels, long& height_pixels )
	{
		check_com_call( surface_->getSize( &width_pixels, &height_pixels ),
			surface_.get(), L"IPCLGraphicSurface::getSize" );
	}

	void resize( long width_pixels, long height_pixels )
	{
		check_com_call( surface_->resize( width_pixels, height_pixels ),
			surface_.get(), L"IPCLGraphicSurface::resize" );
	}

	PCLGraphicSurfaceLock lock()
	{
		COM_ptr<IPCLGraphicSurfaceLock> lock;
		check_com_call( surface_->lock( lock.assignee() ),
			surface_.get(), L"IPCLGraphicSurface::lock" );
		return PCLGraphicSurfaceLock( lock );
	}

	void replace( const PCLGraphicParameters& parameters, byte* data )
	{
		check_com_call( surface_->replace( parameters, data ),
			surface_.get(), L"IPCLGraphicSurface::replace" );
	}

	void convert_custom_units( double custom_width, double custom_height,
		double& pixel_width, double& pixel_height )
	{
		check_com_call( surface_->convertCustomUnits( custom_width, custom_height,
			&pixel_width, &pixel_height ),
			surface_.get(), L"IPCLGraphicSurface::convertCustomUnits" );
	}

private:
	COM_ptr<IPCLGraphicSurface> surface_;
};

typedef std::shared_ptr<PCLGraphicSurface> PCLGraphicSurface_ptr;

/// Wrapper around IPCLExtensionObject
///
/// \note All methods can throw public_nbs::Exception
class PCLExtensionObject : public Argument
{
public:
	PCLExtensionObject( COM_ptr<IPCLArgument> arg ) : Argument( arg ), 
		arg_( query_interface<IPCLExtensionObject>( arg, IID_IPCLExtensionObject ) ) {}

	COM_ptr<IPCLObject> value()
	{
		COM_ptr<IPCLObject> val;
		check_com_call( arg_->getValue( val.assignee() ),
			arg_.get(), L"IPCLExtensionObject::getValue" );
		return val;
	}

	void set_value( COM_ptr<IPCLObject> val )
	{
		check_com_call( arg_->setValue( val.get() ),
			arg_.get(), L"IPCLExtensionObject::setValue" );
	}

private:
	COM_ptr<IPCLExtensionObject> arg_;
};

typedef std::shared_ptr<PCLExtensionObject> PCLExtensionObject_ptr;

class PCLArray;
typedef std::shared_ptr<PCLArray> PCLArray_ptr;

/// Wrapper around IPCLArray
///
/// \note All methods can throw public_nbs::Exception
class PCLArray : public Argument
{
public:
	PCLArray( COM_ptr<IPCLArgument> arr ) : Argument( arr ),
		arr_( query_interface<IPCLArray>( arr, IID_IPCLArray ) ) 
	{}
	
	PCLArray( COM_ptr<IPCLArray> arr ) : Argument( arr ),
		arr_( arr ) 
	{}

	long count()
	{
		long count;
		check_com_call( 
			arr_->getCount( &count ),
			arr_.get(), L"IPCLArray::getCount" );
		return count;
	}
	
	void resize( long count )
	{
		check_com_call(
			arr_->resize( count ),
			arr_.get(), L"IPCLArray::resize" );
	}

	PCLArray_ptr get_sub_array( long index )
	{
		COM_ptr<IPCLArray> temp;
		check_com_call( arr_->getSubArray( index, temp.assignee() ),
			arr_.get(), L"IPCLArray::getSubArray" );
		Argument_ptr arg = Argument::create_subclass( temp );
		return std::dynamic_pointer_cast<PCLArray>( arg );
	}

private:
	COM_ptr<IPCLArray> arr_;
};

/// Wrapper around IPCLStringArray
///
/// \note All methods can throw public_nbs::Exception
class PCLStringArray : public PCLArray
{
public:
	PCLStringArray( COM_ptr<IPCLArgument> arr ) : PCLArray( arr ),
		arr_( query_interface<IPCLStringArray>( arr, IID_IPCLStringArray ) )
	{}

	void set_value( long index, const std::wstring& value )
	{
		public_nbs::B_str temp( value ); 
		check_com_call( arr_->setValue( index, temp.b_str() ),
			arr_.get(), L"IPCLStringArray::setValue" );
	}

	std::wstring get_value( long index )
	{
		public_nbs::B_str value;
		check_com_call( arr_->getValue( index, value.assignee() ),
			arr_.get(), L"IPCLStringArray::getValue" );
		return value.str();
	}

	void add( const std::wstring& value )
	{
		public_nbs::B_str temp( value ); 
		check_com_call( arr_->add( temp.b_str() ),
			arr_.get(), L"IPCLStringArray::add" );
	}

private:
	COM_ptr<IPCLStringArray> arr_;
};

typedef std::shared_ptr<PCLStringArray> PCLStringArray_ptr;

/// Wrapper around IPCLIntArray
///
/// \note All methods can throw public_nbs::Exception
class PCLIntArray : public PCLArray
{
public:
	PCLIntArray( COM_ptr<IPCLArgument> arr ) : PCLArray( arr ),
		arr_( query_interface<IPCLIntArray>( arr, IID_IPCLIntArray ) )
	{}

	void get_data( std::vector<long>& data )
	{
		data.resize( count() );
		long* ptr = get_data_ptr();
		std::copy( ptr, ptr + data.size(), data.begin() );
	}

	long* get_data_ptr()
	{
		long* ptr;
		check_com_call( arr_->getDataPtr( &ptr ),
			arr_.get(), L"IPCLIntArray::getDataPtr" );
		return ptr;
	}

	void set_value( long index, long value )
	{
		check_com_call( arr_->setValue( index, value ),
			arr_.get(), L"IPCLIntArray::setValue" );
	}

	long get_value( long index )
	{
		long value;
		check_com_call( arr_->getValue( index, &value ),
			arr_.get(), L"IPCLIntArray::getValue" );
		return value;
	}

	void add( long value )
	{
		check_com_call( arr_->add( value ),
			arr_.get(), L"IPCLIntArray::add" );
	}

private:
	COM_ptr<IPCLIntArray> arr_;
};

typedef std::shared_ptr<PCLIntArray> PCLIntArray_ptr;

/// Wrapper around IPCLDoubleArray
///
/// \note All methods can throw public_nbs::Exception
class PCLDoubleArray : public PCLArray
{
public:
	PCLDoubleArray( COM_ptr<IPCLArgument> arr ) : PCLArray( arr ),
		arr_( query_interface<IPCLDoubleArray>( arr, IID_IPCLDoubleArray ) )
	{}

	void get_data( std::vector<double>& data )
	{
		data.resize( count() );
		double* ptr = get_data_ptr();
		std::copy( ptr, ptr + data.size(), data.begin() );
	}

	double* get_data_ptr()
	{
		double* ptr;
		check_com_call( arr_->getDataPtr( &ptr ),
			arr_.get(), L"IPCLDoubleArray::getDataPtr" );
		return ptr;
	}

	void set_value( long index, double value )
	{
		check_com_call( arr_->setValue( index, value ),
			arr_.get(), L"IPCLDoubleArray::setValue" );
	}

	double get_value( long index )
	{
		double value;
		check_com_call( arr_->getValue( index, &value ),
			arr_.get(), L"IPCLDoubleArray::getValue" );
		return value;
	}

	void add( double value )
	{
		check_com_call( arr_->add( value ),
			arr_.get(), L"IPCLDoubleArray::add" );
	}

private:
	COM_ptr<IPCLDoubleArray> arr_;
};

typedef std::shared_ptr<PCLDoubleArray> PCLDoubleArray_ptr;

/// Wrapper around IPCLBoolArray
///
/// \note All methods can throw public_nbs::Exception
class PCLBoolArray : public PCLArray
{
public:
	PCLBoolArray( COM_ptr<IPCLArgument> arr ) : PCLArray( arr ),
		arr_( query_interface<IPCLBoolArray>( arr, IID_IPCLBoolArray ) )
	{}

	byte* get_data_ptr()
	{
		byte* ptr;
		check_com_call( arr_->getDataPtr( &ptr ),
			arr_.get(), L"IPCLBoolArray::getDataPtr" );
		return ptr;
	}

	void set_value( long index, bool value )
	{
		check_com_call( arr_->setValue( index, value ),
			arr_.get(), L"IPCLBoolArray::setValue" );
	}

	bool get_value( long index )
	{
		byte value;
		check_com_call( arr_->getValue( index, &value ),
			arr_.get(), L"IPCLBoolArray::getValue" );
		return value != 0;
	}

	void add( byte value )
	{
		check_com_call( arr_->add( value ),
			arr_.get(), L"IPCLBoolArray::add" );
	}

private:
	COM_ptr<IPCLBoolArray> arr_;
};

typedef std::shared_ptr<PCLBoolArray> PCLBoolArray_ptr;

/// Wrapper around IPCLString
///
/// \note All methods can throw public_nbs::Exception
class PCLString : public Argument
{
public:
	PCLString( COM_ptr<IPCLArgument> arg ) : Argument( arg ), 
		arg_( query_interface<IPCLString>( arg, IID_IPCLString ) ) {}

	std::wstring value()
	{
		B_str val;
		check_com_call( arg_->getValue( val.assignee() ),
			arg_.get(), L"IPCLString::getValue" );
		return val.str();
	}

	void set_value( const std::wstring& val )
	{
		B_str temp( val.c_str() );
		check_com_call( arg_->setValue( temp.b_str() ),
			arg_.get(), L"IPCLString::setValue" );
	}

private:
	COM_ptr<IPCLString> arg_;
};

typedef std::shared_ptr<PCLString> PCLString_ptr;

/// Wrapper around IPCLInt
///
/// \note All methods can throw public_nbs::Exception
class PCLInt : public Argument
{
public:
	PCLInt( COM_ptr<IPCLArgument> arg ) : Argument( arg ), 
		arg_( query_interface<IPCLInt>( arg, IID_IPCLInt ) ) {}

	long value()
	{
		long val;
		check_com_call( arg_->getValue( &val ),
			arg_.get(), L"IPCLInt::getValue" );
		return val;
	}

	void set_value( long val )
	{
		check_com_call( arg_->setValue( val ),
			arg_.get(), L"IPCLInt::setValue" );
	}

private:
	COM_ptr<IPCLInt> arg_;
};

typedef std::shared_ptr<PCLInt> PCLInt_ptr;

/// Wrapper around IPCLDouble
///
/// \note All methods can throw public_nbs::Exception
class PCLDouble : public Argument
{
public:
	PCLDouble( COM_ptr<IPCLArgument> arg ) : Argument( arg ), 
		arg_( query_interface<IPCLDouble>( arg, IID_IPCLDouble ) ) {}

	double value()
	{
		double val;
		check_com_call( arg_->getValue( &val ),
			arg_.get(), L"IPCLDouble::getValue" );
		return val;
	}

	void set_value( double val )
	{
		check_com_call( arg_->setValue( val ),
			arg_.get(), L"IPCLDouble::setValue" );
	}

private:
	COM_ptr<IPCLDouble> arg_;
};

typedef std::shared_ptr<PCLDouble> PCLDouble_ptr;

/// Wrapper around IPCLBool
///
/// \note All methods can throw public_nbs::Exception
class PCLBool : public Argument
{
public:
	PCLBool( COM_ptr<IPCLArgument> arg ) : Argument( arg ), 
		arg_( query_interface<IPCLBool>( arg, IID_IPCLBool ) ) {}

	bool value()
	{
		long val;
		check_com_call( arg_->getValue( &val ),
			arg_.get(), L"IPCLBool::getValue" );
		return val != 0;
	}

	void set_value( bool val )
	{
		check_com_call( arg_->setValue( val ),
			arg_.get(), L"IPCLBool::setValue" );
	}

private:
	COM_ptr<IPCLBool> arg_;
};

typedef std::shared_ptr<PCLBool> PCLBool_ptr;

/// Wrapper around IPCLArguments
///
/// \note All methods can throw public_nbs::Exception
class Arguments
{
public:
	Arguments( COM_ptr<IPCLArguments> args ) : args_( args ) {}

	long count()
	{
		long count;
		check_com_call( args_->getCount( &count ),
			args_.get(), L"IPCLArguments::getCount" );
		return count;
	}

	Argument_ptr argument( long index )
	{
		COM_ptr<IPCLArgument> arg;
		check_com_call( args_->getArgument( index, arg.assignee() ),
			args_.get(), L"IPCLArguments::getArgument" );
		return Argument::create_subclass( arg );
	}

	Argument_ptr return_value()
	{
		COM_ptr<IPCLArgument> arg;
		check_com_call( args_->getReturnValue( arg.assignee() ),
			args_.get(), L"IPCLArguments::getReturnValue" );
		return Argument::create_subclass( arg );
	}

private:
	COM_ptr<IPCLArguments> args_;
};

/// Wrapper around IPCLMethod
///
/// \note All methods can throw public_nbs::Exception
class Method
{
public:
	Method( COM_ptr<IPCLMethod> method ) : method_( method ) {}

	void set_name( const std::wstring& name )
	{
		B_str temp( name.c_str() );
		check_com_call( method_->setName( temp.b_str() ),
			method_.get(), L"IPCLMethod::setName" );
	}

	void set_description( const std::wstring& desc )
	{
		B_str temp( desc.c_str() );
		check_com_call( method_->setDescription( temp.b_str() ),
			method_.get(), L"IPCLMethod::setDescription" );
	}

	void add_argument( const std::wstring& name, const std::wstring& type,
		long rank, bool is_reference, const std::wstring& description )
	{
		B_str temp_name( name.c_str() );
		B_str temp_type( type.c_str() );
		B_str temp_desc( description.c_str() );
		check_com_call( method_->addArgument( temp_name.b_str(),
			temp_type.b_str(), rank, is_reference, temp_desc.b_str() ),
			method_.get(), L"IPCLMethod::addArgument" );
	}

	void set_return_value( const std::wstring& type, long rank, const std::wstring& description )
	{
		B_str temp_type( type.c_str() );
		B_str temp_desc( description.c_str() );
		check_com_call( method_->setReturnValue( temp_type.b_str(), rank, temp_desc.b_str() ),
			method_.get(), L"IPCLMethod::setReturnValue" );
	}

private:
	COM_ptr<IPCLMethod> method_;
};

/// Wrapper around IPCLMethodList
///
/// \note All methods can throw public_nbs::Exception
class Method_list
{
public:
	Method_list( COM_ptr<IPCLMethodList> list ) : list_( list ) {}

	Method add_method()
	{
		COM_ptr<IPCLMethod> method;
		check_com_call( list_->addMethod( method.assignee() ),
			list_.get(), L"IPCLMethodList::addMethod" );
		return Method( method );
	}

	Method add_new()
	{
		COM_ptr<IPCLMethod> method;
		check_com_call( list_->addNew( method.assignee() ),
			list_.get(), L"IPCLMethodList::add_new" );
		return Method( method );
	}

	void add_predefined_variable( const std::wstring& type, const std::wstring& name,
		const std::wstring& value )
	{
		B_str temp_type( type.c_str() );
		B_str temp_name( name.c_str() );
		B_str temp_value( value.c_str() );
		check_com_call( list_->addPredefinedVariable( temp_type.b_str(), 
			temp_name.b_str(), temp_value.b_str() ),
			list_.get(), L"IPCLMethodList::addPredefinedVariable" );
	}

private:
	COM_ptr<IPCLMethodList> list_;
};

template <class T1>
void check_args( T1 t1, std::wstring method_name )
{
	if (!t1)
	{
		throw Exception( L"Unexpected arguments in method " + method_name );
	}
}

template <class T1,class T2>
void check_args( T1 t1, T2 t2, std::wstring method_name )
{
	if (!t1 || !t2)
	{
		throw Exception( L"Unexpected arguments in method " + method_name );
	}
}

template <class T1,class T2,class T3>
void check_args( T1 t1, T2 t2, T3 t3, std::wstring method_name )
{
	if (!t1 || !t2 || !t3)
	{
		throw Exception( L"Unexpected arguments in method " + method_name );
	}
}

template <class T1,class T2,class T3,class T4>
void check_args( T1 t1, T2 t2, T3 t3, T4 t4, std::wstring method_name )
{
	check_args( t1, t2, t3, method_name );
	check_args( t4, method_name );
}

template <class T1,class T2,class T3,class T4,class T5>
void check_args( T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, std::wstring method_name )
{
	check_args( t1, t2, t3, method_name );
	check_args( t4, t5, method_name );
}

template <class T1,class T2,class T3,class T4,class T5,class T6>
void check_args( T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, std::wstring method_name )
{
	check_args( t1, t2, t3, method_name );
	check_args( t4, t5, t6, method_name );
}

template <class T1,class T2,class T3,class T4,class T5,class T6,class T7>
void check_args( T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, std::wstring method_name )
{
	check_args( t1, t2, t3, method_name );
	check_args( t4, t5, t6, method_name );
	check_args( t7, method_name );
}

inline void check_arg_count( long count, long expected_count, std::wstring method_name )
{
	if (count != expected_count)
	{
		throw Exception( L"Unexpected argument count in method " + method_name );
	}
}

} // namespace pcl_extension

} // namespace public_nbs