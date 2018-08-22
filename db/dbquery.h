#ifndef DBQUERY_H
#define DBQUERY_H

#include "imports.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

class DBQuery {
public:
  DBQuery();
  QStringList get_gene_accession_list(QString name);
  QStringList get_accession_list_for_gene(QString name, QString gene);
  QStringList get_info_for_accession_number(QString name,
                                            QString accession_number);
  QVector<QStringList> get_junction_list(QString name, QString gene_name,
                                         QString accession_number);
  QVector<double> get_stats(QString name, QString gene_name,
                            QString accession_number);
  QVector<QStringList> get_squashed_list(QString name, QString gene_name,
                                         QString accession_number);
};

#endif // DBQUERY_H
