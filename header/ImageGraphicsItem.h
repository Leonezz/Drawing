#pragma once
#include <qgraphicsitem.h>
class ImageGraphicsItem : public QObject,public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	ImageGraphicsItem();
	~ImageGraphicsItem() = default;


	
};

