#ifndef DATAMERGEGUI_H
#define DATAMERGEGUI_H

#include <QtGui/QDialog>
#include <vector>
#include <string>
#include <map>
#include "ImportElement.h"
#include "typesfile.h"

class QListWidget;
class RasterElement;
class DataElement;
class Progress;
class QTreeWidget;
class QTreeWidgetItem;

class DataMergeGui :  public QDialog
{
	Q_OBJECT
public:
    DataMergeGui(QWidget *parent);
    ~DataMergeGui();
    //void addImportList(std::vector<std::string>& vec);
	void addImportList(std::vector<ImportElement>& vec);
	void setFilenameMap(std::map<std::string, RasterElement*> para);
	void setCubes(std::vector<DataElement*>& para);
	void setProgress(Progress *paraProgress);
	void updateCheckState(QTreeWidgetItem *parent, Qt::CheckState state);
	RasterElement* extractRasterElement(QString str);
	int extractMergeBand(QString str);


public slots:
    void addMergeList();
    void removeMergeList();
	bool mergeData();
	bool selectionChangedSlot();
	bool upButtonSlot();
	bool downButtonSlot();
	void treeItemSlot(QTreeWidgetItem *item, int column);
	
private:
    QListWidget *mergeList;
	QTreeWidget *importTree;
//    QListWidget *importList;
	std::vector<RasterElement *> mergeElementList;
	std::map<std::string, RasterElement*> filenameMap;
	std::vector<DataElement*> cubes;
	QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *mergeButton;
	QPushButton *upButton;
	QPushButton *downButton;
	Progress *pProgress;
	EncodingType firstType;
	int firstRowCount;
	int firstColCount;
	int flag;
};

#endif