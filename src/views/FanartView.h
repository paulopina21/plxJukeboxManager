#ifndef FANARTVIEW_H
#define FANARTVIEW_H

#include "guiview.h"

class ListModel;
class CTheAudioDB;

class CFanartView: public CGUIView
{
  Q_OBJECT
  static CFanartView* INSTANCE;
public:
  CFanartView(QObject* parent = 0);
  static CFanartView* instance() { if (!INSTANCE) new CFanartView(); return INSTANCE; }


public slots:
  virtual void load() {}
  virtual void unload() {}

  ListModel* getFanarts(const QString& strArtist);
  void getFanart(const QString& strArtist);

private:
  ListModel* m_fanarts;
  CTheAudioDB* m_scrapper;
};

#endif // FANARTVIEW_H
