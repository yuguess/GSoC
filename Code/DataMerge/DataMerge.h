#ifndef DATAMERGE_H
#define DATAMERGE_H

#include "ViewerShell.h"
#include "DataMergeGui.h"

class DataMerge : public QObject, public ViewerShell
{
   Q_OBJECT

public:
   DataMerge();
   ~DataMerge();

   bool execute(PlugInArgList*, PlugInArgList*);
   QWidget* getWidget() const;
   bool serialize(SessionItemSerializer& serializer) const;
   bool deserialize(SessionItemDeserializer& deserializer);

public slots:
   void dialogClosed();

private:
   bool showGui();
   DataMergeGui* mpGui;
};

#endif
