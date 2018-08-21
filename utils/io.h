#ifndef IO_H
#define IO_H

#include "imports.h"

class IO {

public:
  IO();
  ~IO();
  QStringList get_blast_query_db_list(QString &dir);
  QString create_gene_html_text(QStringList &gene_info);

private:
};

#endif // IO_H
