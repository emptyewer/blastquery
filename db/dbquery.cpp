#include "dbquery.h"

DBQuery::DBQuery() {}

QStringList DBQuery::get_gene_accession_list(QString name) {
  QSqlDatabase db = QSqlDatabase::database(name);
  QSqlQuery query("SELECT gene_name,nm_number FROM gene", db);
  int gname = query.record().indexOf("gene_name");
  int accession = query.record().indexOf("nm_number");
  QStringList list;
  while (query.next()) {
    QString gene_name = query.value(gname).toString();
    QString accession_number = query.value(accession).toString();
    list.append(gene_name);
    list.append(accession_number);
  }
  return list;
}

QStringList DBQuery::get_accession_list_for_gene(QString name, QString gene) {
  QSqlDatabase db = QSqlDatabase::database(name);
  QSqlQuery query("SELECT nm_number FROM gene WHERE gene_name=\'" + gene + "\'",
                  db);
  QStringList list;
  while (query.next()) {
    QString accession_number = query.value(0).toString();
    list.append(accession_number);
  }
  return list;
}

QStringList DBQuery::get_info_for_accession_number(QString name,
                                                   QString accession_number) {
  QSqlDatabase db = QSqlDatabase::database(name);
  QSqlQuery query(
      "SELECT * FROM gene WHERE nm_number=\'" + accession_number + "\'", db);
  QStringList result;
  if (!query.isValid()) {
    query.first();
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); i++) {
      result << record.value(i).toString();
    }
  }
  return result;
}

QVector<QStringList> DBQuery::get_junction_list(QString name, QString gene_name,
                                                QString accession_number) {
  QSqlDatabase db = QSqlDatabase::database(name);
  QSqlQuery query(
      "SELECT "
      "junction.position,junction.ppm,junction.query_start,junction.frame,"
      "junction.orf FROM junction INNER JOIN gene ON gene.gene_name=\'" +
          gene_name + "\' AND gene.nm_number=\'" + accession_number +
          "\' AND junction.gene_id=gene.id",
      db);
  QVector<QStringList> result;
  if (!query.isValid()) {
    while (query.next()) {
      QStringList junc;
      QSqlRecord record = query.record();
      for (int i = 0; i < record.count(); i++) {
        if (i == 1) {
          junc.append(QString::number(record.value(i).toDouble(), 'g'));
        } else {
          junc.append(record.value(i).toString());
        }
      }
      result.append(junc);
    }
  }
  return result;
}

QVector<double> DBQuery::get_stats(QString name, QString gene_name,
                                   QString accession_number) {
  QSqlDatabase db = QSqlDatabase::database(name);
  QSqlQuery query("SELECT stats.in_frame, stats.in_orf, stats.inframe_inorf, "
                  "stats.not_in_frame, stats.intron, stats.backwards, "
                  "stats.upstream, stats.downstream from stats inner join gene "
                  "on gene.id=stats.gene_id and gene.gene_name=\'" +
                      gene_name + "\' and gene.nm_number=\'" +
                      accession_number + "\'",
                  db);
  QVector<double> result;
  if (!query.isValid()) {
    query.first();
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); i++) {
      result << record.value(i).toDouble();
    }
  }
  return result;
}

QVector<QStringList> DBQuery::get_squashed_list(QString name, QString gene_name,
                                                QString accession_number) {
  QSqlDatabase db = QSqlDatabase::database(name);
  QSqlQuery query(
      "SELECT junction.position, SUM(junction.ppm ), "
      "junction.frame, junction.orf FROM junction INNER JOIN gene ON "
      "junction.gene_id=gene.id AND gene.gene_name=\'" +
          gene_name +
          "\' AND "
          "gene.nm_number=\'" +
          accession_number + "\'  GROUP BY junction.position",
      db);

  QVector<QStringList> result;
  if (!query.isValid()) {
    while (query.next()) {
      QStringList junc;
      QSqlRecord record = query.record();
      for (int i = 0; i < record.count(); i++) {
        if (i == 1) {
          junc.append(QString::number(record.value(i).toDouble(), 'g'));
        } else {
          junc.append(record.value(i).toString());
        }
      }
      result.append(junc);
    }
  }
  return result;
}
