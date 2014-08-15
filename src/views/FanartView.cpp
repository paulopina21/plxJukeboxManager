#include "FanartView.h"

#include <plxFramework/models/ListModel.h>

#include "../media/FanartItem.h"
#include "../frameworks/scraping/providers/theaudiodb.h"

CFanartView* CFanartView::INSTANCE = 0;

CFanartView::CFanartView(QObject* parent): CGUIView(parent)
{
  INSTANCE = this;
  m_fanarts = new ListModel(new CFanartItem(), this);
}


ListModel* CFanartView::getFanarts(const QString& strArtist)
{

}

void CFanartView::getFanart(const QString& strArtist)
{

}

