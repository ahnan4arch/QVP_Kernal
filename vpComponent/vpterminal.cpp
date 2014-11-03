#include <QtGui>

#include "vpterminal.h"
#include "vpconnector.h"

#include "../em/agraph_exec.hh"

//////////////////////////////////////////////////////////////////////////
//	VPConsumer
//	���캯������һ��proxyWidget����������Provider��Rect
//////////////////////////////////////////////////////////////////////////

VPTerminal::VPTerminal( QGraphicsItem *parent, Qt::WindowFlags wFlags)
		   : QGraphicsProxyWidget(parent, wFlags)
		   , VPAbstractNode()
		   , _type(PROVIDER)
{
	setProcess(0);
	setInSlotCount(0);
	setOutSlotCount(0);
	setText("Ternimal");

	setHeadColor(QColor(65,105,225));
	VPAbstractNode::setFont( QFont("Verdana", 15, QFont::Normal) );

	setAttribute(Qt::WA_DeleteOnClose);		
	//������closeEvent�����Ժ󣬻ᷢ��Ϣɾ�������ڣ���������VPAbstractNode����������
	//������closeEvent��û�б�Ҫdestroy
}

VPTerminal::~VPTerminal()
{
}

// reimplementation virtual functions:
QRectF	VPTerminal::boundingRect() const
{
	int slotSize = VPAbstractNode::SlotSize;

	if ( getType()==PROVIDER )
		return QGraphicsProxyWidget::boundingRect().adjusted(0, 0, slotSize, 0);

	return QGraphicsProxyWidget::boundingRect().adjusted(-slotSize, 0, 0, 0);
}	

QPainterPath VPTerminal::shape() const
{
	QPainterPath path = QGraphicsProxyWidget::shape();

	foreach (VPConnector* slot, inputSlots)
		path.addRect( slot->area() );

	return path;
}


// reimplementation VPAbstractNode's virtual functions:
VPConnector* VPTerminal::getConnector(const QPointF& sPos)
{
	return VPAbstractNode::getConnector( QGraphicsItem::mapFromScene(sPos) );	
}

QString
	VPTerminal::getNodeTips(const QPointF& sPos)
{
	return VPAbstractNode::getNodeTips( QGraphicsItem::mapFromScene(sPos) );	
}

QPointF VPTerminal::scenePosition() const
{
	// return the scene position
	return QGraphicsItem::pos();
}

// �ն��в���ʾ�˿ڵ�����
void VPTerminal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
	// ����myRect�Ͷ˿�λ��
	updateSlots();			

	painter->setRenderHint(QPainter::Antialiasing);

	//////////////////////////////////////////////////////////////////////////
	// draw body color
	painter->setPen(Qt::NoPen);
	painter->setBrush( QColor(167,192,220) );
	painter->drawRoundRect( myRect, roundness( myRect.width()), roundness( myRect.height()));

	//////////////////////////////////////////////////////////////////////////
	// draw proxyed widget
	QGraphicsProxyWidget::paint(painter, option, w);

	//////////////////////////////////////////////////////////////////////////
	// draw head area
	QRectF headArea( myRect.left(), myRect.top(), 
					 myRect.width(), WindowHeadWidth+1);
	painter->setPen( Qt::NoPen );

	float Right = headArea.right();
	float Top = headArea.top();
	float Left = headArea.left();
	float Bottom = headArea.bottom();
	float Radius = 20;

	QPainterPath upRoundRectPath;
	upRoundRectPath.moveTo(Right, Top + Radius/2.0);
	upRoundRectPath.arcTo(Right - Radius, Top, Radius, Radius, 0.0, 90.0);
	upRoundRectPath.lineTo(Left + Radius/2.0, Top);
	upRoundRectPath.arcTo(Left, Top, Radius, Radius, 90.0, 90.0);
	upRoundRectPath.lineTo(Left, Bottom);
	upRoundRectPath.lineTo(Right, Bottom);
	upRoundRectPath.closeSubpath();

	painter->setPen(Qt::NoPen);
	painter->fillPath(upRoundRectPath, QColor(167,192,220));

	//////////////////////////////////////////////////////////////////////////
	// draw head text
	painter->setPen(Qt::black);
	painter->setFont( VPAbstractNode::font() );
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawText( myRect, Qt::AlignHorizontal_Mask & Qt::AlignLeft, text());

	//////////////////////////////////////////////////////////////////////////
	// draw slots
	renderSlot(painter);
}

void VPTerminal::closeEvent( QCloseEvent *e )
{
	e->ignore();
}

//////////////////////////////////////////////////////////////////////////
//	��ͬ�ڴ�������Processor���նˣ�Terminal���ĳߴ���Ըı�
//	���û��ı��ն˽��Ŀ���Ժ���Ҫ����myRect�Ͷ˿�λ��
//////////////////////////////////////////////////////////////////////////
void VPTerminal::updateSlots()
{
	//�����ڳߴ�ı��Ժ󣬸��½ŵ��λ��
	myRect = widget()->rect().adjusted(-WindowFrameWidth, -(WindowFrameWidth+WindowHeadWidth), 
								/*VPAbstractNode::SlotSize + */WindowFrameWidth, WindowFrameWidth);


	//redraw output connector's offset
	for (int i(0); i<myInCount; i++)
		inputSlots[i]->setOffset( slotOffset(i, IN) );

	//redraw input connector's offset
	for (int i(0); i<myOutCount; i++)
		outputSlots[i]->setOffset( slotOffset(i, OUT) );

	// redraw input connector's links~
	foreach (ConnectorTypePtr slot, outputSlots)
		slot->redraw();

	// redraw output connector's links~
	foreach (ConnectorTypePtr slot, inputSlots)
		slot->redraw();		
}

void VPTerminal::setupProcess(AProcess* proc, int id)
{
	if (!proc || getProcess() 
		|| -1!=getId() || -1==id)
		return;

	// �趨�ն��ڲ��߼�����
	setProcess(proc);

	// �趨�ն˵�ID
	setId(id);

	// �趨�ն˵�����
	setText( QString(getProcess()->name().c_str()) );	

	// �趨�ն˵�����˿ڸ���
	setInSlotCount( getProcess()->nb_inputs() );

	// �趨�ն˵�����˿ڸ���
	setOutSlotCount( getProcess()->nb_outputs() );		

	// �����ն�����myRect�Ŀ�Ⱥ͸߶�
	adjustRectByContent();

	// װ���ն˵����������˿�
	setupSlots();

	// ���ô����ڵ�����
	QGraphicsProxyWidget::setGeometry(myRect);
}

void VPTerminal::setupProxyWidget(QWidget* proxyW)
{
	proxyW->setWindowTitle( text() );
	setWidget(proxyW);	
	myRect = proxyW->rect().adjusted(-WindowFrameWidth, -(WindowFrameWidth+WindowHeadWidth), 
				/*VPAbstractNode::SlotSize + */WindowFrameWidth, WindowFrameWidth);	
}

//////////////////////////////////////////////////////////////////////////
// ����Բ����
//////////////////////////////////////////////////////////////////////////
int VPTerminal::roundness(double size) const
{
	return 100 * Diameter / int(size);
}

//////////////////////////////////////////////////////////////////////////
// 11/6/2010 RYF
//		- setupProxyWidget, updateSlots ��myRect rightAdjust����
//////////////////////////////////////////////////////////////////////////
