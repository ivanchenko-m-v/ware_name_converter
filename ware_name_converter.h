/// $Header
/// ============================================================================
///		Author		: M. Ivanchenko
///		Date create	: 23-03-2015
///		Date update	: 25-03-2015
///		Comment		:
/// ============================================================================
#ifndef __WARE_NAME_CONVERTER_H__
#define __WARE_NAME_CONVERTER_H__

#include <QHash>

namespace rele_auto
{
	typedef QHash<QChar, QChar> ware_symbol_replace_table;
/// ############################################################################
///			class ware_symbol_replacer
/// ############################################################################
    class ware_symbol_replacer
	{
    /// ========================================================================
    ///		CONSTRUCTORS/DESTRUCTOR
    /// ========================================================================
    private:
        /// --------------------------------------------------------------------
        ware_symbol_replacer( const ware_symbol_replacer &rhs ) = delete;
        /// --------------------------------------------------------------------
        ware_symbol_replacer( );

	public:
        /// --------------------------------------------------------------------
        virtual ~ware_symbol_replacer( );
        /// --------------------------------------------------------------------
		friend ware_symbol_replacer& the_ware_symbol_replacer( );

    /// ========================================================================
    ///		FUNCTIONS
    /// ========================================================================
    private:
        /// --------------------------------------------------------------------
		void initialize( );

	public:

    /// ========================================================================
    ///		OPERATORS
    /// ========================================================================
    private:
        /// --------------------------------------------------------------------
        ware_symbol_replacer& operator=( const ware_symbol_replacer &rhs ) = delete;

	public:
		QChar operator( )( const QChar &symbol ) const;

    /// ========================================================================
    ///			FIELDS
    /// ========================================================================
    public:
		//разделитель слогов
		const QChar _SYLLABLE_DELIMETER_ = ':';

    private:
		//хэш-таблица замены символов:
		//key - символ для замены, value - подстановочный символ
		ware_symbol_replace_table	_symbols_table;

	};//class ware_symbol_replacer

	/// ------------------------------------------------------------------------
	ware_symbol_replacer& the_ware_symbol_replacer( );

/// ############################################################################
/// ----------------------------------------------------------------------------


/// ############################################################################
///			class ware_name_converter
/// ############################################################################
    class ware_name_converter
	{
    /// ========================================================================
    ///		CONSTRUCTORS/DESTRUCTOR
    /// ========================================================================
    private:
        /// --------------------------------------------------------------------
        ware_name_converter( const ware_name_converter &rhs ) = delete;
        /// --------------------------------------------------------------------
        ware_name_converter( );

	public:
        /// --------------------------------------------------------------------
        virtual ~ware_name_converter( );
        /// --------------------------------------------------------------------
		friend ware_name_converter& the_ware_name_converter( );

    /// ========================================================================
    ///		FUNCTIONS
    /// ========================================================================
    private:
        /// --------------------------------------------------------------------
		void initialize( );
        /// --------------------------------------------------------------------
		void append_symbol(
							const QChar &sym, const QChar &prev_sym,
							QString &result
						   ) const;

	public:
		QString convert( const QString &text ) const;

    /// ========================================================================
    ///		OPERATORS
    /// ========================================================================
    private:
        /// --------------------------------------------------------------------
        ware_name_converter& operator=( const ware_name_converter &rhs ) = delete;

    /// ========================================================================
    ///			FIELDS
    /// ========================================================================
    private:
		//инструмент посимвольной замены
		ware_symbol_replacer	*_replacer;

	};//class ware_name_converter

	/// ------------------------------------------------------------------------
	ware_name_converter& the_ware_name_converter( );
/// ############################################################################
/// ----------------------------------------------------------------------------

}//namespace rele_auto
#endif // __WARE_NAME_CONVERTER_H__
