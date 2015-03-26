/// $Header
/// ============================================================================
///		Author		: M. Ivanchenko
///		Date create	: 23-03-2015
///		Date update	: 26-03-2015
///		Comment		:
/// ============================================================================
#include "ware_name_converter.h"

namespace rele_auto
{

/// ############################################################################
///			class ware_symbol_replacer
/// ############################################################################
    /// ========================================================================
    ///		CONSTRUCTORS/DESTRUCTOR
    /// ========================================================================

    /// ------------------------------------------------------------------------
	///	ware_symbol_replacer( )
    /// ------------------------------------------------------------------------
    ware_symbol_replacer::ware_symbol_replacer( )
    {
		this->initialize( );
    }

    /// ------------------------------------------------------------------------
	///	~ware_symbol_replacer( )
    /// ------------------------------------------------------------------------
    ware_symbol_replacer::~ware_symbol_replacer( )
    {
    }

    /// ========================================================================
    ///		OPERATORS
    /// ========================================================================
    /// ------------------------------------------------------------------------
	///	operator( )( const QChar &symbol ) const
    /// ------------------------------------------------------------------------
	QChar ware_symbol_replacer::operator( )( const QChar &symbol ) const
	{
		if( !symbol.isLetterOrNumber( ) )
		{
			return this->_SYLLABLE_DELIMETER_;
		}
		QChar ch = symbol;
		if( symbol.isLetter( ) )
		{
			ch = symbol.toUpper( );
			if( this->_symbols_table.contains( ch ) )
			{
				return this->_symbols_table[ch];
			}
		}
		return ch;
	}

    /// ========================================================================
    ///		FUNCTIONS
    /// ========================================================================
    /// ------------------------------------------------------------------------
	///	initialize( )
    /// ------------------------------------------------------------------------
	void ware_symbol_replacer::initialize( )
	{
		//init symblols exchange table
		//АВЕЁКМНОРСТУХ - symblols to replace
		QString s_replacing{QString::fromStdWString(L"АВЕЁКМНОРСТУХ")};
		//ABEEKMHOPCTYX - symbols to substitute
		QString s_subst{QString::fromStdWString(L"ABEEKMHOPCTYX")};

		auto it_k = s_replacing.begin( );
		auto it_v = s_subst.begin( );
		for( ; it_k != s_replacing.end( ); ++it_k, ++it_v )
		{
			this->_symbols_table.insert( *it_k, *it_v );
		}
	}

    /// ------------------------------------------------------------------------
	///	ware_symbol_replacer( )
    /// ------------------------------------------------------------------------
	ware_symbol_replacer& the_ware_symbol_replacer( )
	{
		static ware_symbol_replacer replacer;
		return replacer;
	}

/// ############################################################################
///			class ware_name_converter
/// ############################################################################

    /// ========================================================================
    ///		CONSTRUCTORS/DESTRUCTOR
    /// ========================================================================

    /// ------------------------------------------------------------------------
	///	ware_name_converter( )
    /// ------------------------------------------------------------------------
	ware_name_converter::ware_name_converter( )
		: _replacer(nullptr)
	{
		this->initialize( );
	}

    /// ------------------------------------------------------------------------
	///	~ware_name_converter( )
    /// ------------------------------------------------------------------------
	ware_name_converter::~ware_name_converter( )
	{
	}

    /// ========================================================================
    ///		FUNCTIONS
    /// ========================================================================
    /// ------------------------------------------------------------------------
	///	initialize( )
    /// ------------------------------------------------------------------------
	void ware_name_converter::initialize( )
	{
		this->_replacer = &( the_ware_symbol_replacer( ) );
	}

    /// ------------------------------------------------------------------------
	///	append_symbol( QChar &sym, QChar &prev_sym, QString &result ) const
	/// sym - symbol to be append
	/// prev_sym - symbol, previously appended to result. Needed to analyze
	///			   result and inserting syllable delimeter.
	/// result - result string, where symbol is appended
    /// ------------------------------------------------------------------------
	void ware_name_converter::append_symbol(
						const QChar &sym, const QChar &prev_sym, QString &result
											) const
	{
		if( !sym.isLetterOrNumber( ) && !prev_sym.isLetterOrNumber( ) )
		{
			//ignore two or more sequental delimeters
			return;
		}

		if( sym.isDigit( ) && prev_sym.isLetter( ) )
		{
			//letter->digit = new syllable
			result.append( this->_replacer->_SYLLABLE_DELIMETER_ );
		}

		if( sym.isLetter( ) && prev_sym.isDigit( ) )
		{
			//digit->letter = new syllable
			result.append( this->_replacer->_SYLLABLE_DELIMETER_ );
		}
		result.append( (*this->_replacer)( sym ) );
	}

    /// ------------------------------------------------------------------------
	///	convert( const QString &text ) const
    /// ------------------------------------------------------------------------
	QString ware_name_converter::convert( const QString &text ) const
	{
		QString result;

		QChar prev_qch = this->_replacer->_SYLLABLE_DELIMETER_;

		for( const QChar &qch : text )
		{
			this->append_symbol( qch, prev_qch, result );
			prev_qch = qch;
		}

		if( result.endsWith( this->_replacer->_SYLLABLE_DELIMETER_ ) )
		{
			//remove last symbol if it is delimeter
			result = result.remove( result.size( ) - 1, 1 );
		}

		return result;
	}

    /// ------------------------------------------------------------------------
	///	the_ware_name_converter( ) - singletone implementation
    /// ------------------------------------------------------------------------
	ware_name_converter& the_ware_name_converter( )
	{
		static ware_name_converter converter;
		return converter;
	}


/// ############################################################################
}//namespace rele_auto
