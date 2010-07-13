#ifndef ANOMALY_DETECTION_H
#define ANOMALY_DETECTION_H

#include "ExecutableShell.h"

class AnomalyDetection : public ExecutableShell
{
public:
   AnomalyDetection();
   virtual ~AnomalyDetection();

   virtual bool getInputSpecification(PlugInArgList*& pInArgList);
   virtual bool getOutputSpecification(PlugInArgList*& pOutArgList);
   virtual bool execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList);
};

#endif