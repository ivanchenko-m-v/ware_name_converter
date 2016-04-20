
/// $Header
/// ============================================================================
///		Author		: M. Ivanchenko
///		Date create	: 18-03-2016
///		Date update	: 14-04-2016
///		Comment		:
/// ============================================================================

#include "ware_name_text.h"
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
		QString tmp_s{this->toUpper( )};
		this->swap( tmp_s );
		//замена схожей кириллицы на латиницу
		this->replace_letters( );
		//замена специальных комбинаций
		this->replace_spec( );
		//замена небуквоцифр
		this->replace_not_alnum( );
		//разделение по "слогам"
		this->insert_syllable_delimeters( );
		//удаление лишних разделителей
		this->cut_extra_delimeters( );
		//уменьшить длину слога до MAX=6
		this->cut_syllable_length( );
		//удалить дубликаты слогов
		this->remove_duplicates( );
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
		QRegExp rx1_3( QString::fromStdWString( L"([1-9])[\"'«]?З" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = rx1_3.indexIn( *this );
		if( pos == -1 )
		{
			return false;
		}
		//если найдено, меняем на маленькие 'з'
		this->replace( rx1_3, QString::fromStdWString( L"\\1з" ) );

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
			if( symbol.isLetter( ) && this->_letters_table.contains( symbol ) )
			{
				symbol = this->_letters_table[symbol];
			}
		}
	}

    /// ------------------------------------------------------------------------
	///	replace_spec( )
    /// ------------------------------------------------------------------------
	void ware_name_text::replace_spec( )
	{
		QRegExp rx( QString::fromStdWString( L"([1-9])[\"'«]?([PЗ])" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		int pos = -1;
		while( (pos = rx.indexIn( *this ))!= -1 )
		{
			this->replace( pos, rx.matchedLength( ), this->_spec_table[rx.cap(1)[0]] + this->_spec_table[rx.cap(2)[0]] + ":" );
		}
	}

    /// ------------------------------------------------------------------------
	///	replace_not_alnum( )
    /// ------------------------------------------------------------------------
	void ware_name_text::replace_not_alnum( )
	{
		QRegExp rx( QString::fromStdWString( L"\\W+" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		this->replace( rx, this->_SYLLABLE_DELIMETER );
	}

    /// ------------------------------------------------------------------------
	///	insert_syllable_delimeters( )
    /// ------------------------------------------------------------------------
	void ware_name_text::insert_syllable_delimeters( )
	{
		//вставляем разделитель в последовательность буквы-цифры
		this->insert_syllable_notnum_num( );
		//вставляем разделитель в последовательность цифры-буквы
		this->insert_syllable_num_notnum( );
	}

    /// ------------------------------------------------------------------------
	///	insert_syllable_notnum_num( )
    /// ------------------------------------------------------------------------
	void ware_name_text::insert_syllable_notnum_num( )
	{
		QRegExp rx( QString::fromStdWString( L"(\\D+)(?=\\d+)" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		if( rx.indexIn(*this) != -1 )
		{
			//вставляем разделитель между последовательностью нецифр и цифр
			this->replace( rx, QString("\\1") + this->_SYLLABLE_DELIMETER );
		}
	}

    /// ------------------------------------------------------------------------
	///	insert_syllable_num_notnum( )
    /// ------------------------------------------------------------------------
	void ware_name_text::insert_syllable_num_notnum( )
	{
		QRegExp rx( QString::fromStdWString( L"(\\d+)(?=\\D+)" ), Qt::CaseSensitive, QRegExp::RegExp2 );
		if( rx.indexIn(*this) != -1 )
		{
			//вставляем разделитель между последовательностью цифр и нецифр
			this->replace( rx, QString("\\1") + this->_SYLLABLE_DELIMETER  );
		}
	}

    /// ------------------------------------------------------------------------
	///	cut_extra_delimeters( )
    /// ------------------------------------------------------------------------
	void ware_name_text::cut_extra_delimeters( )
	{
		QRegExp rx(
					QString::fromStdWString( L"[" + QString(this->_SYLLABLE_DELIMETER).toStdWString( ) + L"]+" ),
											 Qt::CaseSensitive, QRegExp::RegExp2
										   );
		if( rx.indexIn(*this) != -1 )
		{
			this->replace( rx, QString( this->_SYLLABLE_DELIMETER ) );
		}
		if( this->startsWith(this->_SYLLABLE_DELIMETER) )
		{
			//удаляем, если в начале стоит разделитель
			this->remove( 0, 1 );
		}
		if( this->endsWith(this->_SYLLABLE_DELIMETER) )
		{
			//удаляем, если в конце стоит разделитель
			this->remove( this->length( )-1, 1 );
		}
	}

    /// ------------------------------------------------------------------------
	///	cut_syllable_length( )
    /// ------------------------------------------------------------------------
	void ware_name_text::cut_syllable_length( )
	{
		QString rgx_pattern_begin{"([^" + QString(this->_SYLLABLE_DELIMETER) + "]{"};
		QString rgx_pattern_end{",})"};
		//([^:]{7,}) - недвоеточие повторяющееся не менее 7и(или (MAX_SYLLABLE_LEN+1)) раз
		QRegExp rx( QString::fromStdWString(
							rgx_pattern_begin.toStdWString( ) +
							QString::number(
											this->_MAX_SYLLABLE_LEN + 1).toStdWString( ) +
											rgx_pattern_end.toStdWString( )
										   ),
							Qt::CaseSensitive, QRegExp::RegExp2
										   );
		int pos = -1;
		while( (pos = rx.indexIn( *this ))!= -1 )
		{
			//уменьшить длину слога до MAX=6
			this->replace( pos, rx.matchedLength( ), rx.cap(1).left(this->_MAX_SYLLABLE_LEN) );
		}
	}

    /// ------------------------------------------------------------------------
	///	remove_duplicates( )
    /// ------------------------------------------------------------------------
	void ware_name_text::remove_duplicates( )
	{
		QStringList list_syl = this->split( this->_SYLLABLE_DELIMETER );
		list_syl.removeDuplicates( );

		//собираем строку без дубликатов
		this->clear( );
		for( QString syl :list_syl )
		{
			this->append( syl );
			this->append( this->_SYLLABLE_DELIMETER );
		}
		if( this->endsWith(this->_SYLLABLE_DELIMETER) )
		{
			//удаляем разделитель в конце
			this->remove( this->length( )-1, 1 );
		}
	}

/// ############################################################################
}//namespace rele_auto
