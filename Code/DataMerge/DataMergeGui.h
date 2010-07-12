#ifndef DATAMERGEGUI_H
#define DATAMERGEGUI_H

#include <QtGui/QtGui>

#include "PlotWidget.h"
#include "DockWindow.h"
#include "Layer.h"

class QComboBox;
class QDoubleSpinBox;
class QLabel;

class DataMergeGui :  public QDialog
{
	Q_OBJECT

public:
	DataMergeGui( QWidget* pParent = 0, const char* pName = 0, bool modal = FALSE );
	~DataMergeGui();

	public slots:
		void applyScale();
		void generateNewView();

private:
	QPushButton* mpCancelButton;
	QPushButton* mpApplyButton;
	QPushButton* mpGenerateViewButton;
	QComboBox* mpCubeListCombo;
	QDoubleSpinBox* mpXScaleFactor;
	QDoubleSpinBox* mpYScaleFactor;
	QLabel* mpCubeListComboLabel;
	QLabel* mpXScaleFactorLabel;
	QLabel* mpYScaleFactorLabel;

	void init();
	std::vector<std::string> mCubeNames;
	Layer* mpScaledLayer;
	bool mbScalingApplied;
};

#endif