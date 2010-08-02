#ifndef DATAMERGEGUI_H
#define DATAMERGEGUI_H

#include <QtGui/QDialog>
#include <vector>
#include <string>
#include <map>

class QListWidget;
class RasterElement;
class DataElement;

class DataMergeGui :  public QDialog
{
	Q_OBJECT
public:
    DataMergeGui(QWidget *parent);
    ~DataMergeGui();
    void addImportList(std::vector<std::string>& vec);
	void setFilenameMap(std::map<std::string, RasterElement*> para);
	void setCubes(std::vector<DataElement*>& para);

public slots:
    void addMergeList();
    void removeMergeList();
	bool mergeData();

private:
    QListWidget *mergeList;
    QListWidget *importList;
	std::vector<RasterElement *> mergeElementList;
	std::map<std::string, RasterElement*> filenameMap;
	std::vector<DataElement*> cubes;
	QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *mergeButton;
};

#endif