#include "io.h"

IO::IO() {}

IO::~IO() {}

QStringList IO::get_blast_query_db_list(QString &dir) {
  QDir qdir(dir);
  qdir = qdir.filePath("blast_results_query");
  QStringList filters;
  filters << "*.db";
  qdir.setNameFilters(filters);
  QStringList absolute_paths;
  foreach (QString s, qdir.entryList(QDir::Files)) {
    absolute_paths.append(qdir.absoluteFilePath(s));
  }
  return absolute_paths;
}

QString IO::create_gene_html_text(QStringList &gene_info) {
  int start = gene_info.at(2).toInt() - 1;
  int stop = gene_info.at(3).toInt();
  QString html_string =
      "<html><body>> " + gene_info.at(7) + " (<b>" + gene_info.at(1) +
      "</b>) on chromosome <b>" + gene_info.at(6) + "</b> (" + gene_info.at(5) +
      ") ORF Start : <b>" + gene_info.at(2) + "</b>, ORF Stop : <b>" +
      gene_info.at(3) + "</b><br><br><font color = 'silver'>";
  html_string += gene_info.at(4).left(start) + "</font>";
  html_string += gene_info.at(4).mid(start, stop);
  html_string +=
      "<font color='silver'>" + gene_info.at(4).right(stop) + "</font>";
  html_string += "</body></html>";
  return html_string;
}
