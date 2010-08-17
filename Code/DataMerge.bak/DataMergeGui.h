#ifndef DATAMERGEGUI_H
#define DATAMERGEGUI_H

#include <QtGui/QDialog>
#include <vector>
#include <string>
#include <map>
#include "typesfile.h"

class QListWidget;
class RasterElement;
class DataElement;
class Progress;

class DataMergeGui :  public QDialog
{
	Q_OBJECT
public:
    DataMergeGui(QWidget *parent);
    ~DataMergeGui();
    void addImportList(std::vector<std::string>& vec);
	void setFilenameMap(std::map<std::string, RasterElement*> para);
	void setCubes(std::vector<DataElement*>& para);
	void setProgress(Progress *paraProgress);

public slots:
    void addMergeList();
    void removeMergeList();
	bool mergeData();
	bool selectionChangedSlot();
	bool upButtonSlot();
	bool downButtonSlot();

private:
    QListWidget *mergeList;
    QListWidget *importList;
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