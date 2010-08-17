#ifndef DATAMERGE_H
#define DATAMERGE_H

#include "ViewerShell.h"
#include "DataMergeGui.h"

class Progress;

class DataMerge : public QObject, public ViewerShell
{
   Q_OBJECT

public:
   DataMerge();
   ~DataMerge();

   bool getInputSpecification(PlugInArgList*& pArgList);
   bool getOutputSpecification(PlugInArgList*& pArgList);
   bool execute(PlugInArgList*, PlugInArgList*);
   QWidget* getWidget() const;
   bool serialize(SessionItemSerializer& serializer) const;
   bool deserialize(SessionItemDeserializer& deserializer);

public slots:
   void dialogClosed();

private:
   bool showGui();
   DataMergeGui* mpGui;
   Progress *pProgress;
};

#endif
