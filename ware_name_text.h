/// $Header
/// ============================================================================
///		Author		: M. Ivanchenko
///		Date create	: 18-03-2016
///		Date update	: 23-03-2016
///		Comment		:
/// ============================================================================
#ifndef __WARE_NAME_TEXT_H__
#define __WARE_NAME_TEXT_H__

#include <QString>
#include <QHash>

namespace rele_auto
{
	typedef QHash<QChar, QChar> ware_symbol_replace_table;
	typedef QHash<QChar, QString> ware_specsymbol_replace_table;

/// ############################################################################
///			class ware_name_text
/// ############################################################################
    class ware_name_text : public QString
	{
    /// ========================================================================
    ///		CONSTRUCTORS/DESTRUCTOR
    /// ========================================================================
    private:
        /// --------------------------------------------------------------------
        ware_name_text( const ware_name_text &rhs );

	public:
        /// --------------------------------------------------------------------
        explicit ware_name_text( const QString &text );
        /// --------------------------------------------------------------------
        virtual ~ware_name_text( );

    /// ========================================================================
    ///		FUNCTIONS
    /// ========================================================================
    private:
		void init_symbol_tables( );
		void init_letters_table( );
		void init_spec_table( );

		void prepare( );
		void prepare_3( );
		bool prepare_33H( );
		bool prepare_3v( );
		bool prepare_D3T( );
		bool prepare_1_3( );
		bool prepare_A3( );

		void replace_letters( );
		void replace_spec( );
		void replace_not_alnum( );
		void insert_syllable_delimeters( );
		void insert_syllable_notnum_num( );
		void insert_syllable_num_notnum( );
		void cut_extra_delimeters( );
		void cut_syllable_length( );

	public:

    /// ========================================================================
    ///		PROPERTIES
    /// ========================================================================
    private:

	public:

    /// ========================================================================
    ///		OPERATORS
    /// ========================================================================
    private:
        /// --------------------------------------------------------------------
        ware_name_text& operator=( const ware_name_text &rhs );

	public:

    /// ========================================================================
    ///			FIELDS
    /// ========================================================================
	private:
		const int MAX_SYLLABLE_LEN = 6;

		//хэш-таблица замены символов:
		//key - символ для замены, value - подстановочный символ
		ware_symbol_replace_table	_letters_table;
		//хэш-таблица замены специальных символов:
		//key - символ для замены, value - подстановочный символ
		ware_specsymbol_replace_table	_spec_table;

	};//class ware_name_text
/// ############################################################################
/// ----------------------------------------------------------------------------

}//namespace rele_auto
#endif // __WARE_NAME_TEXT_H__
