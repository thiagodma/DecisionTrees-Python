/* Eduardo Peixoto (eduardopeixoto@ieee.org)
 * Queen Mary, University of London
 * December, 2011.
 */

/** \file     TTrEngine.h
    \brief    Transcoder engine (header)
*/

#ifndef __TTRENGINE__
#define __TTRENGINE__

//HEVC Classes
#include "TLibCommon/CommonDef.h"
#include "TLibCommon/TComMv.h"
#include "TLibCommon/TComRom.h"
//Transcoder Classes
#include "TTrDefinitions.h"
#include "TTrH264Definitions.h"
#include "TTrConfig.h"
#include "TTrH264MIParser.h"
#include "TTrH264FrameMI.h"
#include "TTrModeDecision.h"
#include "TTrTrainingData.h"
#include "TTrDelphi.h"
//General C++ classes
#include <stdio.h>
#include <vector>

//The Forward declarations that this class needs.
class TComMv;
class TTrConfig;
class TTrH264MIParser;
class TTrH264FrameMI;
class TTrModeDecision;
class TTrDelphi;

// ====================================================================================================================
// Class definition
// ====================================================================================================================

class TTrEngine
{
private:
  //If the object has been initialized
  Bool m_bInitialized;

  //The size of the frame, in pixels.
  UInt m_uiFrameHeight, m_uiFrameWidth;

  //The size of the LCU.
  UInt m_uiLCUSize;

  //The minimum CU Size
  UInt m_uiMinCUSize;

  //The maximum Level
  UInt m_uiMaxLevel;

  //The current frame being encoded.
  UInt m_uiCurrFrame;

  //The number of frames that will be encoded.
  UInt m_uiTotalNumberOfFrames;

  //The config object
  TTrConfig *m_pcConfig;

  //The parser object.
  TTrH264MIParser *m_pcH264MIParser;

  //The H264FrameMI objects.
  TTrH264FrameMI **m_pcH264Frames;

  //If the H264 Frame has been used.
  Bool *m_bH264FrameRead;

  //The MVCandidates
  std::vector<TComMv> m_vcMVCandidates;

  //The current MV Refinement method, for the current block
  MVRefinementMethod m_cCurrentMVRefinementMethod;

  //Variables that control the training.
  UInt m_uiNFramesForTraining;
  UInt m_uiNFramesTrainingCount;
  Bool m_bIsTraining;
  UInt m_uiNumberOfFeatures;

  //The Training Data Object.
  TTrTrainingData *m_pcTrainingData;

  //To measure the time spent training.
  Long m_lTimeStartedTraining;

  //The Delphi transcoder object.
  TTrDelphi *m_pcDelphiTranscoder;

public:
  //Constructor and destructor
  TTrEngine();
  ~TTrEngine();

  //Destroy.
  Void destroy();

  //Initialize the transcoder.
  Bool initializeTranscoder(UInt uiFrameHeight, UInt uiFrameWidth, UInt uiLCUSize, UInt uiMinCUSize, UInt uiTotalNumberOfFrames);

  //Functions to use the transcoder.
  Void updateCurrentFrame(UInt uiCurrFrame);
  Double finishEncodingCurrentFrame(UInt uiCurrFrame, Bool isIntraFrame);

  TTrModeDecision* getTranscoderModeDecision(UInt uiRow, UInt uiCol, UInt uiQP);
  TTrH264FrameMI*  getCurrentH264FrameMI() {return m_pcH264Frames[m_uiCurrFrame]; }

  //Interface functions to the m_pcConfig.
  //TranscodingOption getTranscodingOption()   {return ((m_pcConfig == NULL) ? TR_DISABLED : m_pcConfig->getTranscodingOption()) ; }
  MVGroupingMethod  getMVGroupingMethod() {return ((m_pcConfig == NULL) ? TR_NONE : m_pcConfig->getMVGroupingMethod()) ;}
  MVRefinementMethod getGeneralMVRefinementMethod(UInt uiLevel) {return ((m_pcConfig == NULL) ? TR_IGNORE_H264_MV : m_pcConfig->getGeneralMVRefinementMethod(uiLevel)); }
  TranscodingOption getTranscodingMode() {return ((m_pcConfig == NULL) ? TR_DISABLED : m_pcConfig->getTranscodingOption());}

  Bool isEnabledTranscoder();

  //Interface functions about the training.
  Bool    startTrainingAtFrame(UInt uiFrame);
  Void    startTraining();
  Double  finishTraining();

  Bool    useTrainingStage()             {return m_pcConfig->useTrainingStage();             }
  UInt    getMaxDepthToComputeFeatures() {return m_pcConfig->getMaxDepthToComputeFeatures(); }
  UInt    getNumberOfFeatures()          {return m_uiNumberOfFeatures;                       }
  Bool    isTraining()                   {return m_bIsTraining;                              }

  TR_DelphiMode     getDelphiMode()      {return m_pcConfig->getDelphiMode();                }
  TTrDelphi*        getDelphiPointer()   {return m_pcDelphiTranscoder;                       }

  //Interface functions to get the MVs.
  //This function might be replaced in the future. For now, I will continue using this.
  //TComMv getMVForABlock(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiHeight, UInt uiWidth, Int iRefFrame) {return (((m_pcH264Frames[m_uiCurrFrame] == NULL) || (m_pcConfig->getMVGroupingMethod() == TR_NONE)) ? ::TComMv(0,0) : m_pcH264Frames[m_uiCurrFrame]->getMV(uiRow_4x4, uiCol_4x4, uiHeight, uiWidth, iRefFrame, m_pcConfig->getMVGroupingMethod())); }

  Void computeMVCandidates(UInt uiLevel, UInt uiRow_4x4, UInt uiCol_4x4, UInt uiHeight, UInt uiWidth, Int iRefFrame);
  Void computeMVCandidates(UInt uiLevel, UInt uiRow_4x4, UInt uiCol_4x4, UInt uiHeight, UInt uiWidth, Int iRefFrame, MVGroupingMethod cMethod);
  std::vector<TComMv>& getMVCandidates() {return m_vcMVCandidates; }
  MVRefinementMethod getCurrentMVRefinementMethod() { return m_cCurrentMVRefinementMethod; }

  //Print the candidates.
  Void  printConfig() {if (m_pcConfig != NULL) { m_pcConfig->printTrConfig(); } }

  Bool   hasIntraMBs(UInt uiDepth, UInt uiRow, UInt uiCol);
  Bool   isAllIntraMBs(UInt uiDepth, UInt uiRow, UInt uiCol);
  Bool   computeFeaturesForIntraMBs()    {return m_pcConfig->isComputeVarianceForCUsWithIntraH264MBs() ; }

  //Functions to compute the features.
  Double* computeFeaturesLDF_v1(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double* computeFeaturesLDF_v2(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double* computeFeaturesLDF_v3(UInt uiDepth, UInt uiRow, UInt uiCol, Double *adHEVCCost);
  Double* computeFeaturesLDF_v4(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double* computeFeaturesLDF_v5(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double* computeFeaturesLDF_v6(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double* computeFeaturesPIBIC_v1(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double* computeFeaturesPIBIC_v2(UInt uiDepth, UInt uiRow, UInt uiCol);

  Bool    checkEarlyTermination_LDF_v5(UInt uiDepth,  UInt uiRow, UInt uiCol, UInt uiQP, UInt uiMode, Double dBestCost, TTrModeDecision *modeDecision);

  //Void  computeFeaturesForCU();
  Void writeFeatures(UInt uiDepth, UInt uiNFeatures, Double *dFeatures, CUDecisionModes cHEVC_Class);
  Void writeFeatures(UInt uiDepth, UInt uiNFeatures, Double *dFeatures, CUDecisionModes cHEVC_Class, UInt uiHEVCQP);
  Void writeFeatures(UInt uiDepth, UInt uiNFeatures, Double *dFeatures, CUDecisionModes cHEVC_Class, Double *adHEVCCost, UInt uiHEVCQP);

  Void printH264FrameHeaderToFile();
  Void printH264LCUToFile(UInt uiRow, UInt uiCol);

protected:
  Void setModeDecision                               (UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision *modeDecision);

  Void setModeDecisionDelphi                         (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision *modeDecision, TTrModeDecision *delphiDecision);

  Bool xModeDecision_TrDisabled                      (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_TrH264Reuse                     (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_TrMVSimilarity                  (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_TrMVVariance                    (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_TrMVPhaseVariance               (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_TrNDCT_Coefficients             (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_TrEDCT_Coefficients             (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_TrMVVariance_NDCTCoefficients   (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_Tr_LDF_v1                       (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_TrDepthBased                    (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_Tr_LDF_v2                       (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_Tr_LDF_v4                       (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_Tr_LDF_v5                       (UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_Tr_LDF_v6                       (UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_Tr_PIBIC_v1					 (UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_Tr_PIBIC_v2                     (UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);
  Bool xModeDecision_Tr_DecisionTrees_v1			 (UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);

  //Matlab Default decision trees
  UInt xdecide_depth0_QP22_v1(Double *dFeatures);
  UInt xdecide_depth0_QP27_v1(Double *dFeatures);
  UInt xdecide_depth0_QP32_v1(Double *dFeatures);
  UInt xdecide_depth0_QP37_v1(Double *dFeatures);
  UInt xdecide_depth1_QP22_v1(Double *dFeatures);
  UInt xdecide_depth1_QP27_v1(Double *dFeatures);
  UInt xdecide_depth1_QP32_v1(Double *dFeatures);
  UInt xdecide_depth1_QP37_v1(Double *dFeatures);

  // Matlab Cost Based decision trees
  UInt xdecide_depth0_QP22_v2(Double *dFeatures);
  UInt xdecide_depth1_QP22_v2(Double *dFeatures);
  UInt xdecide_depth0_QP27_v2(Double *dFeatures);
  UInt xdecide_depth1_QP27_v2(Double *dFeatures);
  UInt xdecide_depth0_QP32_v2(Double *dFeatures);
  UInt xdecide_depth1_QP32_v2(Double *dFeatures);
  UInt xdecide_depth0_QP37_v2(Double *dFeatures);
  UInt xdecide_depth1_QP37_v2(Double *dFeatures);

  UInt xdecide_depth0_QP23_v3(Double *dFeatures);
  UInt xdecide_depth0_QP24_v3(Double *dFeatures);
  UInt xdecide_depth0_QP25_v3(Double *dFeatures);
  UInt xdecide_depth0_QP28_v3(Double *dFeatures);
  UInt xdecide_depth0_QP29_v3(Double *dFeatures);
  UInt xdecide_depth0_QP30_v3(Double *dFeatures);
  UInt xdecide_depth0_QP33_v3(Double *dFeatures);
  UInt xdecide_depth0_QP34_v3(Double *dFeatures);
  UInt xdecide_depth0_QP35_v3(Double *dFeatures);
  UInt xdecide_depth0_QP38_v3(Double *dFeatures);
  UInt xdecide_depth0_QP39_v3(Double *dFeatures);
  UInt xdecide_depth0_QP40_v3(Double *dFeatures);
  UInt xdecide_depth1_QP23_v3(Double *dFeatures);
  UInt xdecide_depth1_QP24_v3(Double *dFeatures);
  UInt xdecide_depth1_QP25_v3(Double *dFeatures);
  UInt xdecide_depth1_QP28_v3(Double *dFeatures);
  UInt xdecide_depth1_QP29_v3(Double *dFeatures);
  UInt xdecide_depth1_QP30_v3(Double *dFeatures);
  UInt xdecide_depth1_QP33_v3(Double *dFeatures);
  UInt xdecide_depth1_QP34_v3(Double *dFeatures);
  UInt xdecide_depth1_QP35_v3(Double *dFeatures);
  UInt xdecide_depth1_QP38_v3(Double *dFeatures);
  UInt xdecide_depth1_QP39_v3(Double *dFeatures);
  UInt xdecide_depth1_QP40_v3(Double *dFeatures);
  /*
  // Python Default decision trees
  UInt xdecide_depth0_QP22_v3(Double *dFeatures);
  UInt xdecide_depth1_QP22_v3(Double *dFeatures);
  UInt xdecide_depth0_QP27_v3(Double *dFeatures);
  UInt xdecide_depth1_QP27_v3(Double *dFeatures);
  UInt xdecide_depth0_QP32_v3(Double *dFeatures);
  UInt xdecide_depth1_QP32_v3(Double *dFeatures);
  UInt xdecide_depth0_QP37_v3(Double *dFeatures);
  UInt xdecide_depth1_QP37_v3(Double *dFeatures);

  /*
  // Python Cost Based decision trees
  UInt xdecide_depth0_QP22_v4(Double *dFeatures);
  UInt xdecide_depth1_QP22_v4(Double *dFeatures);
  UInt xdecide_depth0_QP27_v4(Double *dFeatures);
  UInt xdecide_depth1_QP27_v4(Double *dFeatures);
  UInt xdecide_depth0_QP32_v4(Double *dFeatures);
  UInt xdecide_depth1_QP32_v4(Double *dFeatures);
  UInt xdecide_depth0_QP37_v4(Double *dFeatures);
  UInt xdecide_depth1_QP37_v4(Double *dFeatures);
  */



  Bool xModeDecision_FromH264MB                      (UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision);

  Double xComputeDotProduct(Double *dFeatures, const Double *dWeights, UInt uiNFeatures);


  //Functions to compute the models depending on the transcoding option
  Void xBuildModelMVVarianceDistance();
  Void xBuildModelMVPhaseVariance();
  Void xBuildModelNDCTCoefficients();
  Void xBuildModelEDCTCoefficients();

  Void xBuildModelLDF_v1();
  Void xBuildModelLDF_v3();
  Void xBuildModelLDF_v4();
  Void xBuildModelLDF_v5();
  Void xBuildModelLDF_v6();




  //Filipe - acrescentei essa
  Void xBuildModelPIBIC_v1();




  Double* xGetFeatureToNonSplitClass_LDF_v1(UInt uiDepth);

  //TODO: All these functions should be protected!
  Bool    xComputeFeature(H264Features cFeature) {return m_pcConfig->computeFeature(cFeature); }
  Double  xComputeFeatureNumberOfH264Partitions(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double* xComputeFeatureH264MBDistribution(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double  xComputeFeatureMVVarianceDistance(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double  xComputeFeatureMVVariance_Y(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double  xComputeFeatureMVVariance_X(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double  xComputeFeatureMVPhaseVariance(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double  xComputeFeatureMVPhaseTrigonometricVariance(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double  xComputeFeatureNDCTCoefficients(UInt uiDepth, UInt uiRow, UInt uiCol);
  Double  xComputeFeatureEnergyDCTCoefficients(UInt uiDepth, UInt uiRow, UInt uiCol);

  Void   xComputeFeatureMVVarianceForARegion(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiNRows_4x4, UInt uiNCols_4x4, Double *dRegionVarianceY, Double *dRegionVarianceX, Double *dRegionVarianceDistance);
  Void   xComputeFeatureMVPhaseVarianceForARegion(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiNRows_4x4, UInt uiNCols_4x4, Double *dRegionPhaseVariance);
  Void   xComputeFeatureNDCTCoefficientsForARegion(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiNRows_4x4, UInt uiNCols_4x4, Double *dRegionNDCTCoefficients);
  Void   xComputeFeatureEnergyDCTCoefficientsForARegion(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiNRows_4x4, UInt uiNCols_4x4, Double *dRegionEDCTCoefficients);

  Void   xComputeFeatureMBDistribution4Classes(UInt uiDepth, UInt uiRow, UInt uiCol, Double *pdDistributionSKIP, Double *pdDistribution16, Double *pdDistribution8, Double *pdDistributionIntra);

  UInt   xComputeNumberOfFeatures();
}; // END CLASS DEFINITION TTrEngine

#endif // __TTRENGINE__
