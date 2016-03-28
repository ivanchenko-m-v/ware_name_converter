#include <QCoreApplication>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDateTime>

#include "ware_name_text.h"

void parse_file( );

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	parse_file( );

	return a.exec( );
}

void parse_file( )
{
     QFile file_in("ware_names.txt");
     if (!file_in.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         return;
     }
     QFile file_out("ware_names_idx.txt");
     if (!file_out.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
     {
         return;
     }

     QTextStream in(&file_in);
     in.setCodec("UTF-8");
     QTextStream out(&file_out);
     out.setCodec("UTF-8");
     out << QDateTime::currentDateTime().toString();
     endl(out);
     while( !in.atEnd( ) )
     {
		//читаем
		QString ware_name{in.readLine()};
		rele_auto::ware_name_text ware_idx( ware_name );
        //пишем
        out << ware_name << ";" << ware_idx;
        endl(out);
     }
     out << QDateTime::currentDateTime().toString();
     endl(out);

     file_in.close( );
     file_out.close( );
}
