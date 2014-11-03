#ifndef VPABSTRACTNODE_H
#define VPABSTRACTNODE_H

//	8/6/2010 
//	Created by RYF
//	Email : dizuo@126.com
//	QQ:546952720

#include <QList>
#include <QFont>
#include <QMenu>
#include <QVector>
#include <QRectF>
#include <QColor>
#include <QString>

#include <utility>

class VPLink;
class VPConnector;
class AProcess;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ��������������VPTerminal��VPProcessor�зֱ���д
//	scenePosition 
//	getConnector
//	getNodeTips
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class VPAbstractNode
{
public:
	//////////////////////////////////////////////////////////////////////////
	// ���ͺ�ö�ٶ���
	//////////////////////////////////////////////////////////////////////////
	typedef	VPConnector		ConnectorType;
	typedef	ConnectorType*	ConnectorTypePtr;

	enum SlotType
	{
		OUT = 0, 
		IN = 1, 
		NONE = 2
	};

	typedef std::pair<SlotType, int>	SlotInfoType;

	//////////////////////////////////////////////////////////////////////////
	// ���캯������������
	//////////////////////////////////////////////////////////////////////////
	VPAbstractNode();

	VPAbstractNode(QMenu *contextMenu, AProcess* proc,
		unsigned int nbinput, unsigned int nboutput,
		unsigned int id,			//node id
		const QString& name);		//node name

	virtual ~VPAbstractNode();

	//////////////////////////////////////////////////////////////////////////
	// һ�㹦�ܺ���
	//////////////////////////////////////////////////////////////////////////
	
	// text & color
	void	setText(const QString &text);
	const QString& text() const;

	// font
	const QFont&	font() const;
	void	setFont(const QFont &f);

	void setId(int i)	{ myId = i; }
	unsigned int		getId() const { return myId; }

	AProcess* getProcess() const { return myProcess; }
	void setProcess(AProcess* proc) { myProcess = proc; }
	
	ConnectorTypePtr			queryConnector(int idx, SlotType sType);
	void	querySlotsState(bool bBuf[], bool isIn=true);

	//////////////////////////////////////////////////////////////////////////
	// �麯������
	//////////////////////////////////////////////////////////////////////////
	// abstract virtual interfaces
	virtual QPointF			scenePosition() const = 0;

	// virtual functions
	virtual QString				getNodeTips(const QPointF& localPos);
	virtual ConnectorTypePtr	getConnector(const QPointF& localPos);


	//////////////////////////////////////////////////////////////////////////
	// ��̬��������
	//////////////////////////////////////////////////////////////////////////
	static void	setHeadColor(const QColor &color);
	static void	setBodyColor(const QColor &color);
	static void	setInSlotColor(const QColor &color);
	static void	setOutSlotColor(const QColor &color);

protected:
	//////////////////////////////////////////////////////////////////////////
	// ������Ա����
	//////////////////////////////////////////////////////////////////////////
	void	destroy();

	// slot
	void	setupSlots();
	void	setInSlotCount(int iC) { myInCount = iC; }
	void	setOutSlotCount(int oC) { myOutCount = oC; }

	void	renderSlot(QPainter* painter);
	void	renderSlotName(QPainter* painter);

	void	adjustRectByContent();

	QPointF			slotOffset(int idx, SlotType sType);
	SlotInfoType 	querySlot(const QPointF& mouseP);

	//////////////////////////////////////////////////////////////////////////
	// ��Ա���ݶ���
	//////////////////////////////////////////////////////////////////////////

	QMenu*			myContextMenu;

	unsigned int	myId;
	AProcess*		myProcess;

	QString			myText;
	QRectF			myRect;
	QFont			myFont;

	int							myInCount;
	int							myOutCount;
	QVector<ConnectorTypePtr>	inputSlots;
	QVector<ConnectorTypePtr>	outputSlots;

	static QColor	HeadColor;
	static QColor	BodyColor;
	static QColor	InSlotColor;
	static QColor	OutSlotColor;

	static int SlotSize;
};

//////////////////////////////////////////////////////////////////////////
//	8/20/2010 RYF
//	����VPAbstractNode���캯���Ĳ�����AGraphExec*

//	12/1/2010 RYF
//		ȥ��VPAbstractNode�в�����AGraphExec*
//		VPabstractNode���ڹ���AProcess����������
//////////////////////////////////////////////////////////////////////////

#endif