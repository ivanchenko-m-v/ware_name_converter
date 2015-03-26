/// $Header
/// ============================================================================
///		Author		: M. Ivanchenko
///		Date create	: 23-03-2015
///		Date update	: 25-03-2015
///		Comment		:
/// ============================================================================
#include "ware_name_converter.h"

namespace rele_auto
{

	typedef QVector<QChar> vector_char;
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
	///	ware_symbol_replacer( )
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
		/*
		vector_char replace_symbols({QChar('А'),QChar('В'),QChar('Е'),QChar('Ё'),
									 QChar('К'),QChar('М'),QChar('Н'),QChar('О'),
									 QChar('Р'),QChar('С'),QChar('Т'),QChar('У'),QChar('Х')});
		vector_char subst_symbols({QChar('A'),QChar('B'),QChar('E'),QChar('E'),
								   QChar('K'),QChar('M'),QChar('H'),QChar('O'),
								   QChar('P'),QChar('C'),QChar('T'),QChar('Y'),QChar('X')});
		for(int index = 0; index < replace_symbols.size( ); ++index )
		{
			this->_symbols_table.insert( replace_symbols[index], subst_symbols[index] );
		}
								   */
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
    ///		PROPERTIES
    /// ========================================================================

    /// ========================================================================
    ///		OPERATORS
    /// ========================================================================

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

    /*
     * 1st version
	QString ware_name_converter::convert( const QString &text ) const
	{
		QString result;
		const ware_symbol_replacer &replacer = *( this->_replacer );

		bool is_letter = false;
		bool is_digit = false;
		bool is_delim = true;
		for( const QChar &qch : text )
		{
			if( !qch.isLetterOrNumber( ) )
			{
				if( is_delim )
				{
					//ignore sequental delimeters
					continue;
				}
				is_delim = true;
				is_letter = false;
				is_digit = false;
			}
			if( qch.isDigit( ) )
			{
				if( is_letter )
				{
					//letter->digit = new syllable
					result.append( replacer._SYLLABLE_DELIMETER_ );
					is_letter = false;
				}
				is_digit = true;
				is_delim = false;
			}
			if( qch.isLetter( ) )
			{
				if( is_digit )
				{
					//digit->letter = new syllable
					result.append( replacer._SYLLABLE_DELIMETER_ );
					is_digit = false;
				}
				is_letter = true;
				is_delim = false;
			}
			result.append( replacer( qch ) );
		}

		return result;
	}
	*/
/*
 * 2nd version
	QString ware_name_converter::convert( const QString &text ) const
	{
		QString result;
		const ware_symbol_replacer &replacer = *( this->_replacer );

		QChar prev_qch = replacer._SYLLABLE_DELIMETER_;

		for( const QChar &qch : text )
		{
			if( !qch.isLetterOrNumber( ) && !prev_qch.isLetterOrNumber( ) )
			{
				//ignore sequental delimeters
				continue;
			}
			if( qch.isDigit( ) && prev_qch.isLetter( ) )
			{
				//letter->digit = new syllable
				result.append( replacer._SYLLABLE_DELIMETER_ );
			}
			if( qch.isLetter( ) && prev_qch.isDigit( ) )
			{
				//digit->letter = new syllable
				result.append( replacer._SYLLABLE_DELIMETER_ );
			}
			result.append( replacer( qch ) );
			prev_qch = qch;
		}
		if( result.endsWith( replacer._SYLLABLE_DELIMETER_ ) )
		{
			//remove last symbol if it is delimeter
			result = result.remove( result.size( ) - 1, 1 );
		}

		return result;
	}
	*/
    /// ------------------------------------------------------------------------
	///	convert_symbol( QChar &sym, QChar &prev_sym, QString &result ) const
    /// ------------------------------------------------------------------------
	void ware_name_converter::convert_symbol(
						const QChar &sym, const QChar &prev_sym, QString &result
											) const
	{
		const ware_symbol_replacer &replacer = *( this->_replacer );

		if( !sym.isLetterOrNumber( ) && !prev_sym.isLetterOrNumber( ) )
		{
			//ignore sequental delimeters
			return;
		}

		if( sym.isDigit( ) && prev_sym.isLetter( ) )
		{
			//letter->digit = new syllable
			result.append( replacer._SYLLABLE_DELIMETER_ );
		}

		if( sym.isLetter( ) && prev_sym.isDigit( ) )
		{
			//digit->letter = new syllable
			result.append( replacer._SYLLABLE_DELIMETER_ );
		}
		result.append( replacer( sym ) );
	}

    /// ------------------------------------------------------------------------
	///	convert( const QString &text ) const
    /// ------------------------------------------------------------------------
	QString ware_name_converter::convert( const QString &text ) const
	{
		QString result;
		const ware_symbol_replacer &replacer = *( this->_replacer );

		QChar prev_qch = replacer._SYLLABLE_DELIMETER_;

		for( const QChar &qch : text )
		{
			this->convert_symbol( qch, prev_qch, result );
			prev_qch = qch;
		}

		if( result.endsWith( replacer._SYLLABLE_DELIMETER_ ) )
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
