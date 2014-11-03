#ifndef VPPROVIDER_H
#define VPPROVIDER_H

#include <QGraphicsProxyWidget>
#include "vpabstractnode.h"

class VPTerminal : public QGraphicsProxyWidget, public VPAbstractNode
{
public:
	//////////////////////////////////////////////////////////////////////////
	// ���ͺ�ö��
	//////////////////////////////////////////////////////////////////////////
	enum { Type = UserType + 12 };

	// win7	���ڱ߿���6
	//		���ڱ��������23	
	enum 
	{ 
		WindowFrameWidth = 6,
		WindowHeadWidth = 23
	};

	enum TYPE
	{
		PROVIDER = 0,
		CONSUMER = 1
	};

	enum { Diameter = 20 };

	//////////////////////////////////////////////////////////////////////////
	// ���캯������������
	//////////////////////////////////////////////////////////////////////////
	VPTerminal(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = Qt::Window);

	virtual ~VPTerminal();

	//////////////////////////////////////////////////////////////////////////
	// ��д�麯��
	//////////////////////////////////////////////////////////////////////////
	
	// ��дQGraphicsProxyWidget���麯��
	QRectF	boundingRect() const;
	QPainterPath shape() const;
	int		type() const	{ return Type;}

	// ��дVPAbstractNode�麯��
	VPConnector*	getConnector(const QPointF& sPos);
	QString			getNodeTips(const QPointF& sPos);
	QPointF	scenePosition() const;

	virtual bool execute() = 0;

	void setupProcess(AProcess* proc, int id);
	
	void setType(TYPE t) { _type = t; }
	TYPE getType() const { return _type; }
	
protected:
	
	void updateSlots();

	void setupProxyWidget(QWidget* proxyW);

	void closeEvent( QCloseEvent *e );

	void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget){}

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	TYPE _type;

	int roundness(double size) const;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif