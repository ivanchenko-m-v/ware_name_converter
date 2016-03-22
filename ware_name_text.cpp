/// $Header
/// ============================================================================
///		Author		: M. Ivanchenko
///		Date create	: 18-03-2016
///		Date update	: 21-03-2016
///		Comment		:
/// ============================================================================

#include "ware_name_text.h"
#include "ware_name_converter.h"
#include <QRegExp>

namespace rele_auto
{

/// ############################################################################
///			class ware_name_text
/// ############################################################################

    /// ========================================================================
    ///		CONSTRUCTORS/DESTRUCTOR
    /// ========================================================================

    /// ------------------------------------------------------------------------
	///	ware_name_text( )
    /// ------------------------------------------------------------------------
	ware_name_text::ware_name_text( const QString &text ) :
		QString( text )
	{
		//initialize subst symbols
		this->init_symbol_tables( );
		//prepare symbols combinations
		this->prepare( );
	}

    /// ------------------------------------------------------------------------
	///	~ware_name_text( )
    /// ------------------------------------------------------------------------
	ware_name_text::~ware_name_text( )
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
	///	init_symbol_tables( )
    /// ------------------------------------------------------------------------
	void ware_name_text::init_symbol_tables( )
	{
		//replace letters
		this->init_letters_table( );
		//replace spec symbols
		this->init_spec_table( );
	}

    /// ------------------------------------------------------------------------
	///	init_letters_table( )
    /// ------------------------------------------------------------------------
	void ware_name_text::init_letters_table( )
	{
		//init symblols exchange table
		//АВЕКМНОРСТХ - symblols to replace
		QString s_replacing{QString::fromStdWString(L"АВЕКМНОРСТХ")};
		//ABEKMHOPCTX - symbols to substitute
		QString s_subst{QString::fromStdWString(L"ABEKMHOPCTX")};

		auto it_k = s_replacing.begin( );
		auto it_v = s_subst.begin( );
		for( ; it_k != s_replacing.end( ); ++it_k, ++it_v )
		{
			this->_letters_table.insert( *it_k, *it_v );
		}
	}

    /// ------------------------------------------------------------------------
	///	init_spec_table( )
    /// ------------------------------------------------------------------------
	void ware_name_text::init_spec_table( )
	{
		QString s_replacing{QString::fromStdWString(L"123456789PЗ")};
		//ABEEKMHOPCTYX - symbols to substitute
		QString s_subst[] = {
								QString::fromStdWString(L"o"), //1
								QString::fromStdWString(L"d"), //2
								QString::fromStdWString(L"t"), //3
								QString::fromStdWString(L"r"), //4
								QString::fromStdWString(L"p"), //5
								QString::fromStdWString(L"h"), //6
								QString::fromStdWString(L"s"), //7
								QString::fromStdWString(L"v"), //8
								QString::fromStdWString(L"e"), //9
								QString::fromStdWString(L"rz"),//Р
								QString::fromStdWString(L"zm") //З
							};

		auto it_k = s_replacing.begin( );
		auto it_v = s_subst;
		for( ; it_k != s_replacing.end( ); ++it_k, ++it_v )
		{
			this->_spec_table.insert( *it_k, *it_v );
		}
	}

    /// ------------------------------------------------------------------------
	///	prepare( )
    /// ------------------------------------------------------------------------
	void ware_name_text::prepare( )
	{
		//варианты с русской 'З'
		this->prepare_3( );
		//переводим строку в верхний регистр
		QString tmp_s = this->toUpper( );
		this->swap( tmp_s );
		//замена схожей кириллицы на латиницу
		this->replace_letters( );
		//замена специальных комбинаций
		this->replace_spec( );
		//замена небуквоцифр
		this->replace_not_alnum( );
	}

    /// ------------------------------------------------------------------------
	///	prepare_3( )
    /// ------------------------------------------------------------------------
	void ware_name_text::prepare_3( )
	{
		//русская буква 'З'
		QRegExp rx3( QString::fromStdWString( L"З" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = rx3.indexIn( *this );
		if( pos == -1 )
		{
			return;
		}
		//сочетание русских 'ЗЗ' перед 'Н' или 'П' (ЗЗН,ЗЗП)
		if( this->prepare_33H( ) )
		{
		}
		//сочетание русских 'Зв', 'За' итд
		else if( this->prepare_3v( ) )
		{
		}
		//сочетание 'ДЗТ'итд
		else if( this->prepare_D3T( ) )
		{
		}
		//сочетание '1"З"'итд
		else if( this->prepare_1_3( ) )
		{
		}
		//сочетание 'Э[AА]З'
		else if( this->prepare_A3( ) )
		{
		}
		//в остальных случаях замена русской 'З' на тройку
		this->replace( rx3, QString::fromStdWString( L"3" ) );
	}

    /// ------------------------------------------------------------------------
	///	prepare_33H( )
    /// ------------------------------------------------------------------------
	bool ware_name_text::prepare_33H( )
	{
		//сочетание русских 'ЗЗ' перед 'Н' или 'П' (ЗЗН,ЗЗП)
		QRegExp rx33H( QString::fromStdWString( L"ЗЗ(?=[HНП])" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = rx33H.indexIn( *this );
		if( pos == -1 )
		{
			return false;
		}
		//если найдено, меняем на маленькие 'зз'
		this->replace( rx33H, QString::fromStdWString( L"зз" ) );

		return true;
	}

    /// ------------------------------------------------------------------------
	///	prepare_3v( )
    /// ------------------------------------------------------------------------
	bool ware_name_text::prepare_3v( )
	{
		//сочетание русских 'Зв'итд
		QRegExp rx3v( QString::fromStdWString( L"З(?=[авНH])" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = rx3v.indexIn( *this );
		if( pos == -1 )
		{
			return false;
		}
		//если найдено, меняем на маленькие 'з'
		this->replace( rx3v, QString::fromStdWString( L"з" ) );

		return true;
	}

    /// ------------------------------------------------------------------------
	///	prepare_D3T( )
    /// ------------------------------------------------------------------------
	bool ware_name_text::prepare_D3T( )
	{
		//сочетание 'ДЗТ'
		QRegExp rxD3T( QString::fromStdWString( L"ДЗ[ТT]" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = rxD3T.indexIn( *this );
		if( pos == -1 )
		{
			return false;
		}
		//если найдено, меняем на маленькие 'з'
		this->replace( rxD3T, QString::fromStdWString( L"ДзТ" ) );

		return true;
	}

    /// ------------------------------------------------------------------------
	///	prepare_1_3( )
    /// ------------------------------------------------------------------------
	bool ware_name_text::prepare_1_3( )
	{
		//сочетание '1"З"'итд
		QRegExp rx1_3( QString::fromStdWString( L"(?<=[1-9])[\"'«]?З" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = rx1_3.indexIn( *this );
		if( pos == -1 )
		{
			return false;
		}
		//если найдено, меняем на маленькие 'з'
		this->replace( rx1_3, QString::fromStdWString( L"з" ) );

		return true;
	}

    /// ------------------------------------------------------------------------
	///	prepare_A3( )
    /// ------------------------------------------------------------------------
	bool ware_name_text::prepare_A3( )
	{
		//сочетание 'Э[AА]З'
		QRegExp rxA3( QString::fromStdWString( L"Э[AА]З" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = rxA3.indexIn( *this );
		if( pos == -1 )
		{
			return false;
		}
		//если найдено, меняем на маленькие 'ЭАз'
		this->replace( rxA3, QString::fromStdWString( L"ЭАз" ) );

		return true;
	}

    /// ------------------------------------------------------------------------
	///	replace_letters( )
    /// ------------------------------------------------------------------------
	void ware_name_text::replace_letters( )
	{
		for( QChar &symbol : *this )
		{
			if( symbol.isLetter( ) )
			{
				if( this->_letters_table.contains( symbol ) )
				{
					symbol = this->_letters_table[symbol];
				}
			}
		}
	}

    /// ------------------------------------------------------------------------
	///	replace_spec( )
    /// ------------------------------------------------------------------------
	void ware_name_text::replace_spec( )
	{
		QRegExp rx( QString::fromStdWString( L"([1-9])[\"'«]?([PЗ])" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = rx.indexIn( *this );
		while( pos != -1 )
		{
			QString cap1{rx.cap(1)};
			QString cap2{rx.cap(2)};
			QString xchng_str = this->_spec_table[cap1[0]] + this->_spec_table[cap2[0]];
			this->replace( pos, rx.matchedLength( ), xchng_str );

			pos = rx.indexIn( *this );
		}
	}

    /// ------------------------------------------------------------------------
	///	replace_not_alnum( )
    /// ------------------------------------------------------------------------
	void ware_name_text::replace_not_alnum( )
	{
		QRegExp rx( QString::fromStdWString( L"\\W+" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		this->replace( rx, QString::fromStdWString( L":" ) );
	}


/// ############################################################################
}//namespace rele_auto
