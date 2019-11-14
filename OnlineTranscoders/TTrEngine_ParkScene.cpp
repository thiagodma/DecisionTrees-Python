/* Eduardo Peixoto (eduardopeixoto@ieee.org)
 * Queen Mary, University of London
 * December, 2011.
 */

/** \file     TTrEngine.cpp
    \brief    Transcoder engine
*/

//Transcoder Classes
#include "TTrEngine.h"
//General C++ classes
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

//A simple max function
#define sMAX(X,Y) ((X) > (Y) ? (X) : (Y))

// ====================================================================================================================
// Constructor / destructor / create / destroy
// ====================================================================================================================
/**
 */
TTrEngine::TTrEngine():
m_bInitialized(false),
m_uiFrameHeight(0),
m_uiFrameWidth(0),
m_uiLCUSize(0),
m_uiMinCUSize(0),
m_uiMaxLevel(0),
m_uiCurrFrame(0),
m_uiTotalNumberOfFrames(0),
m_pcConfig(NULL),
m_pcH264MIParser(NULL),
m_pcH264Frames(NULL),
m_bH264FrameRead(NULL),
m_cCurrentMVRefinementMethod(TR_IGNORE_H264_MV),
m_pcTrainingData(NULL),
m_uiNFramesForTraining(0),
m_uiNFramesTrainingCount(0),
m_bIsTraining(false),
m_lTimeStartedTraining(0),
m_pcDelphiTranscoder(NULL)
{
  //Create an empty object.
}

/**
 */
TTrEngine::~TTrEngine()
{
  if (m_bInitialized) destroy();
}

/**
 */
Void TTrEngine::destroy()
{
  UInt k;

  //Destroy the H.264 frames.
  if (m_pcH264Frames != NULL)
  {
    for (k = 0; k < m_uiTotalNumberOfFrames; k++)
    {
      //Delete the other H264Frames that have not been deleted yet.
      if (m_pcH264Frames[k] != NULL)
	  {
#ifdef TR_DEBUG_MEMORY_ALLOCATION
	    printf("Deleting frame %d .\n",k);
#endif
	    delete m_pcH264Frames[k];
	    m_pcH264Frames[k] = NULL;
  	  }
    }
    delete [] m_pcH264Frames;
    m_pcH264Frames = NULL;
  }

  free(m_bH264FrameRead);
  m_bH264FrameRead = NULL;

  //Delete the parser.
  if (m_pcH264MIParser != NULL)
  {
    delete m_pcH264MIParser;
    m_pcH264MIParser = NULL;
  }

  //Delete the training data.
  if (m_pcTrainingData != NULL)
  {
    delete m_pcTrainingData;
	m_pcTrainingData = NULL;
  }

  //Delete the Delphi Transcoder.
  if (m_pcDelphiTranscoder != NULL)
  {
	delete m_pcDelphiTranscoder;
	m_pcDelphiTranscoder = NULL;
  }

  //Delete the config.
  if (m_pcConfig != NULL)
  {
    delete m_pcConfig;
    m_pcConfig = NULL;
  }

  //Erases the variables.
  m_uiFrameHeight          = 0;
  m_uiFrameWidth           = 0;
  m_uiLCUSize              = 0;
  m_uiMinCUSize            = 0;
  m_uiMaxLevel             = 0;
  m_uiCurrFrame            = 0;
  m_uiTotalNumberOfFrames  = 0;
  m_uiNFramesForTraining   = 0;
  m_uiNFramesTrainingCount = 0;
  m_bIsTraining            = false;
  m_lTimeStartedTraining   = 0;

  m_bInitialized  = false;
}

// ====================================================================================================================
// Public member functions
// ====================================================================================================================
/**
 */
Bool TTrEngine::initializeTranscoder(UInt uiFrameHeight, UInt uiFrameWidth, UInt uiLCUSize, UInt uiMinCUSize, UInt uiTotalNumberOfFrames)
{
  //First, create the Config file.
  m_pcConfig = new TTrConfig();
  Bool bTRSettingsOK = m_pcConfig->parseTranscoderSettings();

  if (bTRSettingsOK == false)
  {
	m_pcConfig->printTrConfig();
    printf("\nHEVC Transcoder ERROR: parsing the config file aborted.\n");
    delete m_pcConfig;
	m_pcConfig = NULL;
	return false;
  }

  //Since there is a chance the Delphi Transcoder will break down the execution, I will try it first.
  if (m_pcConfig->getDelphiMode() != TR_DELPHI_OFF)
  {
	Bool bIsDelphiOK;

	m_pcDelphiTranscoder = new TTrDelphi(m_pcConfig->getDelphiFilename());

	if (m_pcConfig->getDelphiMode() == TR_DELPHI_READ)
	{
	  bIsDelphiOK = m_pcDelphiTranscoder->startReading();
	} else if (m_pcConfig->getDelphiMode() == TR_DELPHI_WRITE)
	{
	  bIsDelphiOK = m_pcDelphiTranscoder->startWriting();
	}

	if (!bIsDelphiOK)
	{
	  delete m_pcConfig;
	  m_pcConfig = NULL;
	  delete m_pcDelphiTranscoder;
	  m_pcDelphiTranscoder = NULL;
	  return false;
	}
  }

  //Copy the parameters.
  m_uiFrameHeight         = uiFrameHeight;
  m_uiFrameWidth          = uiFrameWidth;
  m_uiLCUSize             = uiLCUSize;
  m_uiMinCUSize           = uiMinCUSize;
  m_uiCurrFrame           = 0;
  m_uiTotalNumberOfFrames = uiTotalNumberOfFrames;

  //Compute the maximum level.
  m_uiMaxLevel = 0;
  UInt uiSize = m_uiLCUSize;
  while (uiSize != m_uiMinCUSize) {uiSize = uiSize >> 1; m_uiMaxLevel++; }

  //Create the parser.
  if (m_pcConfig->getTranscodingOption() != TR_DISABLED)
  {
    m_pcH264MIParser = new TTrH264MIParser(m_uiFrameHeight,m_uiFrameWidth,m_pcConfig->getH264MIFilename());

	m_pcH264MIParser->setLCUSize(m_uiLCUSize);

	//Sets the parser.
    m_pcH264MIParser->setUseMVSimilarity(m_pcConfig->isUseMVSimilarity(),m_pcConfig->isMVSimScaleMVs());
    m_pcH264MIParser->setMVSimThreshold(m_pcConfig->getMVSimThresholdVector());
	m_pcH264MIParser->setUseMVVariance(m_pcConfig->computeMVVarianceInfo(),m_pcConfig->isMVVarScaleMVs(), m_pcConfig->isComputeVarianceForCUsWithIntraH264MBs());
	m_pcH264MIParser->setUseTransformInfo(m_pcConfig->computeTransformInfo());
	m_pcH264MIParser->setUseFrameMBInfo(m_pcConfig->isUseFrameMBInfo());

	//Allocate the main array for H.264 MI Frames.
	m_pcH264Frames = new TTrH264FrameMI*[m_uiTotalNumberOfFrames];
	//Sets ALL arrays to NULL!
	for (UInt k = 0; k < m_uiTotalNumberOfFrames; k++)
	{
	  m_pcH264Frames[k] = NULL;
	}

	//Allocate and sets the array.
	m_bH264FrameRead = (Bool*) malloc(m_uiTotalNumberOfFrames * sizeof(Bool));
	memset(m_bH264FrameRead,false,m_uiTotalNumberOfFrames);
  }

  //Reserve memory for the MV Candidates.
  m_vcMVCandidates.reserve((m_uiLCUSize * m_uiLCUSize) >> 4);

  //Initialize the class for Decision Trees v1
  if (m_pcConfig->getTranscodingOption() == TR_DECISION_TREES_v1)
  {
	  //TODO
	  //THIAGO - Aqui é onde a gente deve criar/inicializar a classe referente a Decision Trees.
	  m_bIsTraining = false;

	  //Compute and sets the number of features.
	  UInt uiNFeatures = xComputeNumberOfFeatures();
	  m_uiNumberOfFeatures = uiNFeatures;
  }

  //Initialize the class for Decision Trees v2
  if (m_pcConfig->getTranscodingOption() == TR_DECISION_TREES_v2)
  {
	  //TODO
	  //THIAGO - Aqui é onde a gente deve criar/inicializar a classe referente a Decision Trees.
	  m_bIsTraining = false;

	  //Compute and sets the number of features.
	  UInt uiNFeatures = xComputeNumberOfFeatures();
	  m_uiNumberOfFeatures = uiNFeatures;
  }

  //Initialize the class for Decision Trees v3
  if (m_pcConfig->getTranscodingOption() == TR_DECISION_TREES_v3)
  {
	  //TODO
	  //THIAGO - Aqui é onde a gente deve criar/inicializar a classe referente a Decision Trees.
	  m_bIsTraining = false;

	  //Compute and sets the number of features.
	  UInt uiNFeatures = xComputeNumberOfFeatures();
	  m_uiNumberOfFeatures = uiNFeatures;
  }

  //Create the training data (if needed).
  if (m_pcConfig->useTrainingStage())
  {
    m_uiNFramesForTraining   = m_pcConfig->getNFramesUsedForTraining();
    m_uiNFramesTrainingCount = 0;
    m_bIsTraining            = false;
    m_lTimeStartedTraining   = 0;

    m_pcTrainingData = new TTrTrainingData();

	//Calculates the number of features needed.
	UInt uiNFeatures = xComputeNumberOfFeatures();
	m_uiNumberOfFeatures = uiNFeatures;

	m_pcTrainingData->create(m_uiNFramesForTraining, m_uiLCUSize,m_uiFrameHeight, m_uiFrameWidth,uiNFeatures, m_pcConfig->getMaxDepthToComputeFeatures());

	if (m_pcConfig->getTranscodingOption() == TR_LDF_v1)
	{
	  m_pcTrainingData->initialize_LDF_v1();
	}

	if (m_pcConfig->getTranscodingOption() == TR_LDF_v4)
	{
	  m_pcTrainingData->initialize_LDF_v4(m_pcConfig->computeFeature(TR_H264FEATURE_ONE), m_pcConfig->computeFeature(TR_H264FEATURE_MEAN), m_pcConfig->useFeatureStandardization());
	}

	if (m_pcConfig->getTranscodingOption() == TR_LDF_v5)
	{
	  m_pcTrainingData->initialize_LDF_v5(m_pcConfig->computeFeature(TR_H264FEATURE_ONE), m_pcConfig->computeFeature(TR_H264FEATURE_MEAN), m_pcConfig->useFeatureStandardization());
	}

	if (m_pcConfig->getTranscodingOption() == TR_LDF_v6)
	{
		m_pcTrainingData->initialize_LDF_v6(m_pcConfig->computeFeature(TR_H264FEATURE_ONE), m_pcConfig->computeFeature(TR_H264FEATURE_MEAN), m_pcConfig->useFeatureStandardization(), m_pcConfig->getTranscodingOption());
	}

	if (m_pcConfig->getTranscodingOption() == TR_PIBIC_v1)
	{
		//Pibic v1 uses the same initializer as v6.
		m_pcTrainingData->initialize_LDF_v6(m_pcConfig->computeFeature(TR_H264FEATURE_ONE), m_pcConfig->computeFeature(TR_H264FEATURE_MEAN), m_pcConfig->useFeatureStandardization(), m_pcConfig->getTranscodingOption());
	}

	if (m_pcConfig->getTranscodingOption() == TR_PIBIC_v2)
	{
		//Pibic v2 uses the same initializer as v6.
		m_pcTrainingData->initialize_LDF_v6(m_pcConfig->computeFeature(TR_H264FEATURE_ONE), m_pcConfig->computeFeature(TR_H264FEATURE_MEAN), m_pcConfig->useFeatureStandardization(), m_pcConfig->getTranscodingOption());
	}

	if (m_pcConfig->getTranscodingOption() == TR_DECISION_TREES_v1)
	{
		//TODO
		//THIAGO - Aqui é onde a gente deve inicializar a Decision Trees online.
	}

	if (m_pcConfig->getTranscodingOption() == TR_DECISION_TREES_v2)
	{
		//TODO
		//THIAGO - Aqui é onde a gente deve inicializar a Decision Trees online.
	}

	if (m_pcConfig->getTranscodingOption() == TR_DECISION_TREES_v3)
	{
		//TODO
		//THIAGO - Aqui é onde a gente deve inicializar a Decision Trees online.
	}

  }

  m_bInitialized  = true;

  return true;
}

/**
 This function is called BEFORE the frame is encoded.
 */
Void TTrEngine::updateCurrentFrame(UInt uiCurrFrame)
{
  //Updates the current frame.
  m_uiCurrFrame = uiCurrFrame;

  //A pointer to a frame.
  TTrH264FrameMI *pcH264Frame;
  UInt uiRealFrameNumber;
  UInt uiFrame;

#ifdef TR_DEBUG_MEMORY_ALLOCATION
  printf("------------------------------------\n");
  printf("Transcoder Current frame: %d .\n",uiCurrFrame);
#endif

  //If it needs to read frames.
  if (m_pcConfig->getTranscodingOption() != TR_DISABLED)
  {
    //Checks if the current frame has been read.
    Bool bCurrFrameRead = (m_pcH264Frames[uiCurrFrame] != NULL);

	//Gets the next frame the parser will read.
	UInt uiNextFrameToBeRead = m_pcH264MIParser->getCurrentFrameNumber();

	//The last frame to be read at this moment.
	UInt uiLastFrameToBeRead = (((m_uiCurrFrame + 10) < m_uiTotalNumberOfFrames) ? (m_uiCurrFrame + 10) : m_uiTotalNumberOfFrames);

	//Reads the next 10 frames (if they have not been read).
	//Note that I am sure that the uiNextFrameToBeRead has NOT been read because of the parser.
	for (uiFrame = uiNextFrameToBeRead; uiFrame < uiLastFrameToBeRead; uiFrame++)
	{
#ifdef TR_DEBUG_MEMORY_ALLOCATION
	  printf("Read frame %d .\n",uiFrame);
#endif

	  //Effectively reads the frame.
      pcH264Frame = m_pcH264MIParser->readOneFrame();

	  //Checks the frame number.
	  uiRealFrameNumber = pcH264Frame->getFrameNumber();

	  //Checks if the frame read is the correct frame.
	  if (uiFrame != uiRealFrameNumber)
	  {
	    printf("BUG! The frame read is different than expected!.\n");
	  }

	  //Keeps this pointer in my array.
	  m_pcH264Frames[uiFrame] = pcH264Frame;
	}

	//Checks if I can delete some frames.
	Int iLastFrameUsed = -1; //This is the last frame that has been used contiguously!
	while (iLastFrameUsed < (Int) (m_uiTotalNumberOfFrames-1))
	{
	  if (m_bH264FrameRead[iLastFrameUsed + 1])
	  {
		iLastFrameUsed++;
	  } else
	  {
	    break;
	  }
	}

	if (iLastFrameUsed >= 0) //which means that some frames have been used...
	{
	  //The last frame to delete is the minimum between the iLastFrameUsed and (m_uiCurrFrame - 10)
	  Int iFrame = (((((Int)m_uiCurrFrame) - 10) > 0) ? (((Int)m_uiCurrFrame) - 10) : 0);
	  UInt uiLastFrameToDelete = ((iLastFrameUsed < iFrame ) ? iLastFrameUsed : iFrame);

	  //Delete all frames from 0 to that.
	  UInt i;
	  for (i = 0; i < uiLastFrameToDelete; i++)
	  {
	    if (m_pcH264Frames[i] != NULL)
		{
#ifdef TR_DEBUG_MEMORY_ALLOCATION
		  printf("Deleting frame %d .\n",i);
#endif
		  delete m_pcH264Frames[i];
		  m_pcH264Frames[i] = NULL;
		}
	  }
	}

	//Writes that this frame has been used.
	m_bH264FrameRead[m_uiCurrFrame] = true;
  }

#ifdef TR_DEBUG_MEMORY_ALLOCATION
  printf("------------------------------------\n");
#endif
}

/**
 This function is called AFTER the frame is encoded.
 */
Double TTrEngine::finishEncodingCurrentFrame(UInt uiCurrFrame, Bool isIntraFrame)
{
  Double dResult = 0.0;

  if (isTraining() && !isIntraFrame)
  {
	m_uiNFramesTrainingCount++;
	if (m_uiNFramesTrainingCount == m_uiNFramesForTraining)
	{
#ifdef TR_DEBUG_TRANSCODER_TRAINING
      printf("------------------------------------\n");
  	  Long lBefore = clock();
#endif

	  dResult = finishTraining();

	  //m_pcConfig->printTrConfig();

#ifdef TR_DEBUG_TRANSCODER_TRAINING
	  Double dTimeElapsed = (clock() - lBefore) / CLOCKS_PER_SEC;
	  printf("Time elapsed computing the model: %6.6f seconds.\n",dTimeElapsed);
	  printf("------------------------------------\n");
#endif
	}
  }
  return dResult;
}

/**
 */
Bool TTrEngine::startTrainingAtFrame(UInt uiFrame)
{
  //If the training stage is used, then I have to check if this frame is in my list.
  if (m_pcConfig->useTrainingStage())
  {
	std::vector<UInt> vuiFrames = m_pcConfig->getFramesToStartTraining();

    for (UInt i = 0; i < vuiFrames.size(); i++)
    {
	  if (vuiFrames[i] == uiFrame) return true;
    }
  }

  return false;
}

/**
 */
Void TTrEngine::startTraining()
{
  //Resets the training variables.
  m_uiNFramesTrainingCount = 0;
  m_bIsTraining            = true;

  //Resets the TrainingData Object.
  m_pcTrainingData->resetTrainingData();

  //Computes the time.
  m_lTimeStartedTraining   = clock();

#ifdef TR_DEBUG_TRANSCODER_TRAINING
  printf("Started training: frame number: %d .\n",m_uiCurrFrame);
#endif
}

/**
 */
Double TTrEngine::finishTraining()
{
  //Stops the training
  m_bIsTraining = false;

  switch (m_pcConfig->getTranscodingOption())
  {
	case TR_MVVARIANCE:
	  xBuildModelMVVarianceDistance();
	  break;

    case TR_MVPHASEVARIANCE:
	  xBuildModelMVPhaseVariance();
      break;

    case TR_NDCT_COEFFICIENTS:
	  xBuildModelNDCTCoefficients();
	  break;

	case TR_EDCT_COEFFICIENTS:
	  xBuildModelEDCTCoefficients();
	  break;

    case TR_LDF_v1:
	  xBuildModelLDF_v1();
	  break;

	case TR_LDF_v2:
	  //I am not buiding the model yet.
	  printf("The LDF_v2 is not ready to build a model! .\n");
	  //xBuildModelLDF_v1();
	  break;

	case TR_LDF_v3:
	  xBuildModelLDF_v3();
	  break;

	case TR_LDF_v4:
	  xBuildModelLDF_v4();
	  break;

	case TR_LDF_v5:
	  xBuildModelLDF_v5();
	  break;

	case TR_LDF_v6:
	  xBuildModelLDF_v6();
	  break;

	case TR_PIBIC_v1:
	  //Pibic v1 uses the same function as LDF_v6.
	  xBuildModelLDF_v6();
	  break;

	case TR_PIBIC_v2:
	  //PIBIC v1 and v2 should get its own BuildModel functions!
	  xBuildModelLDF_v6();
	  break;

	case TR_DECISION_TREES_v1:
	  //TODO
	  //THIAGO: Aqui é onde você constrói o modelo do Decision Trees v1.
      break;

	case TR_DECISION_TREES_v2:
		//TODO
		//THIAGO: Aqui é onde você constrói o modelo do Decision Trees v2.
		break;

	case TR_DECISION_TREES_v3:
		//TODO
		//THIAGO: Aqui é onde você constrói o modelo do Decision Trees v3.
		break;
  }

#ifdef TR_WRITE_TRAINING_DATA_TO_FILE
  //For debugging, write all features to a file.
  m_pcTrainingData->writeFeaturesToFile();
#endif

  //m_pcConfig->printTrConfig();

  //Computes the time.
  Double dTimeElapsedTraining;
  dTimeElapsedTraining = (double) (clock() - m_lTimeStartedTraining) / CLOCKS_PER_SEC;

#ifdef TR_DEBUG_TRANSCODER_TRAINING
  printf("Finished training: frame number: %d .\n",m_uiCurrFrame);
  printf("Time spent in this training    : %12.3f sec.\n", dTimeElapsedTraining);
#endif

  return dTimeElapsedTraining;
}

/**
 * Computes the features for LDF v1.
 */
Double* TTrEngine::computeFeaturesLDF_v1(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Bool bDebug = false;

  //These are the entities that I want to compute:
  Double *adFeatures;
  UInt uiTotalFeatures;
  UInt uiNFeatures = 0;

  //Allocates the feature array.
  uiTotalFeatures = getNumberOfFeatures();
  adFeatures = (Double*) malloc(uiTotalFeatures * sizeof(Double));

  //The level of the 4x4 block.
  UInt uiLevel_4x4 = getCurrentH264FrameMI()->getTransformInfo()->getLevel_4x4();
  //Get the coordinates of the CU (in 4x4 blocks)
  UInt uiRow_4x4  = uiRow << (uiLevel_4x4 - uiDepth);
  UInt uiCol_4x4  = uiCol << (uiLevel_4x4 - uiDepth);
  UInt uiSize_4x4 =     1 << (uiLevel_4x4 - uiDepth);

  if (bDebug) printf("-------------  ONE  -------------\n");
  //---------------------------------------
  //One
  if (xComputeFeature(TR_H264FEATURE_ONE))
  {
	adFeatures[uiNFeatures] = 1;
	if (bDebug) printf("One: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  if (bDebug) printf("------------- 2Nx2N -------------\n");
  //---------------------------------------
  //Number of H264 Partitions 2Nx2N.
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_PARTITIONS_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureNumberOfH264Partitions(uiDepth, uiRow, uiCol);
	if (bDebug) printf("Number of Partitions 2Nx2N: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
  {
    adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance X 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureMVVariance_X(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance X 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Y 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureMVVariance_Y(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance Y 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //MV Phase Variance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Phase Variance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //Number of DCT Coefficients 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth, uiRow, uiCol);
	if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //Energy of DCT Coefficients 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureEnergyDCTCoefficients(uiDepth, uiRow, uiCol);
	if (bDebug) printf("Energy of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  if (bDebug) printf("-------------  MEAN -------------\n");
  //---------------------------------------
  //Mean
  if (xComputeFeature(TR_H264FEATURE_MEAN))
  {
	Double dSum = 0.0;
	for (UInt ui_k = (xComputeFeature(TR_H264FEATURE_ONE) ? 1 : 0) ; ui_k < uiNFeatures; ui_k++)
	{
	  dSum += adFeatures[ui_k];
	}
	adFeatures[uiNFeatures] = dSum / (Double) ( (xComputeFeature(TR_H264FEATURE_ONE) ? (uiNFeatures - 1) : uiNFeatures) );
	if (bDebug) printf("Mean: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }
  return adFeatures;
}

/**
 * Computes the features for LDF v2.
 */
Double* TTrEngine::computeFeaturesLDF_v2(UInt uiDepth, UInt uiRow, UInt uiCol)
{

  //These are the entities that I want to compute:
  /*
  Double *adFeatures;
  UInt uiTotalFeatures;
  UInt uiNFeatures = 0;

  //Allocates the feature array.
  uiTotalFeatures = pcTranscoderEngine->getNumberOfFeatures();
  adFeatures = (Double*) malloc(uiTotalFeatures * sizeof(Double));

  //The level of the 4x4 block.
  UInt uiLevel_4x4 = pcTranscoderEngine->getCurrentH264FrameMI()->getTransformInfo()->getLevel_4x4();
  //Get the coordinates of the CU (in 4x4 blocks)
  UInt uiRow_4x4  = uiRow << (uiLevel_4x4 - uiDepth);
  UInt uiCol_4x4  = uiCol << (uiLevel_4x4 - uiDepth);
  UInt uiSize_4x4 =     1 << (uiLevel_4x4 - uiDepth);

  UInt j0,j1,j2,j3,i0,i1,i2,i3,jSize,iSize,jSize0,jSize1,iSize0,iSize1;


  if (bDebug) printf("------------- 2Nx2N -------------\n");
  //---------------------------------------
  //Number of H264 Partitions 2Nx2N.
  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_PARTITIONS_2Nx2N))
  {
	adFeatures[uiNFeatures] = pcTranscoderEngine->xComputeFeatureNumberOfH264Partitions(uiDepth, uiRow, uiCol);
	if (bDebug) printf("Number of Partitions 2Nx2N: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance 2Nx2N
  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
  {
    adFeatures[uiNFeatures] = pcTranscoderEngine->xComputeFeatureMVVarianceDistance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance X 2Nx2N
  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2Nx2N))
  {
	adFeatures[uiNFeatures] = pcTranscoderEngine->xComputeFeatureMVVariance_X(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance X 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Y 2Nx2N
  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2Nx2N))
  {
	adFeatures[uiNFeatures] = pcTranscoderEngine->xComputeFeatureMVVariance_Y(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance Y 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

    //---------------------------------------
    //MV Phase Variance 2Nx2N
    if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2Nx2N))
    {
	  adFeatures[uiNFeatures] = pcTranscoderEngine->xComputeFeatureMVPhaseVariance(uiDepth, uiRow, uiCol);
	  if (bDebug) printf("MV Phase Variance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
      uiNFeatures++;
    }

    //---------------------------------------
    //Number of DCT Coefficients 2Nx2N
    if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
    {
	  adFeatures[uiNFeatures] = pcTranscoderEngine->xComputeFeatureNDCTCoefficients(uiDepth, uiRow, uiCol);
	  if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
      uiNFeatures++;
    }

    //---------------------------------------
    //Energy of DCT Coefficients 2Nx2N
    if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2Nx2N))
    {
	  adFeatures[uiNFeatures] = pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficients(uiDepth, uiRow, uiCol);
	  if (bDebug) printf("Energy of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
      uiNFeatures++;
    }

	//------------- 2NxN  -------------
	j0    = uiRow_4x4;                        i0 = uiCol_4x4;
	j1    = uiRow_4x4 + (uiSize_4x4 >> 1);    i1 = uiCol_4x4;
	jSize = (uiSize_4x4 >> 1);
	iSize = uiSize_4x4;
	if (bDebug) printf("------------- 2NxN  -------------\n");
	if (bDebug) printf("P0 = (%d,%d) - P1 = (%d,%d) - Size = (%d,%d).\n",j0,i0,j1,i1,jSize,iSize);
	//---------------------------------------
    //MV Variance Distance , Y and X 2NxN
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxN) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxN) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxN))
    {
	  Double dVarianceDistance_0, dVarianceDistance_1, dVarianceY_0, dVarianceY_1, dVarianceX_0, dVarianceX_1;

	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j0,i0,jSize,iSize, &dVarianceY_0, &dVarianceX_0, &dVarianceDistance_0);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j1,i1,jSize,iSize, &dVarianceY_1, &dVarianceX_1, &dVarianceDistance_1);

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxN))
	  {
		adFeatures[uiNFeatures++] = dVarianceDistance_0;
		adFeatures[uiNFeatures++] = dVarianceDistance_1;
		if (bDebug) printf("MV Variance Distance 2NxN : %2.2f %2.2f.\n",dVarianceDistance_0,dVarianceDistance_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxN))
	  {
		adFeatures[uiNFeatures++] = dVarianceX_0;
		adFeatures[uiNFeatures++] = dVarianceX_1;
		if (bDebug) printf("MV Variance X 2NxN : %2.2f %2.2f.\n",dVarianceX_0,dVarianceX_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxN))
	  {
		adFeatures[uiNFeatures++] = dVarianceY_0;
		adFeatures[uiNFeatures++] = dVarianceY_1;
		if (bDebug) printf("MV Variance Y 2NxN : %2.2f %2.2f.\n",dVarianceY_0,dVarianceY_1);
	  }
    }

	//---------------------------------------
    //MV Phase Variance 2NxN
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2NxN))
    {
	  Double dPhaseVariance_0, dPhaseVariance_1;

	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j0,i0,jSize,iSize, &dPhaseVariance_0);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j1,i1,jSize,iSize, &dPhaseVariance_1);

	  adFeatures[uiNFeatures++] = dPhaseVariance_0;
	  adFeatures[uiNFeatures++] = dPhaseVariance_1;
	  if (bDebug) printf("MV Phase Variance 2NxN : %2.2f %2.2f.\n",dPhaseVariance_0,dPhaseVariance_1);
    }

	//---------------------------------------
    //Number of DCT Coefficients 2NxN
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2NxN))
    {
	  Double dNDCT_0, dNDCT_1;

	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j0,i0,jSize,iSize, &dNDCT_0);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j1,i1,jSize,iSize, &dNDCT_1);

	  adFeatures[uiNFeatures++] = dNDCT_0;
	  adFeatures[uiNFeatures++] = dNDCT_1;
	  if (bDebug) printf("Number of DCT Coefficients 2NxN : %2.2f %2.2f.\n",dNDCT_0,dNDCT_1);
    }

	//---------------------------------------
    //Energy of DCT Coefficients 2NxN
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2NxN))
    {
	  Double dEDCT_0, dEDCT_1;

	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j0,i0,jSize,iSize, &dEDCT_0);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j1,i1,jSize,iSize, &dEDCT_1);

	  adFeatures[uiNFeatures++] = dEDCT_0;
	  adFeatures[uiNFeatures++] = dEDCT_1;
	  if (bDebug) printf("Energy of DCT Coefficients 2NxN : %2.2f %2.2f.\n",dEDCT_0,dEDCT_1);
    }

	//------------- Nx2N  -------------
	j0    = uiRow_4x4;                        i0 = uiCol_4x4;
	j1    = uiRow_4x4;                        i1 = uiCol_4x4 + (uiSize_4x4 >> 1);
	jSize = uiSize_4x4;
	iSize = (uiSize_4x4 >> 1);
	if (bDebug) printf("------------- Nx2N  -------------\n");
	if (bDebug) printf("P0 = (%d,%d) - P1 = (%d,%d) - Size = (%d,%d).\n",j0,i0,j1,i1,jSize,iSize);
	//---------------------------------------
    //MV Variance Distance , Y and X Nx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_Nx2N) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_Nx2N) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_Nx2N))
    {
	  Double dVarianceDistance_0, dVarianceDistance_1, dVarianceY_0, dVarianceY_1, dVarianceX_0, dVarianceX_1;

	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j0,i0,jSize,iSize,  &dVarianceY_0, &dVarianceX_0, &dVarianceDistance_0);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j1,i1,jSize,iSize,  &dVarianceY_1, &dVarianceX_1, &dVarianceDistance_1);

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_Nx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceDistance_0;
		adFeatures[uiNFeatures++] = dVarianceDistance_1;
		if (bDebug) printf("MV Variance Distance Nx2N : %2.2f %2.2f.\n",dVarianceDistance_0,dVarianceDistance_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_Nx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceX_0;
		adFeatures[uiNFeatures++] = dVarianceX_1;
		if (bDebug) printf("MV Variance X Nx2N : %2.2f %2.2f.\n",dVarianceX_0,dVarianceX_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_Nx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceY_0;
		adFeatures[uiNFeatures++] = dVarianceY_1;
		if (bDebug) printf("MV Variance Y Nx2N : %2.2f %2.2f.\n",dVarianceY_0,dVarianceY_1);
	  }
    }

	//---------------------------------------
    //MV Phase Variance Nx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_Nx2N))
    {
	  Double dPhaseVariance_0, dPhaseVariance_1;

	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j0,i0,jSize,iSize, &dPhaseVariance_0);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j1,i1,jSize,iSize, &dPhaseVariance_1);

	  adFeatures[uiNFeatures++] = dPhaseVariance_0;
	  adFeatures[uiNFeatures++] = dPhaseVariance_1;
	  if (bDebug) printf("MV Phase Variance Nx2N : %2.2f %2.2f.\n",dPhaseVariance_0,dPhaseVariance_1);
    }

	//---------------------------------------
    //Number of DCT Coefficients Nx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_Nx2N))
    {
	  Double dNDCT_0, dNDCT_1;

	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j0,i0,jSize,iSize, &dNDCT_0);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j1,i1,jSize,iSize, &dNDCT_1);

	  adFeatures[uiNFeatures++] = dNDCT_0;
	  adFeatures[uiNFeatures++] = dNDCT_1;
	  if (bDebug) printf("Number of DCT Coefficients Nx2N : %2.2f %2.2f.\n",dNDCT_0,dNDCT_1);
    }

	//---------------------------------------
    //Energy of DCT Coefficients Nx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_Nx2N))
    {
	  Double dEDCT_0, dEDCT_1;

	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j0,i0,jSize,iSize, &dEDCT_0);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j1,i1,jSize,iSize, &dEDCT_1);

	  adFeatures[uiNFeatures++] = dEDCT_0;
	  adFeatures[uiNFeatures++] = dEDCT_1;
	  if (bDebug) printf("Energy of DCT Coefficients Nx2N : %2.2f %2.2f.\n",dEDCT_0,dEDCT_1);
    }

	//-------------  NxN  -------------
	j0    = uiRow_4x4;                        i0 = uiCol_4x4;
	j1    = uiRow_4x4;                        i1 = uiCol_4x4 + (uiSize_4x4 >> 1);
	j2    = uiRow_4x4 + (uiSize_4x4 >> 1);    i2 = uiCol_4x4;
	j3    = uiRow_4x4 + (uiSize_4x4 >> 1);    i3 = uiCol_4x4 + (uiSize_4x4 >> 1);
	jSize = (uiSize_4x4 >> 1);
	iSize = (uiSize_4x4 >> 1);
	if (bDebug) printf("-------------  NxN  -------------\n");
	if (bDebug) printf("P0 = (%d,%d) - P1 = (%d,%d) - P2 = (%d,%d) - P3 = (%d,%d) - Size = (%d,%d).\n",j0,i0,j1,i1,j2,i2,j3,i3,jSize,iSize);
	//---------------------------------------
    //MV Variance Distance , Y and X NxN
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_NxN) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_NxN))
    {
	  Double dVarianceDistance_0, dVarianceDistance_1, dVarianceDistance_2, dVarianceDistance_3;
	  Double dVarianceY_0, dVarianceY_1, dVarianceY_2, dVarianceY_3;
	  Double dVarianceX_0, dVarianceX_1, dVarianceX_2, dVarianceX_3;

	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j0,i0,jSize,iSize,  &dVarianceY_0, &dVarianceX_0, &dVarianceDistance_0);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j1,i1,jSize,iSize,  &dVarianceY_1, &dVarianceX_1, &dVarianceDistance_1);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j2,i2,jSize,iSize,  &dVarianceY_2, &dVarianceX_2, &dVarianceDistance_2);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j3,i3,jSize,iSize,  &dVarianceY_3, &dVarianceX_3, &dVarianceDistance_3);

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN))
	  {
		adFeatures[uiNFeatures++] = dVarianceDistance_0;
		adFeatures[uiNFeatures++] = dVarianceDistance_1;
		adFeatures[uiNFeatures++] = dVarianceDistance_2;
		adFeatures[uiNFeatures++] = dVarianceDistance_3;
		if (bDebug) printf("MV Variance Distance NxN : %2.2f %2.2f %2.2f %2.2f.\n",dVarianceDistance_0,dVarianceDistance_1, dVarianceDistance_2,dVarianceDistance_3);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_NxN))
	  {
		adFeatures[uiNFeatures++] = dVarianceX_0;
		adFeatures[uiNFeatures++] = dVarianceX_1;
		adFeatures[uiNFeatures++] = dVarianceX_2;
		adFeatures[uiNFeatures++] = dVarianceX_3;
		if (bDebug) printf("MV Variance X NxN : %2.2f %2.2f %2.2f %2.2f.\n",dVarianceX_0,dVarianceX_1,dVarianceX_2,dVarianceX_3);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_NxN))
	  {
		adFeatures[uiNFeatures++] = dVarianceY_0;
		adFeatures[uiNFeatures++] = dVarianceY_1;
		adFeatures[uiNFeatures++] = dVarianceY_2;
		adFeatures[uiNFeatures++] = dVarianceY_3;
		if (bDebug) printf("MV Variance Y NxN : %2.2f %2.2f %2.2f %2.2f.\n",dVarianceY_0,dVarianceY_1,dVarianceY_2,dVarianceY_3);
	  }
    }

	//---------------------------------------
    //MV Phase Variance NxN
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_NxN))
    {
	  Double dPhaseVariance_0, dPhaseVariance_1, dPhaseVariance_2, dPhaseVariance_3;

	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j0,i0,jSize,iSize, &dPhaseVariance_0);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j1,i1,jSize,iSize, &dPhaseVariance_1);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j2,i2,jSize,iSize, &dPhaseVariance_2);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j3,i3,jSize,iSize, &dPhaseVariance_3);

	  adFeatures[uiNFeatures++] = dPhaseVariance_0;
	  adFeatures[uiNFeatures++] = dPhaseVariance_1;
	  adFeatures[uiNFeatures++] = dPhaseVariance_2;
	  adFeatures[uiNFeatures++] = dPhaseVariance_3;
	  if (bDebug) printf("MV Phase Variance NxN : %2.2f %2.2f %2.2f %2.2f.\n",dPhaseVariance_0,dPhaseVariance_1, dPhaseVariance_2,dPhaseVariance_3);
    }

	//---------------------------------------
    //Number of DCT Coefficients NxN
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN))
    {
	  Double dNDCT_0, dNDCT_1, dNDCT_2, dNDCT_3;

	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j0,i0,jSize,iSize, &dNDCT_0);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j1,i1,jSize,iSize, &dNDCT_1);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j2,i2,jSize,iSize, &dNDCT_2);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j3,i3,jSize,iSize, &dNDCT_3);

	  adFeatures[uiNFeatures++] = dNDCT_0;
	  adFeatures[uiNFeatures++] = dNDCT_1;
	  adFeatures[uiNFeatures++] = dNDCT_2;
	  adFeatures[uiNFeatures++] = dNDCT_3;

	  if (bDebug) printf("Number of DCT Coefficients NxN : %2.2f %2.2f %2.2f %2.2f.\n",dNDCT_0,dNDCT_1,dNDCT_2,dNDCT_3);
    }

	//---------------------------------------
    //Energy of DCT Coefficients NxN
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_NxN))
    {
	  Double dEDCT_0, dEDCT_1,dEDCT_2, dEDCT_3;

	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j0,i0,jSize,iSize, &dEDCT_0);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j1,i1,jSize,iSize, &dEDCT_1);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j2,i2,jSize,iSize, &dEDCT_2);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j3,i3,jSize,iSize, &dEDCT_3);

	  adFeatures[uiNFeatures++] = dEDCT_0;
	  adFeatures[uiNFeatures++] = dEDCT_1;
	  adFeatures[uiNFeatures++] = dEDCT_2;
	  adFeatures[uiNFeatures++] = dEDCT_3;
	  if (bDebug) printf("Energy of DCT Coefficients NxN : %2.2f %2.2f %2.2f %2.2f.\n",dEDCT_0,dEDCT_1,dEDCT_2,dEDCT_3);
    }

	//------------- 2NxnU -------------
	j0    = uiRow_4x4;                        i0 = uiCol_4x4;
	j1    = uiRow_4x4 + (uiSize_4x4 >> 2);    i1 = uiCol_4x4;
	jSize0 = (uiSize_4x4 >> 2);
	iSize0 = uiSize_4x4;
	jSize1 = 3 * (uiSize_4x4 >> 2);
	iSize1 = uiSize_4x4;
	if (bDebug) printf("------------- 2NxnU -------------\n");
	if (bDebug) printf("P0 = (%d,%d) - P1 = (%d,%d) - Size0 = (%d,%d) - Size1 = (%d,%d).\n",j0,i0,j1,i1,jSize0,iSize0,jSize1,iSize1);

	//---------------------------------------
    //MV Variance Distance , Y and X 2NxnU
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxnU) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxnU) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxnU))
    {
	  Double dVarianceDistance_0, dVarianceDistance_1, dVarianceY_0, dVarianceY_1, dVarianceX_0, dVarianceX_1;

	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j0,i0,jSize0,iSize0, &dVarianceY_0, &dVarianceX_0, &dVarianceDistance_0);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j1,i1,jSize1,iSize1, &dVarianceY_1, &dVarianceX_1, &dVarianceDistance_1);

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxnU))
	  {
		adFeatures[uiNFeatures++] = dVarianceDistance_0;
		adFeatures[uiNFeatures++] = dVarianceDistance_1;
		if (bDebug) printf("MV Variance Distance 2NxnU : %2.2f %2.2f.\n",dVarianceDistance_0,dVarianceDistance_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxnU))
	  {
		adFeatures[uiNFeatures++] = dVarianceX_0;
		adFeatures[uiNFeatures++] = dVarianceX_1;
		if (bDebug) printf("MV Variance X 2NxnU : %2.2f %2.2f.\n",dVarianceX_0,dVarianceX_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxnU))
	  {
		adFeatures[uiNFeatures++] = dVarianceY_0;
		adFeatures[uiNFeatures++] = dVarianceY_1;
		if (bDebug) printf("MV Variance Y 2NxnU : %2.2f %2.2f.\n",dVarianceY_0,dVarianceY_1);
	  }
    }

	//---------------------------------------
    //MV Phase Variance 2NxnU
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2NxnU))
    {
	  Double dPhaseVariance_0, dPhaseVariance_1;

	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j0,i0,jSize0,iSize0, &dPhaseVariance_0);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j1,i1,jSize1,iSize1, &dPhaseVariance_1);

	  adFeatures[uiNFeatures++] = dPhaseVariance_0;
	  adFeatures[uiNFeatures++] = dPhaseVariance_1;
	  if (bDebug) printf("MV Phase Variance 2NxnU : %2.2f %2.2f.\n",dPhaseVariance_0,dPhaseVariance_1);
    }

	//---------------------------------------
    //Number of DCT Coefficients 2NxnU
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2NxnU))
    {
	  Double dNDCT_0, dNDCT_1;

	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j0,i0,jSize0,iSize0, &dNDCT_0);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j1,i1,jSize1,iSize1, &dNDCT_1);

	  adFeatures[uiNFeatures++] = dNDCT_0;
	  adFeatures[uiNFeatures++] = dNDCT_1;
	  if (bDebug) printf("Number of DCT Coefficients 2NxnU : %2.2f %2.2f.\n",dNDCT_0,dNDCT_1);
    }

	//---------------------------------------
    //Energy of DCT Coefficients 2NxnU
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2NxnU))
    {
	  Double dEDCT_0, dEDCT_1;

	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j0,i0,jSize0,iSize0, &dEDCT_0);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j1,i1,jSize1,iSize1, &dEDCT_1);

	  adFeatures[uiNFeatures++] = dEDCT_0;
	  adFeatures[uiNFeatures++] = dEDCT_1;
	  if (bDebug) printf("Energy of DCT Coefficients 2NxnU : %2.2f %2.2f.\n",dEDCT_0,dEDCT_1);
    }

	//------------- 2NxnD -------------
	j0    = uiRow_4x4;                         i0 = uiCol_4x4;
	j1    = uiRow_4x4 + 3 * (uiSize_4x4 >> 2); i1 = uiCol_4x4;
	jSize0 = 3 * (uiSize_4x4 >> 2);
	iSize0 = uiSize_4x4;
	jSize1 = (uiSize_4x4 >> 2);
	iSize1 = uiSize_4x4;
	if (bDebug) printf("------------- 2NxnD -------------\n");
	if (bDebug) printf("P0 = (%d,%d) - P1 = (%d,%d) - Size0 = (%d,%d) - Size1 = (%d,%d).\n",j0,i0,j1,i1,jSize0,iSize0,jSize1,iSize1);
	//---------------------------------------
    //MV Variance Distance , Y and X 2NxnD
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxnD) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxnD) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxnD))
    {
	  Double dVarianceDistance_0, dVarianceDistance_1, dVarianceY_0, dVarianceY_1, dVarianceX_0, dVarianceX_1;

	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j0,i0,jSize0,iSize0, &dVarianceY_0, &dVarianceX_0, &dVarianceDistance_0);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j1,i1,jSize1,iSize1, &dVarianceY_1, &dVarianceX_1, &dVarianceDistance_1);

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxnD))
	  {
		adFeatures[uiNFeatures++] = dVarianceDistance_0;
		adFeatures[uiNFeatures++] = dVarianceDistance_1;
		if (bDebug) printf("MV Variance Distance 2NxnD : %2.2f %2.2f.\n",dVarianceDistance_0,dVarianceDistance_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxnD))
	  {
		adFeatures[uiNFeatures++] = dVarianceX_0;
		adFeatures[uiNFeatures++] = dVarianceX_1;
		if (bDebug) printf("MV Variance X 2NxnD : %2.2f %2.2f.\n",dVarianceX_0,dVarianceX_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxnD))
	  {
		adFeatures[uiNFeatures++] = dVarianceY_0;
		adFeatures[uiNFeatures++] = dVarianceY_1;
		if (bDebug) printf("MV Variance Y 2NxnD : %2.2f %2.2f.\n",dVarianceY_0,dVarianceY_1);
	  }
    }

	//---------------------------------------
    //MV Phase Variance 2NxnD
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2NxnD))
    {
	  Double dPhaseVariance_0, dPhaseVariance_1;

	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j0,i0,jSize0,iSize0, &dPhaseVariance_0);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j1,i1,jSize1,iSize1, &dPhaseVariance_1);

	  adFeatures[uiNFeatures++] = dPhaseVariance_0;
	  adFeatures[uiNFeatures++] = dPhaseVariance_1;
	  if (bDebug) printf("MV Phase Variance 2NxnD : %2.2f %2.2f.\n",dPhaseVariance_0,dPhaseVariance_1);
    }

	//---------------------------------------
    //Number of DCT Coefficients 2NxnD
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2NxnD))
    {
	  Double dNDCT_0, dNDCT_1;

	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j0,i0,jSize0,iSize0, &dNDCT_0);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j1,i1,jSize1,iSize1, &dNDCT_1);

	  adFeatures[uiNFeatures++] = dNDCT_0;
	  adFeatures[uiNFeatures++] = dNDCT_1;
	  if (bDebug) printf("Number of DCT Coefficients 2NxnD : %2.2f %2.2f.\n",dNDCT_0,dNDCT_1);
    }

	//---------------------------------------
    //Energy of DCT Coefficients 2NxnD
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2NxnD))
    {
	  Double dEDCT_0, dEDCT_1;

	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j0,i0,jSize0,iSize0, &dEDCT_0);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j1,i1,jSize1,iSize1, &dEDCT_1);

	  adFeatures[uiNFeatures++] = dEDCT_0;
	  adFeatures[uiNFeatures++] = dEDCT_1;
	  if (bDebug) printf("Energy of DCT Coefficients 2NxnD : %2.2f %2.2f.\n",dEDCT_0,dEDCT_1);
    }

	//------------- nLx2N -------------
	j0    = uiRow_4x4;                         i0 = uiCol_4x4;
	j1    = uiRow_4x4;                         i1 = uiCol_4x4 + (uiSize_4x4 >> 2);
	jSize0 = uiSize_4x4;
	iSize0 = (uiSize_4x4 >> 2);
	jSize1 = uiSize_4x4;
	iSize1 = 3 * (uiSize_4x4 >> 2);
	if (bDebug) printf("------------- nLx2N -------------\n");
	if (bDebug) printf("P0 = (%d,%d) - P1 = (%d,%d) - Size0 = (%d,%d) - Size1 = (%d,%d).\n",j0,i0,j1,i1,jSize0,iSize0,jSize1,iSize1);
	//---------------------------------------
    //MV Variance Distance , Y and X nLx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_nLx2N) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_nLx2N) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_nLx2N))
    {
	  Double dVarianceDistance_0, dVarianceDistance_1, dVarianceY_0, dVarianceY_1, dVarianceX_0, dVarianceX_1;

	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j0,i0,jSize0,iSize0,  &dVarianceY_0, &dVarianceX_0, &dVarianceDistance_0);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j1,i1,jSize1,iSize1,  &dVarianceY_1, &dVarianceX_1, &dVarianceDistance_1);

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_nLx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceDistance_0;
		adFeatures[uiNFeatures++] = dVarianceDistance_1;
		if (bDebug) printf("MV Variance Distance nLx2N : %2.2f %2.2f.\n",dVarianceDistance_0,dVarianceDistance_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_nLx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceX_0;
		adFeatures[uiNFeatures++] = dVarianceX_1;
		if (bDebug) printf("MV Variance X nLx2N : %2.2f %2.2f.\n",dVarianceX_0,dVarianceX_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_nLx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceY_0;
		adFeatures[uiNFeatures++] = dVarianceY_1;
		if (bDebug) printf("MV Variance Y nLx2N : %2.2f %2.2f.\n",dVarianceY_0,dVarianceY_1);
	  }
    }

	//---------------------------------------
    //MV Phase Variance nLx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_nLx2N))
    {
	  Double dPhaseVariance_0, dPhaseVariance_1;

	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j0,i0,jSize0,iSize0, &dPhaseVariance_0);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j1,i1,jSize1,iSize1, &dPhaseVariance_1);

	  adFeatures[uiNFeatures++] = dPhaseVariance_0;
	  adFeatures[uiNFeatures++] = dPhaseVariance_1;
	  if (bDebug) printf("MV Phase Variance nLx2N : %2.2f %2.2f.\n",dPhaseVariance_0,dPhaseVariance_1);
    }

	//---------------------------------------
    //Number of DCT Coefficients nLx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_nLx2N))
    {
	  Double dNDCT_0, dNDCT_1;

	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j0,i0,jSize0,iSize0, &dNDCT_0);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j1,i1,jSize1,iSize1, &dNDCT_1);

	  adFeatures[uiNFeatures++] = dNDCT_0;
	  adFeatures[uiNFeatures++] = dNDCT_1;
	  if (bDebug) printf("Number of DCT Coefficients nLx2N : %2.2f %2.2f.\n",dNDCT_0,dNDCT_1);
    }

	//---------------------------------------
    //Energy of DCT Coefficients nLx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_nLx2N))
    {
	  Double dEDCT_0, dEDCT_1;

	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j0,i0,jSize0,iSize0, &dEDCT_0);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j1,i1,jSize1,iSize1, &dEDCT_1);

	  adFeatures[uiNFeatures++] = dEDCT_0;
	  adFeatures[uiNFeatures++] = dEDCT_1;
	  if (bDebug) printf("Energy of DCT Coefficients nLx2N : %2.2f %2.2f.\n",dEDCT_0,dEDCT_1);
    }

	//------------- nRx2N -------------
	j0    = uiRow_4x4;                         i0 = uiCol_4x4;
	j1    = uiRow_4x4;                         i1 = uiCol_4x4 + 3 * (uiSize_4x4 >> 2);
	jSize0 = uiSize_4x4;
	iSize0 = 3 * (uiSize_4x4 >> 2);
	jSize1 = uiSize_4x4;
	iSize1 = (uiSize_4x4 >> 2);
	if (bDebug) printf("------------- nRx2N -------------\n");
	if (bDebug) printf("P0 = (%d,%d) - P1 = (%d,%d) - Size0 = (%d,%d) - Size1 = (%d,%d).\n",j0,i0,j1,i1,jSize0,iSize0,jSize1,iSize1);
	//---------------------------------------
    //MV Variance Distance , Y and X nRx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_nRx2N) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_nRx2N) || pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_nRx2N))
    {
	  Double dVarianceDistance_0, dVarianceDistance_1, dVarianceY_0, dVarianceY_1, dVarianceX_0, dVarianceX_1;

	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j0,i0,jSize0,iSize0,  &dVarianceY_0, &dVarianceX_0, &dVarianceDistance_0);
	  pcTranscoderEngine->xComputeFeatureMVVarianceForARegion(j1,i1,jSize1,iSize1,  &dVarianceY_1, &dVarianceX_1, &dVarianceDistance_1);

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_nRx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceDistance_0;
		adFeatures[uiNFeatures++] = dVarianceDistance_1;
		if (bDebug) printf("MV Variance Distance nRx2N : %2.2f %2.2f.\n",dVarianceDistance_0,dVarianceDistance_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_nRx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceX_0;
		adFeatures[uiNFeatures++] = dVarianceX_1;
		if (bDebug) printf("MV Variance X nRx2N : %2.2f %2.2f.\n",dVarianceX_0,dVarianceX_1);
	  }

	  if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_nRx2N))
	  {
		adFeatures[uiNFeatures++] = dVarianceY_0;
		adFeatures[uiNFeatures++] = dVarianceY_1;
		if (bDebug) printf("MV Variance Y nRx2N : %2.2f %2.2f.\n",dVarianceY_0,dVarianceY_1);
	  }
    }

	//---------------------------------------
    //MV Phase Variance nRx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_nRx2N))
    {
	  Double dPhaseVariance_0, dPhaseVariance_1;

	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j0,i0,jSize0,iSize0, &dPhaseVariance_0);
	  pcTranscoderEngine->xComputeFeatureMVPhaseVarianceForARegion(j1,i1,jSize1,iSize1, &dPhaseVariance_1);

	  adFeatures[uiNFeatures++] = dPhaseVariance_0;
	  adFeatures[uiNFeatures++] = dPhaseVariance_1;
	  if (bDebug) printf("MV Phase Variance nRx2N : %2.2f %2.2f.\n",dPhaseVariance_0,dPhaseVariance_1);
    }

	//---------------------------------------
    //Number of DCT Coefficients nRx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_nRx2N))
    {
	  Double dNDCT_0, dNDCT_1;

	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j0,i0,jSize0,iSize0, &dNDCT_0);
	  pcTranscoderEngine->xComputeFeatureNDCTCoefficientsForARegion(j1,i1,jSize1,iSize1, &dNDCT_1);

	  adFeatures[uiNFeatures++] = dNDCT_0;
	  adFeatures[uiNFeatures++] = dNDCT_1;
	  if (bDebug) printf("Number of DCT Coefficients nRx2N : %2.2f %2.2f.\n",dNDCT_0,dNDCT_1);
    }

	//---------------------------------------
    //Energy of DCT Coefficients nRx2N
	if (pcTranscoderEngine->xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_nRx2N))
    {
	  Double dEDCT_0, dEDCT_1;

	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j0,i0,jSize0,iSize0, &dEDCT_0);
	  pcTranscoderEngine->xComputeFeatureEnergyDCTCoefficientsForARegion(j1,i1,jSize1,iSize1, &dEDCT_1);

	  adFeatures[uiNFeatures++] = dEDCT_0;
	  adFeatures[uiNFeatures++] = dEDCT_1;
	  if (bDebug) printf("Energy of DCT Coefficients nRx2N : %2.2f %2.2f.\n",dEDCT_0,dEDCT_1);
    }

	//---------------------------------------
    //MB Mode Distribution
	if (false)
	{
	  //ASSIM QUE CALCULA. AGORA TEM QUE INTEGRAR ISSO NO RESTO.
	  Double *mbDistribution = pcTranscoderEngine->xComputeFeatureH264MBDistribution(uiDepth, uiRow, uiCol);
	  //printf("CU: (%d,%d) - size:  %d.\n",uiTPelY,uiLPelX,uiSize);
	  printf("CU: (%d,%d) - depth: %d.\n",uiRow,uiCol,uiDepth);
	  printf("CU 4x4 Coordinates: (%d,%d) - size %d.\n",uiRow_4x4,uiCol_4x4,uiSize_4x4);
	  printf("MB Distribution   : [ %1.2f %1.2f %1.2f %1.2f %1.2f %1.2f %1.2f %1.2f %1.2f ] \n",mbDistribution[0],mbDistribution[1],mbDistribution[2],mbDistribution[3],mbDistribution[4],mbDistribution[5],mbDistribution[6],mbDistribution[7],mbDistribution[8]);
	  delete mbDistribution;
	  printf("BREAKPOINT.\n");
	}

  return adFeatures;
  */
  return NULL;
}

/**
 * Computes the features for LDF v4.
 */
Double* TTrEngine::computeFeaturesLDF_v3(UInt uiDepth, UInt uiRow, UInt uiCol, Double *adHEVCCost)
{
  Bool bDebug = false;

  //These are the entities that I want to compute:
  Double *adFeatures;
  UInt uiTotalFeatures;
  UInt uiNFeatures = 0;

  //Allocates the feature array.
  uiTotalFeatures = getNumberOfFeatures();
  adFeatures = (Double*) malloc(uiTotalFeatures * sizeof(Double));

  //The level of the 4x4 block.
  //UInt uiLevel_4x4 = getCurrentH264FrameMI()->getTransformInfo()->getLevel_4x4();
  //Get the coordinates of the CU (in 4x4 blocks)
  //UInt uiRow_4x4  = uiRow << (uiLevel_4x4 - uiDepth);
  //UInt uiCol_4x4  = uiCol << (uiLevel_4x4 - uiDepth);
  //UInt uiSize_4x4 =     1 << (uiLevel_4x4 - uiDepth);

  if (bDebug) printf("-------------  ONE  -------------\n");
  //---------------------------------------
  //One
  if (xComputeFeature(TR_H264FEATURE_ONE))
  {
	adFeatures[uiNFeatures] = 1;
	if (bDebug) printf("One: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
  {
    adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance NxN
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN))
  {
    adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1), (uiCol << 1));
	if (bDebug) printf("MV Variance Distance NxN block 0: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	if (bDebug) printf("MV Variance Distance NxN block 1: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	if (bDebug) printf("MV Variance Distance NxN block 2: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);
	if (bDebug) printf("MV Variance Distance NxN block 3: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Phase Variance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Phase Variance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //MV Phase Variance NxN
  if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_NxN))
  {
    adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1), (uiCol << 1));
	if (bDebug) printf("MV Phase Variance NxN block 0: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	if (bDebug) printf("MV Phase Variance NxN block 1: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	if (bDebug) printf("MV Phase Variance NxN block 2: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);
	if (bDebug) printf("MV Phase Variance NxN block 3: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //Number of DCT Coefficients 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth, uiRow, uiCol);
	if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //Number of DCT Coefficients NxN
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN))
  {
    adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1), (uiCol << 1));
	if (bDebug) printf("Number of DCT Coefficients NxN block 0: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	if (bDebug) printf("Number of DCT Coefficients NxN block 1: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	if (bDebug) printf("Number of DCT Coefficients NxN block 2: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);
	if (bDebug) printf("Number of DCT Coefficients NxN block 3: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MB Distribution 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))
  {
	Double dSKIP, d16,d8,dIntra;
	xComputeFeatureMBDistribution4Classes(uiDepth, uiRow, uiCol,&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes 2Nx2N: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);
  }

  //---------------------------------------
  //MB Distribution NxN
  if (xComputeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))
  {
	Double dSKIP, d16,d8,dIntra;

	xComputeFeatureMBDistribution4Classes(uiDepth+1, (uiRow << 1), (uiCol << 1),&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes NxN block 0: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);

	xComputeFeatureMBDistribution4Classes(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1,&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes NxN block 1: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);

	xComputeFeatureMBDistribution4Classes(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1),&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes NxN block 2: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);

	xComputeFeatureMBDistribution4Classes(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1,&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes NxN block 3: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);
  }

  if (bDebug) printf("-------------  MEAN -------------\n");
  //---------------------------------------
  //Mean
  if (xComputeFeature(TR_H264FEATURE_MEAN))
  {
	Double dSum = 0.0;
	for (UInt ui_k = (xComputeFeature(TR_H264FEATURE_ONE) ? 1 : 0) ; ui_k < uiNFeatures; ui_k++)
	{
	  dSum += adFeatures[ui_k];
	}
	adFeatures[uiNFeatures] = dSum / (Double) ( (xComputeFeature(TR_H264FEATURE_ONE) ? (uiNFeatures - 1) : uiNFeatures) );
	if (bDebug) printf("Mean: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  if (xComputeFeature(TR_HEVCFEATURE_COST))
  {
	for (UInt ui_k = 0; ui_k < 11; ui_k++)
	{
	  adFeatures[uiNFeatures] = adHEVCCost[ui_k];
	  uiNFeatures++;
	}
  }

  return adFeatures;
}

/**
 * Computes the features for LDF v4.
 */
Double* TTrEngine::computeFeaturesLDF_v4(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Bool bDebug = false;

  //These are the entities that I want to compute:
  Double *adFeatures;
  UInt uiTotalFeatures;
  UInt uiNFeatures = 0;

  //Allocates the feature array.
  uiTotalFeatures = getNumberOfFeatures();
  adFeatures = (Double*) malloc(uiTotalFeatures * sizeof(Double));

  //The level of the 4x4 block.
  //UInt uiLevel_4x4 = getCurrentH264FrameMI()->getTransformInfo()->getLevel_4x4();
  //Get the coordinates of the CU (in 4x4 blocks)
  //UInt uiRow_4x4  = uiRow << (uiLevel_4x4 - uiDepth);
  //UInt uiCol_4x4  = uiCol << (uiLevel_4x4 - uiDepth);
  //UInt uiSize_4x4 =     1 << (uiLevel_4x4 - uiDepth);

  if (bDebug) printf("-------------  ONE  -------------\n");
  //---------------------------------------
  //One
  if (xComputeFeature(TR_H264FEATURE_ONE))
  {
	adFeatures[uiNFeatures] = 1;
	if (bDebug) printf("One: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
  {
    adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance NxN
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN))
  {
    adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1), (uiCol << 1));
	if (bDebug) printf("MV Variance Distance NxN block 0: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	if (bDebug) printf("MV Variance Distance NxN block 1: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	if (bDebug) printf("MV Variance Distance NxN block 2: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);
	if (bDebug) printf("MV Variance Distance NxN block 3: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Phase Variance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Phase Variance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //MV Phase Variance NxN
  if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_NxN))
  {
    adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1), (uiCol << 1));
	if (bDebug) printf("MV Phase Variance NxN block 0: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	if (bDebug) printf("MV Phase Variance NxN block 1: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	if (bDebug) printf("MV Phase Variance NxN block 2: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);
	if (bDebug) printf("MV Phase Variance NxN block 3: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //Number of DCT Coefficients 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth, uiRow, uiCol);
	if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //Number of DCT Coefficients NxN
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN))
  {
    adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1), (uiCol << 1));
	if (bDebug) printf("Number of DCT Coefficients NxN block 0: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	if (bDebug) printf("Number of DCT Coefficients NxN block 1: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	if (bDebug) printf("Number of DCT Coefficients NxN block 2: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;

	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);
	if (bDebug) printf("Number of DCT Coefficients NxN block 3: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MB Distribution 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))
  {
	Double dSKIP, d16,d8,dIntra;
	xComputeFeatureMBDistribution4Classes(uiDepth, uiRow, uiCol,&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes 2Nx2N: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);
  }

  //---------------------------------------
  //MB Distribution NxN
  if (xComputeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))
  {
	Double dSKIP, d16,d8,dIntra;

	xComputeFeatureMBDistribution4Classes(uiDepth+1, (uiRow << 1), (uiCol << 1),&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes NxN block 0: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);

	xComputeFeatureMBDistribution4Classes(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1,&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes NxN block 1: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);

	xComputeFeatureMBDistribution4Classes(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1),&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes NxN block 2: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);

	xComputeFeatureMBDistribution4Classes(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1,&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes NxN block 3: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);
  }

  if (bDebug) printf("-------------  MEAN -------------\n");
  //---------------------------------------
  //Mean
  if (xComputeFeature(TR_H264FEATURE_MEAN))
  {
	Double dSum = 0.0;
	for (UInt ui_k = (xComputeFeature(TR_H264FEATURE_ONE) ? 1 : 0) ; ui_k < uiNFeatures; ui_k++)
	{
	  dSum += adFeatures[ui_k];
	}
	adFeatures[uiNFeatures] = dSum / (Double) ( (xComputeFeature(TR_H264FEATURE_ONE) ? (uiNFeatures - 1) : uiNFeatures) );
	if (bDebug) printf("Mean: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }
  return adFeatures;
}

/**
 * Computes the features for LDF v5.
 */
Double* TTrEngine::computeFeaturesLDF_v5(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Bool bDebug = false;

  //These are the entities that I want to compute:
  Double *adFeatures;
  UInt uiTotalFeatures;
  UInt uiNFeatures = 0;

  //Allocates the feature array.
  uiTotalFeatures = getNumberOfFeatures();
  //printf("Number of features: %d\n", uiTotalFeatures);
  adFeatures = (Double*) malloc(uiTotalFeatures * sizeof(Double));

  if (bDebug) printf("-------------  ONE  -------------\n");
  //---------------------------------------
  //One
  if (xComputeFeature(TR_H264FEATURE_ONE))
  {
	adFeatures[uiNFeatures] = 1;
	if (bDebug) printf("One: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
  {
    adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance NxN
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN_MAX))
  {
	Double dMVVD_block0 = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1), (uiCol << 1));
	Double dMVVD_block1 = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	Double dMVVD_block2 = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	Double dMVVD_block3 = xComputeFeatureMVVarianceDistance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);

	Double dMVVD_max    = sMAX( sMAX(dMVVD_block0,dMVVD_block1) , sMAX(dMVVD_block2,dMVVD_block3) );

    adFeatures[uiNFeatures] = dMVVD_max;
	if (bDebug) printf("MV Variance Distance NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n",dMVVD_block0 , dMVVD_block1 , dMVVD_block2 ,  dMVVD_block3 ,  adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Phase Variance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth, uiRow, uiCol);
	if (bDebug) printf("MV Phase Variance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //MV Phase Variance NxN
  if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_NxN_MAX))
  {
	Double dMVPhase_block0 = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1), (uiCol << 1));
	Double dMVPhase_block1 = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	Double dMVPhase_block2 = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	Double dMVPhase_block3 = xComputeFeatureMVPhaseVariance(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);

	Double dMVPhase_max    = sMAX( sMAX(dMVPhase_block0,dMVPhase_block1) , sMAX(dMVPhase_block2,dMVPhase_block3) );

    adFeatures[uiNFeatures] = dMVPhase_max;
	if (bDebug) printf("MV Phase Variance NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n",dMVPhase_block0 , dMVPhase_block1 , dMVPhase_block2 ,  dMVPhase_block3 ,  adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //Number of DCT Coefficients 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
  {
	adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth, uiRow, uiCol);
	if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //Number of DCT Coefficients NxN
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN_MAX))
  {
	Double dNDCT_block0 = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1), (uiCol << 1));
	Double dNDCT_block1 = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1), (uiCol << 1) + 1);
	Double dNDCT_block2 = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1));
	Double dNDCT_block3 = xComputeFeatureNDCTCoefficients(uiDepth+1, (uiRow << 1) + 1, (uiCol << 1) + 1);

	Double dNDCT_max    = sMAX( sMAX(dNDCT_block0,dNDCT_block1) , sMAX(dNDCT_block2,dNDCT_block3) );

    adFeatures[uiNFeatures] = dNDCT_max;
	if (bDebug) printf("Number of DCT Coefficients NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n",dNDCT_block0 , dNDCT_block1 , dNDCT_block2 ,  dNDCT_block3 ,  adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MB Distribution 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))
  {
	Double dSKIP, d16,d8,dIntra;
	xComputeFeatureMBDistribution4Classes(uiDepth, uiRow, uiCol,&dSKIP,&d16,&d8,&dIntra);
	adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
	adFeatures[uiNFeatures] = d16;    uiNFeatures++;
	adFeatures[uiNFeatures] = d8;     uiNFeatures++;
	adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
    if (bDebug) printf("MB Distribution 4 Classes 2Nx2N: [ %2.2f %2.2f %2.2f %2.2f ].\n",dSKIP, d16,d8,dIntra);
  }

  if (bDebug) printf("-------------  MEAN -------------\n");
  //---------------------------------------
  //Mean
  if (xComputeFeature(TR_H264FEATURE_MEAN))
  {
	Double dSum = 0.0;
	for (UInt ui_k = (xComputeFeature(TR_H264FEATURE_ONE) ? 1 : 0) ; ui_k < uiNFeatures; ui_k++)
	{
	  dSum += adFeatures[ui_k];
	}
	adFeatures[uiNFeatures] = dSum / (Double) ( (xComputeFeature(TR_H264FEATURE_ONE) ? (uiNFeatures - 1) : uiNFeatures) );
	if (bDebug) printf("Mean: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //printf("BREAKPOINT \n");
  return adFeatures;
}

/**
* Computes the features for LDF v6.
*/
Double* TTrEngine::computeFeaturesLDF_v6(UInt uiDepth, UInt uiRow, UInt uiCol)
{
	Bool bDebug = false;

	//These are the entities that I want to compute:
	Double *adFeatures;
	UInt uiTotalFeatures;
	UInt uiNFeatures = 0;

	//Allocates the feature array.
	uiTotalFeatures = getNumberOfFeatures();
	adFeatures = (Double*)malloc(uiTotalFeatures * sizeof(Double));

	if (bDebug) printf("-------------  ONE  -------------\n");
	//---------------------------------------
	//One
	if (xComputeFeature(TR_H264FEATURE_ONE))
	{
		adFeatures[uiNFeatures] = 1;
		if (bDebug) printf("One: %3.0f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Variance Distance 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
	{
		adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth, uiRow, uiCol);
		if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Variance Distance NxN
	if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN_MAX))
	{
		Double dMVVD_block0 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1), (uiCol << 1));
		Double dMVVD_block1 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1), (uiCol << 1) + 1);
		Double dMVVD_block2 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1));
		Double dMVVD_block3 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1) + 1);

		Double dMVVD_max = sMAX(sMAX(dMVVD_block0, dMVVD_block1), sMAX(dMVVD_block2, dMVVD_block3));

		adFeatures[uiNFeatures] = dMVVD_max;
		if (bDebug) printf("MV Variance Distance NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n", dMVVD_block0, dMVVD_block1, dMVVD_block2, dMVVD_block3, adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Phase Variance 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2Nx2N))
	{
		adFeatures[uiNFeatures] = xComputeFeatureMVPhaseVariance(uiDepth, uiRow, uiCol);
		if (bDebug) printf("MV Phase Variance 2Nx2N: %2.2f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Phase Variance NxN
	if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_NxN_MAX))
	{
		Double dMVPhase_block0 = xComputeFeatureMVPhaseVariance(uiDepth + 1, (uiRow << 1), (uiCol << 1));
		Double dMVPhase_block1 = xComputeFeatureMVPhaseVariance(uiDepth + 1, (uiRow << 1), (uiCol << 1) + 1);
		Double dMVPhase_block2 = xComputeFeatureMVPhaseVariance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1));
		Double dMVPhase_block3 = xComputeFeatureMVPhaseVariance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1) + 1);

		Double dMVPhase_max = sMAX(sMAX(dMVPhase_block0, dMVPhase_block1), sMAX(dMVPhase_block2, dMVPhase_block3));

		adFeatures[uiNFeatures] = dMVPhase_max;
		if (bDebug) printf("MV Phase Variance NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n", dMVPhase_block0, dMVPhase_block1, dMVPhase_block2, dMVPhase_block3, adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//Number of DCT Coefficients 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
	{
		adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth, uiRow, uiCol);
		if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//Number of DCT Coefficients NxN
	if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN_MAX))
	{
		Double dNDCT_block0 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1), (uiCol << 1));
		Double dNDCT_block1 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1), (uiCol << 1) + 1);
		Double dNDCT_block2 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1));
		Double dNDCT_block3 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1) + 1);

		Double dNDCT_max = sMAX(sMAX(dNDCT_block0, dNDCT_block1), sMAX(dNDCT_block2, dNDCT_block3));

		adFeatures[uiNFeatures] = dNDCT_max;
		if (bDebug) printf("Number of DCT Coefficients NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n", dNDCT_block0, dNDCT_block1, dNDCT_block2, dNDCT_block3, adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MB Distribution 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))
	{
		Double dSKIP, d16, d8, dIntra;
		xComputeFeatureMBDistribution4Classes(uiDepth, uiRow, uiCol, &dSKIP, &d16, &d8, &dIntra);
		adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
		adFeatures[uiNFeatures] = d16;    uiNFeatures++;
		adFeatures[uiNFeatures] = d8;     uiNFeatures++;
		adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
		if (bDebug) printf("MB Distribution 4 Classes 2Nx2N: [ %2.2f %2.2f %2.2f %2.2f ].\n", dSKIP, d16, d8, dIntra);
	}

	if (bDebug) printf("-------------  MEAN -------------\n");
	//---------------------------------------
	//Mean
	if (xComputeFeature(TR_H264FEATURE_MEAN))
	{
		Double dSum = 0.0;
		for (UInt ui_k = (xComputeFeature(TR_H264FEATURE_ONE) ? 1 : 0); ui_k < uiNFeatures; ui_k++)
		{
			dSum += adFeatures[ui_k];
		}
		adFeatures[uiNFeatures] = dSum / (Double)((xComputeFeature(TR_H264FEATURE_ONE) ? (uiNFeatures - 1) : uiNFeatures));
		if (bDebug) printf("Mean: %3.0f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//printf("BREAKPOINT \n");
	return adFeatures;
}

/**
* Computes the features for PIBIC_v1.
*/
Double* TTrEngine::computeFeaturesPIBIC_v1(UInt uiDepth, UInt uiRow, UInt uiCol)
{
	Bool bDebug = false;

	//These are the entities that I want to compute:
	Double *adFeatures;
	UInt uiTotalFeatures;
	UInt uiNFeatures = 0;

	//Allocates the feature array.
	uiTotalFeatures = getNumberOfFeatures();
	adFeatures = (Double*)malloc(uiTotalFeatures * sizeof(Double));

	if (bDebug) printf("-------------  ONE  -------------\n");
	//---------------------------------------
	//One
	if (xComputeFeature(TR_H264FEATURE_ONE))
	{
		adFeatures[uiNFeatures] = 1;
		if (bDebug) printf("One: %3.0f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Variance Distance 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
	{
		adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth, uiRow, uiCol);
		if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Variance Distance NxN
	if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN_MAX))
	{
		Double dMVVD_block0 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1), (uiCol << 1));
		Double dMVVD_block1 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1), (uiCol << 1) + 1);
		Double dMVVD_block2 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1));
		Double dMVVD_block3 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1) + 1);

		Double dMVVD_max = sMAX(sMAX(dMVVD_block0, dMVVD_block1), sMAX(dMVVD_block2, dMVVD_block3));

		adFeatures[uiNFeatures] = dMVVD_max;
		if (bDebug) printf("MV Variance Distance NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n", dMVVD_block0, dMVVD_block1, dMVVD_block2, dMVVD_block3, adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//Number of DCT Coefficients 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
	{
		adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth, uiRow, uiCol);
		if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//Number of DCT Coefficients NxN
	if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN_MAX))
	{
		Double dNDCT_block0 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1), (uiCol << 1));
		Double dNDCT_block1 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1), (uiCol << 1) + 1);
		Double dNDCT_block2 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1));
		Double dNDCT_block3 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1) + 1);

		Double dNDCT_max = sMAX(sMAX(dNDCT_block0, dNDCT_block1), sMAX(dNDCT_block2, dNDCT_block3));

		adFeatures[uiNFeatures] = dNDCT_max;
		if (bDebug) printf("Number of DCT Coefficients NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n", dNDCT_block0, dNDCT_block1, dNDCT_block2, dNDCT_block3, adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MB Distribution 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))
	{
		Double dSKIP, d16, d8, dIntra;
		xComputeFeatureMBDistribution4Classes(uiDepth, uiRow, uiCol, &dSKIP, &d16, &d8, &dIntra);
		adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
		adFeatures[uiNFeatures] = d16;    uiNFeatures++;
		adFeatures[uiNFeatures] = d8;     uiNFeatures++;
		adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
		if (bDebug) printf("MB Distribution 4 Classes 2Nx2N: [ %2.2f %2.2f %2.2f %2.2f ].\n", dSKIP, d16, d8, dIntra);
	}

	if (bDebug) printf("-------------  MEAN -------------\n");
	//---------------------------------------
	//Mean
	if (xComputeFeature(TR_H264FEATURE_MEAN))
	{
		Double dSum = 0.0;
		for (UInt ui_k = (xComputeFeature(TR_H264FEATURE_ONE) ? 1 : 0); ui_k < uiNFeatures; ui_k++)
		{
			dSum += adFeatures[ui_k];
		}
		adFeatures[uiNFeatures] = dSum / (Double)((xComputeFeature(TR_H264FEATURE_ONE) ? (uiNFeatures - 1) : uiNFeatures));
		if (bDebug) printf("Mean: %3.0f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//printf("BREAKPOINT \n");
	return adFeatures;
}

/**
* Computes the features for PIBIC_v2.
*/
Double* TTrEngine::computeFeaturesPIBIC_v2(UInt uiDepth, UInt uiRow, UInt uiCol)
{
	Bool bDebug = false;

	//These are the entities that I want to compute:
	Double *adFeatures;
	UInt uiTotalFeatures;
	UInt uiNFeatures = 0;

	//Allocates the feature array.
	uiTotalFeatures = getNumberOfFeatures();
	adFeatures = (Double*)malloc(uiTotalFeatures * sizeof(Double));

	if (bDebug) printf("-------------  ONE  -------------\n");
	//---------------------------------------
	//One
	if (xComputeFeature(TR_H264FEATURE_ONE))
	{
		adFeatures[uiNFeatures] = 1;
		if (bDebug) printf("One: %3.0f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Variance Distance 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
	{
		adFeatures[uiNFeatures] = xComputeFeatureMVVarianceDistance(uiDepth, uiRow, uiCol);
		if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Variance Distance NxN
	if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN_MAX))
	{
		Double dMVVD_block0 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1), (uiCol << 1));
		Double dMVVD_block1 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1), (uiCol << 1) + 1);
		Double dMVVD_block2 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1));
		Double dMVVD_block3 = xComputeFeatureMVVarianceDistance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1) + 1);

		Double dMVVD_max = sMAX(sMAX(dMVVD_block0, dMVVD_block1), sMAX(dMVVD_block2, dMVVD_block3));

		adFeatures[uiNFeatures] = dMVVD_max;
		if (bDebug) printf("MV Variance Distance NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n", dMVVD_block0, dMVVD_block1, dMVVD_block2, dMVVD_block3, adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Phase Trigonometric Variance 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_MV_PHASE_TRIGONOMETRIC_VARIANCE_2Nx2N))
	{
		adFeatures[uiNFeatures] = xComputeFeatureMVPhaseTrigonometricVariance(uiDepth, uiRow, uiCol);
		if (bDebug) printf("MV Phase Trigonometric Variance 2Nx2N: %2.2f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MV Variance Distance NxN
	if (xComputeFeature(TR_H264FEATURE_MV_PHASE_TRIGONOMETRIC_VARIANCE_NxN_MAX))
	{
		Double dMVPhaseTrig_block0 = xComputeFeatureMVPhaseTrigonometricVariance(uiDepth + 1, (uiRow << 1), (uiCol << 1));
		Double dMVPhaseTrig_block1 = xComputeFeatureMVPhaseTrigonometricVariance(uiDepth + 1, (uiRow << 1), (uiCol << 1) + 1);
		Double dMVPhaseTrig_block2 = xComputeFeatureMVPhaseTrigonometricVariance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1));
		Double dMVPhaseTrig_block3 = xComputeFeatureMVPhaseTrigonometricVariance(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1) + 1);

		Double dMVPhaseTrig_max = sMAX(sMAX(dMVPhaseTrig_block0, dMVPhaseTrig_block1), sMAX(dMVPhaseTrig_block2, dMVPhaseTrig_block3));

		adFeatures[uiNFeatures] = dMVPhaseTrig_max;
		if (bDebug) printf("MV Variance Distance NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n", dMVPhaseTrig_block0, dMVPhaseTrig_block1, dMVPhaseTrig_block2, dMVPhaseTrig_block3, adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//Number of DCT Coefficients 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
	{
		adFeatures[uiNFeatures] = xComputeFeatureNDCTCoefficients(uiDepth, uiRow, uiCol);
		if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//Number of DCT Coefficients NxN
	if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN_MAX))
	{
		Double dNDCT_block0 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1), (uiCol << 1));
		Double dNDCT_block1 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1), (uiCol << 1) + 1);
		Double dNDCT_block2 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1));
		Double dNDCT_block3 = xComputeFeatureNDCTCoefficients(uiDepth + 1, (uiRow << 1) + 1, (uiCol << 1) + 1);

		Double dNDCT_max = sMAX(sMAX(dNDCT_block0, dNDCT_block1), sMAX(dNDCT_block2, dNDCT_block3));

		adFeatures[uiNFeatures] = dNDCT_max;
		if (bDebug) printf("Number of DCT Coefficients NxN : [ %2.2f %2.2f %2.2f %2.2f ] -> MAX: %2.2f .\n", dNDCT_block0, dNDCT_block1, dNDCT_block2, dNDCT_block3, adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//---------------------------------------
	//MB Distribution 2Nx2N
	if (xComputeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))
	{
		Double dSKIP, d16, d8, dIntra;
		xComputeFeatureMBDistribution4Classes(uiDepth, uiRow, uiCol, &dSKIP, &d16, &d8, &dIntra);
		adFeatures[uiNFeatures] = dSKIP;  uiNFeatures++;
		adFeatures[uiNFeatures] = d16;    uiNFeatures++;
		adFeatures[uiNFeatures] = d8;     uiNFeatures++;
		adFeatures[uiNFeatures] = dIntra; uiNFeatures++;
		if (bDebug) printf("MB Distribution 4 Classes 2Nx2N: [ %2.2f %2.2f %2.2f %2.2f ].\n", dSKIP, d16, d8, dIntra);
	}

	if (bDebug) printf("-------------  MEAN -------------\n");
	//---------------------------------------
	//Mean
	if (xComputeFeature(TR_H264FEATURE_MEAN))
	{
		Double dSum = 0.0;
		for (UInt ui_k = (xComputeFeature(TR_H264FEATURE_ONE) ? 1 : 0); ui_k < uiNFeatures; ui_k++)
		{
			dSum += adFeatures[ui_k];
		}
		adFeatures[uiNFeatures] = dSum / (Double)((xComputeFeature(TR_H264FEATURE_ONE) ? (uiNFeatures - 1) : uiNFeatures));
		if (bDebug) printf("Mean: %3.0f.\n", adFeatures[uiNFeatures]);
		uiNFeatures++;
	}

	//printf("BREAKPOINT \n");
	return adFeatures;
}

/**
 */
Void TTrEngine::xBuildModelMVVarianceDistance()
{
  //Sets the CUCompressionModes in the low region.
  Bool cCUCompressionModes_Low[CU_DECISION_MAX];
  cCUCompressionModes_Low[CU_DECISION_SPLIT]       = false;
  cCUCompressionModes_Low[CU_DECISION_SKIP]        = true;
  cCUCompressionModes_Low[CU_DECISION_INTER_2Nx2N] = true;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_Low[CU_DECISION_INTRA]       = false;

  //Sets the CUCompressionModes in the High region.
  Bool cCUCompressionModes_High[CU_DECISION_MAX];
  cCUCompressionModes_High[CU_DECISION_SPLIT]       = true;
  cCUCompressionModes_High[CU_DECISION_SKIP]        = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2Nx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_High[CU_DECISION_INTRA]       = false;

  //Iterates to each depth.
  Double dTLow, dTHigh;
  for (UInt uiDepth = 0; uiDepth <= getMaxDepthToComputeFeatures(); uiDepth++)
  {
	//Computes the thresholds.
	dTLow  = m_pcTrainingData->computeThresholdLow(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold() ,cCUCompressionModes_Low,0.9f);
	dTHigh = m_pcTrainingData->computeThresholdHigh(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold(),cCUCompressionModes_High,0.9f);

	printf("Depth : %d - Low: %4.4f - High: %4.4f \n",uiDepth,dTLow, dTHigh);

	//Writes the thresholds.
	m_pcConfig->setMVVar_Min_Threshold(uiDepth,dTLow);
	m_pcConfig->setMVVar_Max_Threshold(uiDepth,dTHigh);
  }
}

/**
 */
Void TTrEngine::xBuildModelMVPhaseVariance()
{
  //Sets the CUCompressionModes in the low region.
  Bool cCUCompressionModes_Low[CU_DECISION_MAX];
  cCUCompressionModes_Low[CU_DECISION_SPLIT]       = false;
  cCUCompressionModes_Low[CU_DECISION_SKIP]        = true;
  cCUCompressionModes_Low[CU_DECISION_INTER_2Nx2N] = true;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_Low[CU_DECISION_INTRA]       = false;

  //Sets the CUCompressionModes in the High region.
  Bool cCUCompressionModes_High[CU_DECISION_MAX];
  cCUCompressionModes_High[CU_DECISION_SPLIT]       = true;
  cCUCompressionModes_High[CU_DECISION_SKIP]        = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2Nx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_High[CU_DECISION_INTRA]       = false;

  //Iterates to each depth.
  Double dTLow, dTHigh;
  for (UInt uiDepth = 0; uiDepth <= getMaxDepthToComputeFeatures(); uiDepth++)
  {
	//Computes the thresholds.
	dTLow  = m_pcTrainingData->computeThresholdLow(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold() ,cCUCompressionModes_Low,0.9f);
	dTHigh = m_pcTrainingData->computeThresholdHigh(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold(),cCUCompressionModes_High,0.9f);

	printf("Depth : %d - Low: %4.4f - High: %4.4f \n",uiDepth,dTLow, dTHigh);

	//Writes the thresholds.
	m_pcConfig->setMVPhaseVar_Min_Threshold(uiDepth,dTLow);
	m_pcConfig->setMVPhaseVar_Max_Threshold(uiDepth,dTHigh);
  }
}

/**
 */
Void TTrEngine::xBuildModelNDCTCoefficients()
{
  //Sets the CUCompressionModes in the low region.
  Bool cCUCompressionModes_Low[CU_DECISION_MAX];
  cCUCompressionModes_Low[CU_DECISION_SPLIT]       = false;
  cCUCompressionModes_Low[CU_DECISION_SKIP]        = true;
  cCUCompressionModes_Low[CU_DECISION_INTER_2Nx2N] = true;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_Low[CU_DECISION_INTRA]       = false;

  //Sets the CUCompressionModes in the High region.
  Bool cCUCompressionModes_High[CU_DECISION_MAX];
  cCUCompressionModes_High[CU_DECISION_SPLIT]       = true;
  cCUCompressionModes_High[CU_DECISION_SKIP]        = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2Nx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_High[CU_DECISION_INTRA]       = false;

  //Iterates to each depth.
  Double dTLow, dTHigh;
  Int    iTLow, iTHigh;
  for (UInt uiDepth = 0; uiDepth <= getMaxDepthToComputeFeatures(); uiDepth++)
  {
	//Computes the thresholds.
	dTLow  = m_pcTrainingData->computeThresholdLow(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold() ,cCUCompressionModes_Low,0.9f);
	dTHigh = m_pcTrainingData->computeThresholdHigh(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold(),cCUCompressionModes_High,0.9f);

	iTLow  = (Int) dTLow;
	iTHigh = (Int) dTHigh;

	printf("Depth : %d - Low: %d - High: %d \n",uiDepth,iTLow, iTHigh);

	//Writes the thresholds.
	m_pcConfig->setNDCT_Min_Threshold(uiDepth,iTLow);
	m_pcConfig->setNDCT_Max_Threshold(uiDepth,iTHigh);
  }
}

/**
 */
Void TTrEngine::xBuildModelEDCTCoefficients()
{
  //Sets the CUCompressionModes in the low region.
  Bool cCUCompressionModes_Low[CU_DECISION_MAX];
  cCUCompressionModes_Low[CU_DECISION_SPLIT]       = false;
  cCUCompressionModes_Low[CU_DECISION_SKIP]        = true;
  cCUCompressionModes_Low[CU_DECISION_INTER_2Nx2N] = true;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_Low[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_Low[CU_DECISION_INTRA]       = false;

  //Sets the CUCompressionModes in the High region.
  Bool cCUCompressionModes_High[CU_DECISION_MAX];
  cCUCompressionModes_High[CU_DECISION_SPLIT]       = true;
  cCUCompressionModes_High[CU_DECISION_SKIP]        = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2Nx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_High[CU_DECISION_INTRA]       = false;

  //Iterates to each depth.
  Double dTLow, dTHigh;
  Int    iTLow, iTHigh;
  for (UInt uiDepth = 0; uiDepth <= getMaxDepthToComputeFeatures(); uiDepth++)
  {
	//Computes the thresholds.
	dTLow  = m_pcTrainingData->computeThresholdLow(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold() ,cCUCompressionModes_Low,0.9f);
	dTHigh = m_pcTrainingData->computeThresholdHigh(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold(),cCUCompressionModes_High,0.9f);

	iTLow  = (Int) dTLow;
	iTHigh = (Int) dTHigh;

	printf("Depth : %d - Low: %d - High: %d \n",uiDepth,iTLow, iTHigh);

	//Writes the thresholds.
	m_pcConfig->setEDCT_Min_Threshold(uiDepth,iTLow);
	m_pcConfig->setEDCT_Max_Threshold(uiDepth,iTHigh);
  }
}

/**
 */
Void TTrEngine::xBuildModelLDF_v1()
{
  //Sets the CUCompressionModes in the High region.
  Bool cCUCompressionModes_High[CU_DECISION_MAX];
  cCUCompressionModes_High[CU_DECISION_SPLIT]       = true;
  cCUCompressionModes_High[CU_DECISION_SKIP]        = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2Nx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_High[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_High[CU_DECISION_INTRA]       = false;

  //Sets the CUCompressionModes for the LDF function.
  Bool cCUCompressionModes_LDF[CU_DECISION_MAX];
  cCUCompressionModes_LDF[CU_DECISION_SPLIT]       = true;
  cCUCompressionModes_LDF[CU_DECISION_SKIP]        = false;
  cCUCompressionModes_LDF[CU_DECISION_INTER_2Nx2N] = false;
  cCUCompressionModes_LDF[CU_DECISION_INTER_2NxN]  = false;
  cCUCompressionModes_LDF[CU_DECISION_INTER_Nx2N]  = false;
  cCUCompressionModes_LDF[CU_DECISION_INTER_2NxnU] = false;
  cCUCompressionModes_LDF[CU_DECISION_INTER_2NxnD] = false;
  cCUCompressionModes_LDF[CU_DECISION_INTER_nLx2N] = false;
  cCUCompressionModes_LDF[CU_DECISION_INTER_nRx2N] = false;
  cCUCompressionModes_LDF[CU_DECISION_INTER_NxN]   = false;
  cCUCompressionModes_LDF[CU_DECISION_INTRA]       = false;

  //Iterates to each depth.
  Double dTHigh;

  for (UInt uiDepth = 0; uiDepth <= getMaxDepthToComputeFeatures(); uiDepth++)
  {
	//Computes the thresholds.
	//dTHigh = m_pcTrainingData->computeThresholdHigh(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold(),cCUCompressionModes_High,0.9f);
	//Stop removing features based solely on the MV Variance DIstance.
	dTHigh = -1;

	//Writes the thresholds.
	m_pcConfig->setMVVar_Max_Threshold(uiDepth,dTHigh);

	//Asks the TTrTrainingData object to compute the LDF model.
	// -> uiDepth
	// -> uiFeature
	// -> MVVarianceDistance Threshold High
	// -> CUCompressionModesclasses.
	Double **dWeights;

	//Bruno, modification, 29/1/2013
	Double *dMeanN = NULL;
    Double *dStdN  = NULL;

	UInt Uik = 2;                                        //Number of classes
	UInt UiM = m_pcTrainingData->getNumberOfFeatures();  //Number of features

	//Allocates the weights.
	dWeights = (Double**) malloc (Uik * sizeof(Double*));
    for (UInt i = 0; i < Uik; i++)
    {
	  dWeights[i] = (Double*) malloc(UiM * sizeof(Double));
	}

	//Allocates the means and variances, if needed.
	if(m_pcConfig->useFeatureStandardization()) //Bruno, modification, 29/1/2013
    {
	  dMeanN = (Double*) malloc(UiM * sizeof(Double));
      dStdN = (Double*) malloc(UiM * sizeof(Double));
    }

	//Before computing the LDF model, adds at least a single feature to the non split class.
	Double *adFeatureForNonSplitClass = xGetFeatureToNonSplitClass_LDF_v1(uiDepth);
	m_pcTrainingData->writeFeatures(uiDepth,m_pcTrainingData->getNumberOfFeatures(),adFeatureForNonSplitClass,CU_DECISION_SKIP);
	free(adFeatureForNonSplitClass);

	m_pcTrainingData->computeModel_LDF_v1(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold(), dTHigh, cCUCompressionModes_LDF, dMeanN, dStdN, m_pcConfig->useFeatureStandardization(), dWeights);

	//Writes the weights to the m_pcConfig;
	m_pcConfig->setWeightsForLDF(uiDepth,0,m_pcTrainingData->getNumberOfFeatures(),dWeights[0]); //Class 0
	m_pcConfig->setWeightsForLDF(uiDepth,1,m_pcTrainingData->getNumberOfFeatures(),dWeights[1]); //Class 1

	//Writes the mean and std to the m_pcConfig;
	if(m_pcConfig->useFeatureStandardization()) //Bruno, modification, 29/1/2013
	{
	  m_pcConfig->setLDFMeansAndVariances(uiDepth, m_pcTrainingData->getNumberOfFeatures(), dMeanN, dStdN);
	}

#ifdef TR_WRITE_TRAINING_DATA_TO_FILE
	//Writes the data to check the model.
	m_pcTrainingData->writeFeaturesToFile_LDF(uiDepth, m_pcConfig->getFeatureToComputeTheDynamicThreshold(), dTHigh, cCUCompressionModes_LDF);
#endif

#ifdef TR_WRITE_MODEL_WEIGHTS_TO_FILE
	//Writes the weights to file.
	m_pcTrainingData->writeWeightsToFile(uiDepth,m_pcTrainingData->getNumberOfFeatures(),dWeights,dMeanN,dStdN);
#endif

	//Frees the weights.
	UInt i;
    for (i = 0; i < 2; i++)
    {
	  free(dWeights[i]);
    }
    free(dWeights);

	free(dMeanN);
	free(dStdN);
  }
}

/**
 * Get a feature for the NON SPLIT class.
 */
Double* TTrEngine::xGetFeatureToNonSplitClass_LDF_v1(UInt uiDepth)
{
  Bool bDebug = false;

  //These are the entities that I want to compute:
  Double *adFeatures;
  UInt uiTotalFeatures;
  UInt uiNFeatures = 0;

  //Allocates the feature array.
  uiTotalFeatures = getNumberOfFeatures();
  adFeatures = (Double*) malloc(uiTotalFeatures * sizeof(Double));

  //The level of the 4x4 block.
  UInt uiLevel_4x4 = getCurrentH264FrameMI()->getTransformInfo()->getLevel_4x4();
  UInt uiSize_4x4 =     1 << (uiLevel_4x4 - uiDepth);

  if (bDebug) printf("-------------  ONE  -------------\n");
  //---------------------------------------
  //One
  if (xComputeFeature(TR_H264FEATURE_ONE))
  {
	adFeatures[uiNFeatures] = 1;
	if (bDebug) printf("One: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  if (bDebug) printf("------------- 2Nx2N -------------\n");
  //---------------------------------------
  //Number of H264 Partitions 2Nx2N.
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_PARTITIONS_2Nx2N))
  {
	adFeatures[uiNFeatures] = (uiSize_4x4 >> 2) * (uiSize_4x4 >> 2);
	if (bDebug) printf("Number of Partitions 2Nx2N: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Distance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))
  {
    adFeatures[uiNFeatures] = 0.0;
	if (bDebug) printf("MV Variance Distance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance X 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_X_2Nx2N))
  {
	adFeatures[uiNFeatures] = 0.0;
	if (bDebug) printf("MV Variance X 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //MV Variance Y 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2Nx2N))
  {
	adFeatures[uiNFeatures] = 0.0;
	if (bDebug) printf("MV Variance Y 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //MV Phase Variance 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2Nx2N))
  {
	adFeatures[uiNFeatures] = 0.0;
	if (bDebug) printf("MV Phase Variance 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //Number of DCT Coefficients 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N))
  {
	adFeatures[uiNFeatures] = 0.0;
	if (bDebug) printf("Number of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  //---------------------------------------
  //Energy of DCT Coefficients 2Nx2N
  if (xComputeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2Nx2N))
  {
	adFeatures[uiNFeatures] = 0.0;
	if (bDebug) printf("Energy of DCT Coefficients 2Nx2N: %2.2f.\n",adFeatures[uiNFeatures]);
    uiNFeatures++;
  }

  if (bDebug) printf("-------------  MEAN -------------\n");
  //---------------------------------------
  //Mean
  if (xComputeFeature(TR_H264FEATURE_MEAN))
  {
	Double dSum = 0.0;
	for (UInt ui_k = (xComputeFeature(TR_H264FEATURE_ONE) ? 1 : 0) ; ui_k < uiNFeatures; ui_k++)
	{
	  dSum += adFeatures[ui_k];
	}
	adFeatures[uiNFeatures] = dSum / (Double) ( (xComputeFeature(TR_H264FEATURE_ONE) ? (uiNFeatures - 1) : uiNFeatures) );
	if (bDebug) printf("Mean: %3.0f.\n",adFeatures[uiNFeatures]);
	uiNFeatures++;
  }
  return adFeatures;
}

/**
 */
Void TTrEngine::xBuildModelLDF_v3()
{
  //Write the set of features to file.
  m_pcTrainingData->writeFeaturesToFile();

  //Then I do not compute any model yet.
}

/**
 */
Void TTrEngine::xBuildModelLDF_v4()
{
#ifdef TR_WRITE_TRAINING_DATA_TO_FILE
  //Write the set of features to file.
  m_pcTrainingData->writeFeaturesToFile();
#endif

  //Computes the Model for all depths in the LDF v4
  m_pcTrainingData->computeModel_LDF_v4();
}

/**
 */
Void TTrEngine::xBuildModelLDF_v5()
{
#ifdef TR_WRITE_TRAINING_DATA_TO_FILE
  //Write the set of features to file.
  m_pcTrainingData->writeFeaturesToFile_LDF_v5();
#endif

  //Computes the model for the LDF v5.
  m_pcTrainingData->computeModel_LDF_v5();

#ifdef TR_WRITE_MODEL_WEIGHTS_TO_FILE
  //Writes the weights to file.
  m_pcTrainingData->writeWeightsToFile_LDF_v5();
#endif
}

/**
*/
Void TTrEngine::xBuildModelLDF_v6()
{
#ifdef TR_WRITE_TRAINING_DATA_TO_FILE
  //Write the set of features to file.
  m_pcTrainingData->writeFeaturesToFile_LDF_v6();
#endif

  //Computes the model for the LDF v5.
  m_pcTrainingData->computeModel_LDF_v6();

#ifdef TR_WRITE_MODEL_WEIGHTS_TO_FILE
  //Writes the weights to file.
  m_pcTrainingData->writeWeightsToFile_LDF_v6();
#endif
}



/**	Filipe - acrescentei isso
*/
Void TTrEngine::xBuildModelPIBIC_v1()
{
#ifdef TR_WRITE_TRAINING_DATA_TO_FILE
	//Write the set of features to file.
	m_pcTrainingData->writeFeaturesToFile_LDF_v6();
#endif

	//Computes the model for the LDF v5.
	m_pcTrainingData->computeModel_LDF_v6();

#ifdef TR_WRITE_MODEL_WEIGHTS_TO_FILE
	//Writes the weights to file.
	m_pcTrainingData->writeWeightsToFile_LDF_v6();
#endif
}



/**
 */
Bool TTrEngine::isEnabledTranscoder()
{
  if (m_pcConfig == NULL) return false;

  if (m_pcConfig->getTranscodingOption() == TR_DISABLED) return false;

  if (isTraining()) return false;

  return true;
}

/**
 */
Bool TTrEngine::hasIntraMBs(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Bool hasIntra;

  //The current coordinates in 4x4 blocks.
  //m_uiMaxLevel is the level for the 8x8 CU.
  UInt uiRow_4x4  = uiRow << (m_uiMaxLevel + 1 - uiDepth);
  UInt uiCol_4x4  = uiCol << (m_uiMaxLevel + 1 - uiDepth);
  UInt uiSize_4x4 =     1 << (m_uiMaxLevel + 1 - uiDepth);

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //Checks if the H.264 partition has intra frames
  hasIntra = pcH264Frame->hasIntra(uiRow_4x4, uiCol_4x4, uiSize_4x4);

  //Returns.
  return hasIntra;
}

/**
 */
Bool TTrEngine::isAllIntraMBs(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Bool hasIntra;

  //The current coordinates in 4x4 blocks.
  //m_uiMaxLevel is the level for the 8x8 CU.
  UInt uiRow_4x4  = uiRow << (m_uiMaxLevel + 1 - uiDepth);
  UInt uiCol_4x4  = uiCol << (m_uiMaxLevel + 1 - uiDepth);
  UInt uiSize_4x4 =     1 << (m_uiMaxLevel + 1 - uiDepth);

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //Checks if the H.264 partition has intra frames
  hasIntra = pcH264Frame->allIntra(uiRow_4x4, uiCol_4x4, uiSize_4x4);

  //Returns.
  return hasIntra;
}

/**
 */
Double TTrEngine::xComputeFeatureNumberOfH264Partitions(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Double dFeature;

  //The current coordinates in 4x4 blocks.
  //m_uiMaxLevel is the level for the 8x8 CU.
  UInt uiRow_4x4  = uiRow << (m_uiMaxLevel + 1 - uiDepth);
  UInt uiCol_4x4  = uiCol << (m_uiMaxLevel + 1 - uiDepth);

  //The current coordinates in 16x16 blocks.
  //m_uiMaxLevel is the level for the 8x8 CU.
  UInt uiRow_16x16 = (uiRow_4x4 >> 2);
  UInt uiCol_16x16 = (uiCol_4x4 >> 2);
  UInt uiLevel_16x16 = m_uiMaxLevel - 1;

  //Get the number of 16x16 MBs at this level.
  UInt uiNMBs = 1 << (uiLevel_16x16 - uiDepth);

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //Compute the Feature.
  dFeature = pcH264Frame->getFrameMBInfo()->computeNumberOfPartitionsOfARegion(uiRow_16x16, uiCol_16x16 , uiNMBs);

  return dFeature;
}

/**
 */
Double* TTrEngine::xComputeFeatureH264MBDistribution(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Double* dFeature;

  //The current coordinates in 4x4 blocks.
  //m_uiMaxLevel is the level for the 8x8 CU.
  UInt uiRow_4x4  = uiRow << (m_uiMaxLevel + 1 - uiDepth);
  UInt uiCol_4x4  = uiCol << (m_uiMaxLevel + 1 - uiDepth);

  //The current coordinates in 16x16 blocks.
  //m_uiMaxLevel is the level for the 8x8 CU.
  UInt uiRow_16x16 = (uiRow_4x4 >> 2);
  UInt uiCol_16x16 = (uiCol_4x4 >> 2);
  UInt uiLevel_16x16 = m_uiMaxLevel - 1;

  //Get the number of 16x16 MBs at this level.
  UInt uiNMBs = 1 << (uiLevel_16x16 - uiDepth);

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //Compute the Feature.
  dFeature = pcH264Frame->getFrameMBInfo()->computeMBDistributionOfARegion(uiRow_16x16, uiCol_16x16 , uiNMBs, uiNMBs);

  return dFeature;
}

/**
 */
Void TTrEngine::xComputeFeatureMBDistribution4Classes(UInt uiDepth, UInt uiRow, UInt uiCol, Double *pdDistributionSKIP, Double *pdDistribution16, Double *pdDistribution8, Double *pdDistributionIntra)
{
  Double *adMBDistribution = NULL;

  //Compute the MB Distribution (9 classes).
  adMBDistribution = xComputeFeatureH264MBDistribution(uiDepth, uiRow, uiCol);

  //Merge it into the four classes.
  *pdDistributionSKIP  = adMBDistribution[TR_H264MB_DIST_SKIP];
  *pdDistribution16    = adMBDistribution[TR_H264MB_DIST_16x16] + adMBDistribution[TR_H264MB_DIST_16x8] + adMBDistribution[TR_H264MB_DIST_8x16];
  *pdDistribution8     = adMBDistribution[TR_H264MB_DIST_8x8] + adMBDistribution[TR_H264MB_DIST_8x4] + adMBDistribution[TR_H264MB_DIST_4x8] + adMBDistribution[TR_H264MB_DIST_4x4];
  *pdDistributionIntra = adMBDistribution[TR_H264MB_DIST_INTRA];

  free(adMBDistribution);
}

/**
 */
Double TTrEngine::xComputeFeatureMVVarianceDistance(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Double dFeature;

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //pcH264Frame->getMVVariance()->printMVVarianceForLevel(uiDepth);

  dFeature = pcH264Frame->getMVVariance()->getVarianceDistance(uiDepth, uiRow, uiCol);

  return dFeature;
}

/**
 */
Double TTrEngine::xComputeFeatureMVVariance_Y(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Double dFeature;

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  dFeature = pcH264Frame->getMVVariance()->getVariance_Y(uiDepth, uiRow, uiCol);

  return dFeature;
}

/**
 */
Double TTrEngine::xComputeFeatureMVVariance_X(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Double dFeature;

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  dFeature = pcH264Frame->getMVVariance()->getVariance_X(uiDepth, uiRow, uiCol);

  return dFeature;
}

/**
 * This function is not optimised.
 */
Void TTrEngine::xComputeFeatureMVVarianceForARegion(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiNRows_4x4, UInt uiNCols_4x4, Double *dRegionVarianceY, Double *dRegionVarianceX, Double *dRegionVarianceDistance)
{
  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //pcH264Frame->getMVVariance()->printMVVarianceForLevel(uiDepth);
  pcH264Frame->getMVVariance()->computeVarianceForARegion(pcH264Frame, uiRow_4x4, uiCol_4x4, uiNRows_4x4, uiNCols_4x4, dRegionVarianceY, dRegionVarianceX, dRegionVarianceDistance);
}

/**
 */
Double TTrEngine::xComputeFeatureMVPhaseVariance(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Double dFeature;

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  dFeature = pcH264Frame->getMVVariance()->getPhaseVariance(uiDepth, uiRow, uiCol);

  return dFeature;
}

/**
*/
Double TTrEngine::xComputeFeatureMVPhaseTrigonometricVariance(UInt uiDepth, UInt uiRow, UInt uiCol)
{
	Double dFeature;

	//A pointer to the frame object.
	TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

	dFeature = pcH264Frame->getMVVariance()->getPhaseTrigonometricVariance(uiDepth, uiRow, uiCol);

	return dFeature;
}

/**
 * This function is not optimised.
 */
Void TTrEngine::xComputeFeatureMVPhaseVarianceForARegion(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiNRows_4x4, UInt uiNCols_4x4, Double *dRegionPhaseVariance)
{
  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  Double dRegionPhaseSinVariance, dRegionPhaseCosVariance; //In this function I do not use these values.

  pcH264Frame->getMVVariance()->computePhaseVarianceForARegion(pcH264Frame, uiRow_4x4, uiCol_4x4, uiNRows_4x4, uiNCols_4x4, dRegionPhaseVariance, &dRegionPhaseSinVariance, &dRegionPhaseCosVariance);
}

/**
 */
Double TTrEngine::xComputeFeatureNDCTCoefficients(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Double dFeature;

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  dFeature = pcH264Frame->getTransformInfo()->getNumberDCTCoeffs(uiDepth, uiRow, uiCol);

  return dFeature;
}

/**
 * This function is not optimised.
 */
Void TTrEngine::xComputeFeatureNDCTCoefficientsForARegion(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiNRows_4x4, UInt uiNCols_4x4, Double *dRegionNDCTCoefficients)
{
  UInt uiFeature;

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  pcH264Frame->getTransformInfo()->computeNumberDCTCoeffsForARegion(uiRow_4x4, uiCol_4x4, uiNRows_4x4, uiNCols_4x4, &uiFeature);

  *dRegionNDCTCoefficients = (Double) uiFeature;
}

/**
 */
Double TTrEngine::xComputeFeatureEnergyDCTCoefficients(UInt uiDepth, UInt uiRow, UInt uiCol)
{
  Double dFeature;

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  dFeature = pcH264Frame->getTransformInfo()->computeAverageEnergyDCTCoeffs(uiDepth, uiRow, uiCol);

  return dFeature;
}

/**
 * This function is not optimised.
 */
Void TTrEngine::xComputeFeatureEnergyDCTCoefficientsForARegion(UInt uiRow_4x4, UInt uiCol_4x4, UInt uiNRows_4x4, UInt uiNCols_4x4, Double *dRegionEDCTCoefficients)
{
  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  pcH264Frame->getTransformInfo()->computeAverageEnergyDCTCoeffsForARegion(uiRow_4x4, uiCol_4x4, uiNRows_4x4, uiNCols_4x4, dRegionEDCTCoefficients);
}

/**
 */
Void TTrEngine::writeFeatures(UInt uiDepth, UInt uiNFeatures, Double *dFeatures, CUDecisionModes cHEVC_Class)
{
  m_pcTrainingData->writeFeatures(uiDepth, uiNFeatures, dFeatures, cHEVC_Class);
}

/**
*/
Void TTrEngine::writeFeatures(UInt uiDepth, UInt uiNFeatures, Double *dFeatures, CUDecisionModes cHEVC_Class, UInt uiHEVCQP)
{
	m_pcTrainingData->writeFeatures(uiDepth, uiNFeatures, dFeatures, cHEVC_Class, uiHEVCQP);
}

/**
 */
Void TTrEngine::writeFeatures(UInt uiDepth, UInt uiNFeatures, Double *dFeatures, CUDecisionModes cHEVC_Class, Double *adHEVCCost, UInt uiHEVCQP)
{
  m_pcTrainingData->writeFeatures(uiDepth, uiNFeatures, dFeatures, cHEVC_Class,adHEVCCost,uiHEVCQP);
}

/**
 */
Void TTrEngine::computeMVCandidates(UInt uiLevel, UInt uiRow_4x4, UInt uiCol_4x4, UInt uiHeight, UInt uiWidth, Int iRefFrame)
{
  //Clears the vector.
  m_vcMVCandidates.clear();

  if ((m_pcH264Frames[m_uiCurrFrame] == NULL) || (m_pcConfig->getMVGroupingMethod() == TR_NONE))
  {
	m_vcMVCandidates.push_back(::TComMv(0,0));
	return;
  }

  //Creates some temporary variables.
  Int mv_y[256];
  Int mv_x[256];
  UInt count[256];
  UInt nMVs = 0;

  //Fill the MV lists
  m_pcH264Frames[m_uiCurrFrame]->getListOfMVs(uiRow_4x4, uiCol_4x4, uiHeight, uiWidth, iRefFrame, mv_y, mv_x, count, &nMVs);

  MVGroupingMethod cMethod = m_pcConfig->getMVGroupingMethod();

  //Now, it returns the MV depending on the method.
  if (nMVs == 0)
  {
    //If it could not find any MVs, I will return (0,0) anyway.
	m_vcMVCandidates.push_back(::TComMv(0,0));
  } else if (cMethod == TR_AVERAGE)
  {
    Int av_mv_y = 0;
	Int av_mv_x = 0;
	Int sum_count = 0;
	UInt k;

	for (k = 0; k < nMVs; k++)
	{
	  av_mv_y   += (mv_y[k] * count[k]);
      av_mv_x   += (mv_x[k] * count[k]);
	  sum_count += count[k];
	}

	av_mv_y = (Int) (av_mv_y / sum_count);
	av_mv_x = (Int) (av_mv_x / sum_count);

	m_vcMVCandidates.push_back(::TComMv(av_mv_x,av_mv_y));
  } else if (cMethod == TR_MODE)
  {
    UInt k ,max_count;

	//Find the max_count.
	max_count = count[0];
	for (k = 1; k < nMVs; k++)
	{
	  if (count[k] > max_count)
	  {
		max_count = count[k];
	  }
	}

	//Add to the list all MVs with count equal to max_count.
	for (k = 0; k < nMVs; k++)
	{
	  if (count[k] == max_count)
	  {
		m_vcMVCandidates.push_back(::TComMv(mv_x[k],mv_y[k]));
	  }
	}
  } else if (cMethod == TR_ALL)
  {
    UInt k;

	for (k = 0; k < nMVs; k++)
	{
	  m_vcMVCandidates.push_back(::TComMv(mv_x[k],mv_y[k]));
	}
  }

  //This is where it decides the current MV Refinement method.
  //It should depend on the number of candidate MVs and on the transcoding method.
  //TODO: make it dependent on the transcoding option (and on other things).
  //Note that I have all information needed to re-compute the variance (ie, the count, the nMVs, the MVs...)
  //if (nMVs == 0)
  //{
  //  printf("Could not find MVs: (row,col) = (%d,%d) - size: (%dx%d) - RF: %d.\n",uiRow_4x4,uiCol_4x4,uiWidth,uiHeight,iRefFrame);
  // m_cCurrentMVRefinementMethod = TR_IGNORE_H264_MV;
  //} else
  //{
    m_cCurrentMVRefinementMethod = getGeneralMVRefinementMethod(uiLevel);
  //}
}

/**
 */
Void TTrEngine::computeMVCandidates(UInt uiLevel, UInt uiRow_4x4, UInt uiCol_4x4, UInt uiHeight, UInt uiWidth, Int iRefFrame, MVGroupingMethod cMethod)
{
  //Clears the vector.
  m_vcMVCandidates.clear();

  if ((m_pcH264Frames[m_uiCurrFrame] == NULL) || (cMethod == TR_NONE))
  {
	m_vcMVCandidates.push_back(::TComMv(0,0));
	return;
  }

  //Creates some temporary variables.
  Int mv_y[256];
  Int mv_x[256];
  UInt count[256];
  UInt nMVs = 0;

  //Fill the MV lists
  m_pcH264Frames[m_uiCurrFrame]->getListOfMVs(uiRow_4x4, uiCol_4x4, uiHeight, uiWidth, iRefFrame, mv_y, mv_x, count, &nMVs);

  //Now, it returns the MV depending on the method.
  if (nMVs == 0)
  {
    //If it could not find any MVs, I will return (0,0) anyway.
	m_vcMVCandidates.push_back(::TComMv(0,0));
  } else if (cMethod == TR_AVERAGE)
  {
    Int av_mv_y = 0;
	Int av_mv_x = 0;
	Int sum_count = 0;
	UInt k;

	for (k = 0; k < nMVs; k++)
	{
	  av_mv_y   += (mv_y[k] * count[k]);
      av_mv_x   += (mv_x[k] * count[k]);
	  sum_count += count[k];
	}

	av_mv_y = (Int) (av_mv_y / sum_count);
	av_mv_x = (Int) (av_mv_x / sum_count);

	m_vcMVCandidates.push_back(::TComMv(av_mv_x,av_mv_y));
  } else if (cMethod == TR_MODE)
  {
    UInt k ,max_count;

	//Find the max_count.
	max_count = count[0];
	for (k = 1; k < nMVs; k++)
	{
	  if (count[k] > max_count)
	  {
		max_count = count[k];
	  }
	}

	//Add to the list all MVs with count equal to max_count.
	for (k = 0; k < nMVs; k++)
	{
	  if (count[k] == max_count)
	  {
		m_vcMVCandidates.push_back(::TComMv(mv_x[k],mv_y[k]));
	  }
	}
  } else if (cMethod == TR_ALL)
  {
    UInt k;

	for (k = 0; k < nMVs; k++)
	{
	  m_vcMVCandidates.push_back(::TComMv(mv_x[k],mv_y[k]));
	}
  }
}

/**
 */
TTrModeDecision* TTrEngine::getTranscoderModeDecision(UInt uiRow, UInt uiCol, UInt uiQP)
{
  //Creates the object modeDecision
  TTrModeDecision *modeDecision = new TTrModeDecision();

  //Checks if the Delphi mode is activated.
  if (m_pcConfig->getDelphiMode() == TR_DELPHI_READ)
  {
    //if it is activated, create the Delphi Decision object.
    TTrModeDecision *delphiDecision = new TTrModeDecision();

	//Reads it from the file.
	Bool bReadOK = m_pcDelphiTranscoder->readCUDecision(uiRow,uiCol,delphiDecision);

	//Calls the setModeDecisionDelphi, to merge both transcoders.
	setModeDecisionDelphi(0, uiRow, uiCol, modeDecision, delphiDecision);

	//Deletes the delphi object.
	delete delphiDecision;
  } else
  {
    //If it is not, then call the recursive function to set the modeDecision.
    setModeDecision(uiQP, 0, uiRow, uiCol, modeDecision);
  }

  //Returns the mode Decision.
  return modeDecision;
}

/**
 */
Void TTrEngine::printH264FrameHeaderToFile()
{
  Char cFilename[80];
  FILE *fileIO;

  //The name of the file.
  sprintf(cFilename,"MotionVectorsH264_%d.txt",m_uiCurrFrame);

  //Opens the file
  fileIO = fopen(cFilename,"w");

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //Effectively prints the frame header on this file.
  pcH264Frame->printH264FrameHeaderOnFile(fileIO);

  //Closes the file.
  fclose(fileIO);
}

/**
 */
Void TTrEngine::printH264LCUToFile(UInt uiRow, UInt uiCol)
{
  Char cFilename[80];
  FILE *fileIO;

  //The name of the file.
  sprintf(cFilename,"MotionVectorsH264_%d.txt",m_uiCurrFrame);

  //Opens the file
  fileIO = fopen(cFilename,"a");

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //Effectively prints the equivalente to this CU in the H.264 on the file.
  pcH264Frame->printH264LCUOnFile(fileIO,uiRow,uiCol,m_uiLCUSize);

  //Closes the file.
  fclose(fileIO);
}


// ====================================================================================================================
// Private member functions
// ====================================================================================================================
/**
 */
Void TTrEngine::setModeDecision(UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision *modeDecision)
{
  Bool bIsSplit = false;

  UInt uiPSize = (m_uiLCUSize >> uiLevel);

  UInt row_px = uiRow * uiPSize;
  UInt col_px = uiCol * uiPSize;

  //Test if this partition is inside the frame boundaries.
  if  (( (row_px + uiPSize -1) <= (m_uiFrameHeight) ) &&  ( (col_px + uiPSize -1) <= (m_uiFrameWidth) ))
  {
    //Set this block as valid.
	modeDecision->setValid(true);

	if (isEnabledTranscoder() == false)
	{
	  bIsSplit = xModeDecision_TrDisabled(uiLevel, uiRow, uiCol, modeDecision);
	} else
	{
	  if ((uiPSize <= 16) && (m_pcConfig->getH264MBTranscodingMethod() != TR_IGNORE_H264MB))
	  {
	    bIsSplit = xModeDecision_FromH264MB(uiLevel, uiRow, uiCol, modeDecision);
	  }
	  else
	  {
        //If it is, then set the decision for this mode.
        switch (m_pcConfig->getTranscodingOption())
        {
          case TR_DISABLED:
	        bIsSplit = xModeDecision_TrDisabled(uiLevel, uiRow, uiCol, modeDecision);
	        break;

  	      case TR_H264MV_REUSE_TRANSCODER:
	        bIsSplit = xModeDecision_TrH264Reuse(uiLevel, uiRow, uiCol, modeDecision);
	        break;

          case TR_MVSIMILARITY:
	        bIsSplit = xModeDecision_TrMVSimilarity(uiLevel, uiRow, uiCol, modeDecision);
	        break;

	      case TR_MVVARIANCE:
	        bIsSplit = xModeDecision_TrMVVariance(uiLevel, uiRow, uiCol, modeDecision);
	        break;

		  case TR_MVPHASEVARIANCE:
            bIsSplit = xModeDecision_TrMVPhaseVariance(uiLevel, uiRow, uiCol, modeDecision);
			break;

          case TR_NDCT_COEFFICIENTS:
	        bIsSplit = xModeDecision_TrNDCT_Coefficients(uiLevel, uiRow, uiCol, modeDecision);
	        break;

		  case TR_EDCT_COEFFICIENTS:
	        bIsSplit = xModeDecision_TrEDCT_Coefficients(uiLevel, uiRow, uiCol, modeDecision);
	        break;

          case TR_MVVARIANCE_COEFFICIENTS:
	        bIsSplit = xModeDecision_TrMVVariance_NDCTCoefficients(uiLevel, uiRow, uiCol, modeDecision);
	        break;

		  case TR_LDF_v1:
		    //TODO
	        bIsSplit = xModeDecision_Tr_LDF_v1(uiLevel, uiRow, uiCol, modeDecision);
	        break;

		  case TR_DEPTH_BASED:
			bIsSplit = xModeDecision_TrDepthBased(uiLevel, uiRow, uiCol, modeDecision);
			break;

		  case TR_LDF_v2:
			//TODO: This mode is disabled, as I am only interested in the features and the model for now.
			bIsSplit = xModeDecision_TrDisabled(uiLevel, uiRow, uiCol, modeDecision);
			//bIsSplit = xModeDecision_Tr_LDF_v2(uiLevel, uiRow, uiCol, modeDecision);
			break;

	      case TR_LDF_v3:
			//TODO: This mode is disabled, as I am only interested in the features and the model for now.
			bIsSplit = xModeDecision_TrDisabled(uiLevel, uiRow, uiCol, modeDecision);
			//bIsSplit = xModeDecision_Tr_LDF_v3(uiLevel, uiRow, uiCol, modeDecision);
			break;

	      case TR_LDF_v4:
			bIsSplit = xModeDecision_Tr_LDF_v4(uiLevel, uiRow, uiCol, modeDecision);
			break;

		  case TR_LDF_v5:
			bIsSplit = xModeDecision_Tr_LDF_v5(uiQP, uiLevel, uiRow, uiCol, modeDecision);
			break;

		  case TR_LDF_v6:
			bIsSplit = xModeDecision_Tr_LDF_v6(uiQP, uiLevel, uiRow, uiCol, modeDecision);
			break;

		  case TR_PIBIC_v1:
			bIsSplit = xModeDecision_Tr_PIBIC_v1(uiQP, uiLevel, uiRow, uiCol, modeDecision);
			break;

		  case TR_PIBIC_v2:
			bIsSplit = xModeDecision_Tr_PIBIC_v2(uiQP, uiLevel, uiRow, uiCol, modeDecision);
			break;

		  case TR_DECISION_TREES_v1:
			bIsSplit = xModeDecision_Tr_DecisionTrees_v1(uiQP, uiLevel, uiRow, uiCol, modeDecision);
			break;

		}
	  }
	}
  } else
  {
    //If this partition is outside the frame boundaries, then I have to split.
    bIsSplit = true;

	//Set this block as NOT valid.
	modeDecision->setValid(false);
  }

  //If the decision is to split AND the maximum level has not been reached, then split.
  if (bIsSplit && (uiLevel < m_uiMaxLevel))
  {
    //Sets the partition to split.
    modeDecision->setSplit(true);

	setModeDecision(uiQP, uiLevel + 1, 2 * uiRow    , 2 * uiCol    , modeDecision->getChild(0));
	setModeDecision(uiQP, uiLevel + 1, 2 * uiRow    , 2 * uiCol + 1, modeDecision->getChild(1));
	setModeDecision(uiQP, uiLevel + 1, 2 * uiRow + 1, 2 * uiCol    , modeDecision->getChild(2));
	setModeDecision(uiQP, uiLevel + 1, 2 * uiRow + 1, 2 * uiCol + 1, modeDecision->getChild(3));
  }
}

/*
 */
Void TTrEngine::setModeDecisionDelphi(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision *modeDecision, TTrModeDecision *delphiDecision)
{
  //THIS FUNCTION ONLY WORKS FOR THE LDF!!

  Bool bIsSplit = false;

  UInt uiPSize = (m_uiLCUSize >> uiLevel);

  UInt row_px = uiRow * uiPSize;
  UInt col_px = uiCol * uiPSize;

  //For now, this SHOULD be the case!!
  if (m_pcConfig->getTranscodingOption() == TR_LDF_v1)
  {
    //Test if this partition is inside the frame boundaries.
    if  (( (row_px + uiPSize -1) <= (m_uiFrameHeight) ) &&  ( (col_px + uiPSize -1) <= (m_uiFrameWidth) ))
    {
      //Set this block as valid.
	  modeDecision->setValid(true);

	  //If this CU is 16x16, then the delphi decision no longer applies.
	  if ((uiPSize <= 16) && (m_pcConfig->getH264MBTranscodingMethod() != TR_IGNORE_H264MB))
	  {
	    bIsSplit = xModeDecision_FromH264MB(uiLevel, uiRow, uiCol, modeDecision);
	  }
	  else
	  {
		 Bool bDelphiIntra = false;
	     Bool bDelphiSplit = false;
	     Bool bDelphiInter = false;

		if (delphiDecision != NULL)
		{
		  //This is the "correct" decision, read from the file.
          bDelphiIntra = (delphiDecision->isTestMode(INTRA_2Nx2N)) ||  (delphiDecision->isTestMode(INTRA_NxN)) || (delphiDecision->isTestMode(INTRA_PCM));
	      bDelphiSplit = delphiDecision->isSplit();
	      bDelphiInter = !bDelphiIntra && !bDelphiSplit;
		}

		//Compute the transcoder prediction for this depth.
		bIsSplit = xModeDecision_Tr_LDF_v1(uiLevel, uiRow, uiCol, modeDecision);

		//Get how the LDF classified this CU.
		Bool bLDFIntra = modeDecision->isTestMode(INTRA_2Nx2N);
		Bool bLDFSplit = bIsSplit;
		Bool bLDFInter = !bLDFIntra && !bLDFSplit;

		//Merge it with the Delphi decision.
		if (bDelphiIntra == true)
		{
		  //Computes the LDF Accuracy.
		  if (bLDFIntra == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 0, 0);
		  } else if (bLDFSplit == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 0, 1);
		  } else if (bLDFInter == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 0, 2);
		  }

		  //If it is INTRA, then the LDF should have classfied it as INTRA as well.
		  bIsSplit = true;
		  modeDecision->setTestModesInBatch(true,true,true,true,false,true);
		} else if (bDelphiSplit == true)
		{
		  //Computes the LDF Accuracy.
		  if (bLDFIntra == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 1, 0);
		  } else if (bLDFSplit == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 1, 1);
		  } else if (bLDFInter == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 1, 2);
		  }

		  //SPLIT
		  bIsSplit = true;
		  modeDecision->setTestModesInBatch(true,false,false,false,false,false);
		} else if (bDelphiInter == true)
		{
		  //Computes the LDF Accuracy.
		  if (bLDFIntra == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 2, 0);
		  } else if (bLDFSplit == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 2, 1);
		  } else if (bLDFInter == true)
		  {
			m_pcDelphiTranscoder->countLDFAccuracy(uiLevel, 2, 2);
		  }

		  //INTER
		  bIsSplit = false;
		  modeDecision->setTestModesInBatch(true,true,true,true,false,false);
		} else
		{
		  //DelphiDecision == NULL!
		  //This will only happen if the INTRA mode
		  //In this case, keep whatever it was decided.
		  modeDecision->setTestMode(SKIP_MERGE,true);
		}

	  }
    } else
    {
      //If this partition is outside the frame boundaries, then I have to split.
	  //If it is outside the boundaries, then the Delph is probably split as well.
      bIsSplit = true;

	  //Set this block as NOT valid.
	  modeDecision->setValid(false);
    }

    //If the decision is to split AND the maximum level has not been reached, then split.
    if (bIsSplit && (uiLevel < m_uiMaxLevel))
    {
      //Sets the partition to split.
      modeDecision->setSplit(true);

	  if ((delphiDecision != NULL) && (delphiDecision->isSplit() == true))
	  {
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol    , modeDecision->getChild(0), delphiDecision->getChild(0));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol + 1, modeDecision->getChild(1), delphiDecision->getChild(1));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol    , modeDecision->getChild(2), delphiDecision->getChild(2));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol + 1, modeDecision->getChild(3), delphiDecision->getChild(3));
	  } else {
		setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol    , modeDecision->getChild(0), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol + 1, modeDecision->getChild(1), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol    , modeDecision->getChild(2), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol + 1, modeDecision->getChild(3), NULL);
	  }
    }
  } else if (m_pcConfig->getTranscodingOption() == TR_LDF_v2)
  {
	//Test if this partition is inside the frame boundaries.
    if  (( (row_px + uiPSize -1) <= (m_uiFrameHeight) ) &&  ( (col_px + uiPSize -1) <= (m_uiFrameWidth) ))
    {
      //Set this block as valid.
	  modeDecision->setValid(true);

	  //If this CU is 16x16, then the delphi decision no longer applies.
	  if ((uiPSize <= 16) && (m_pcConfig->getH264MBTranscodingMethod() != TR_IGNORE_H264MB))
	  {
	    bIsSplit = xModeDecision_FromH264MB(uiLevel, uiRow, uiCol, modeDecision);
	  }
	  else
	  {
		 Bool bDelphiIntra = false;
	     Bool bDelphiSplit = false;
	     Bool bDelphiInter = false;
		 Bool bDelphiSKIP  = false;

		//This is the "correct" decision, read from the file.
        bDelphiIntra = (delphiDecision->isTestMode(INTRA_2Nx2N)) ||  (delphiDecision->isTestMode(INTRA_NxN)) || (delphiDecision->isTestMode(INTRA_PCM));
	    bDelphiSplit = delphiDecision->isSplit();
		bDelphiSKIP  = delphiDecision->isTestMode(SKIP_MERGE);
	    bDelphiInter = !bDelphiIntra && !bDelphiSplit && !bDelphiSKIP;

	    //There is no transcoder prediction yet...
		//Compute the transcoder prediction for this depth.
		//bIsSplit = xModeDecision_Tr_LDF_v1(uiLevel, uiRow, uiCol, modeDecision);

		//Get how the LDF classified this CU.

		//Merge it with the Delphi decision.
		if (bDelphiIntra == true)
		{
		  //INTRA
		  bIsSplit = false;
		  modeDecision->setTestModesInBatch(false,false,false,false,false,true);
		} else if (bDelphiSplit == true)
		{
		  //SPLIT
		  bIsSplit = true;
		  modeDecision->setTestModesInBatch(true,false,false,false,false,false);
		} else if (bDelphiInter == true)
		{
		  //INTER
		  bIsSplit = false;
		  modeDecision->setTestModesInBatch(true,true,true,true,false,false);
		} else if (bDelphiSKIP == true)
		{
		  //SKIP
		  bIsSplit = false;
		  modeDecision->setTestModesInBatch(true,false,false,false,false,false);
		}

	  }
    } else
    {
      //If this partition is outside the frame boundaries, then I have to split.
	  //If it is outside the boundaries, then the Delph is probably split as well.
      bIsSplit = true;

	  //Set this block as NOT valid.
	  modeDecision->setValid(false);
    }

    //If the decision is to split AND the maximum level has not been reached, then split.
    if (bIsSplit && (uiLevel < m_uiMaxLevel))
    {
      //Sets the partition to split.
      modeDecision->setSplit(true);

	  if ((delphiDecision != NULL) && (delphiDecision->isSplit() == true))
	  {
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol    , modeDecision->getChild(0), delphiDecision->getChild(0));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol + 1, modeDecision->getChild(1), delphiDecision->getChild(1));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol    , modeDecision->getChild(2), delphiDecision->getChild(2));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol + 1, modeDecision->getChild(3), delphiDecision->getChild(3));
	  } else {
		setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol    , modeDecision->getChild(0), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol + 1, modeDecision->getChild(1), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol    , modeDecision->getChild(2), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol + 1, modeDecision->getChild(3), NULL);
	  }
    }
  } else if (m_pcConfig->getTranscodingOption() == TR_LDF_v3)
  {
	//Test if this partition is inside the frame boundaries.
    if  (( (row_px + uiPSize -1) <= (m_uiFrameHeight) ) &&  ( (col_px + uiPSize -1) <= (m_uiFrameWidth) ))
    {
      //Set this block as valid.
	  modeDecision->setValid(true);

	  Bool bDelphiIntra = false;
	  Bool bDelphiSplit = false;
	  Bool bDelphiInter = false;
	  Bool bDelphiSKIP  = false;

	  //This is the "correct" decision, read from the file.
	  if (delphiDecision != NULL)
	  {
        bDelphiIntra = (delphiDecision->isTestMode(INTRA_2Nx2N)) ||  (delphiDecision->isTestMode(INTRA_NxN)) || (delphiDecision->isTestMode(INTRA_PCM));
	    bDelphiSplit = delphiDecision->isSplit();
	    bDelphiSKIP  = delphiDecision->isTestMode(SKIP_MERGE);
	    bDelphiInter = !bDelphiIntra && !bDelphiSplit && !bDelphiSKIP;
	  }

	  //There is no transcoder prediction yet...
	  //Compute the transcoder prediction for this depth.
	  //bIsSplit = xModeDecision_Tr_LDF_v1(uiLevel, uiRow, uiCol, modeDecision);

	  //Get how the LDF classified this CU.

	  //Merge it with the Delphi decision.
	  if (bDelphiIntra == true)
	  {
		//INTRA
		bIsSplit = false;
		modeDecision->setTestModesInBatch(false,false,false,false,false,true);
	  } else if (bDelphiSplit == true)
	  {
		//SPLIT
		bIsSplit = true;
		modeDecision->setTestModesInBatch(true,false,false,false,false,false);
	  } else if (bDelphiInter == true)
	  {
		//INTER
		bIsSplit = false;
		modeDecision->setTestModesInBatch(true,true,true,true,false,false);
	  } else if (bDelphiSKIP == true)
	  {
		//SKIP
		bIsSplit = false;
		modeDecision->setTestModesInBatch(true,false,false,false,false,false);
	  } else
	  {
		//ALL
		bIsSplit = true;
		modeDecision->setTestModesInBatch(true,true,true,true,true,true);
	  }

    } else
    {
      //If this partition is outside the frame boundaries, then I have to split.
	  //If it is outside the boundaries, then the Delph is probably split as well.
      bIsSplit = true;

	  //Set this block as NOT valid.
	  modeDecision->setValid(false);
    }

    //If the decision is to split AND the maximum level has not been reached, then split.
    if (bIsSplit && (uiLevel < m_uiMaxLevel))
    {
      //Sets the partition to split.
      modeDecision->setSplit(true);

	  if ((delphiDecision != NULL) && (delphiDecision->isSplit() == true))
	  {
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol    , modeDecision->getChild(0), delphiDecision->getChild(0));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol + 1, modeDecision->getChild(1), delphiDecision->getChild(1));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol    , modeDecision->getChild(2), delphiDecision->getChild(2));
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol + 1, modeDecision->getChild(3), delphiDecision->getChild(3));
	  } else {
		setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol    , modeDecision->getChild(0), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow    , 2 * uiCol + 1, modeDecision->getChild(1), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol    , modeDecision->getChild(2), NULL);
	    setModeDecisionDelphi(uiLevel + 1, 2 * uiRow + 1, 2 * uiCol + 1, modeDecision->getChild(3), NULL);
	  }
    }
  }
}

/**
 */
Bool TTrEngine::xModeDecision_TrDisabled(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //Set the split to true.
  Bool bIsSplit = true;

  //The NxN mode is ONLY tested in the final level.
  Bool bTestInter_NxN = (uiLevel == m_uiMaxLevel);

  //Set all modes to true.
  modeDecision->setTestModesInBatch(true,true,true,true,bTestInter_NxN,true);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_TrH264Reuse(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //Set the split to true.
  Bool bIsSplit = true;

  //The NxN mode is ONLY tested in the final level.
  Bool bTestInter_NxN = (uiLevel == m_uiMaxLevel);

  //Checks if the INTRA mode will be tested.
  UInt uiPSize    = (m_uiLCUSize >> uiLevel);
  UInt uiSize_4x4 = (uiPSize >> 2);
  UInt uiRow_4x4  = ((uiRow * uiPSize) >> 2);
  UInt uiCol_4x4  = ((uiCol * uiPSize) >> 2);
  Bool bTestIntra = getCurrentH264FrameMI()->hasIntra(uiRow_4x4, uiCol_4x4, uiSize_4x4);

  //Set all modes.
  modeDecision->setTestModesInBatch(true,true,true,true,bTestInter_NxN,bTestIntra);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_TrMVSimilarity(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The SKIP and the 2Nx2N modes are always true.
  Bool bTestSkip        = true;
  Bool bTestInter_2Nx2N = true;

  //Check if this partition is similar.
  Bool bIsSimilar = getCurrentH264FrameMI()->getMVSimilarity()->getSimilarity(uiLevel, uiRow, uiCol);

  //If the partition is similar, then the smaller partitions are NOT tested and the split is set to false.
  Bool bTestInter_Nx2N_2NxN = !bIsSimilar;
  Bool bTestInter_nLnRxnUnD = !bIsSimilar;
  Bool bIsSplit             = !bIsSimilar;

  //The NxN mode is ONLY tested in the final level and if the block is not similar.
  Bool bTestInter_NxN = (uiLevel == m_uiMaxLevel) && (!bIsSimilar);

  //Checks if the INTRA mode will be tested.
  UInt uiPSize    = (m_uiLCUSize >> uiLevel);
  UInt uiSize_4x4 = (uiPSize >> 2);
  UInt uiRow_4x4  = ((uiRow * uiPSize) >> 2);
  UInt uiCol_4x4  = ((uiCol * uiPSize) >> 2);
  Bool bTestIntra = getCurrentH264FrameMI()->hasIntra(uiRow_4x4, uiCol_4x4, uiSize_4x4);

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_TrMVVariance(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The SKIP is always tested.
  Bool bTestSkip        = true;

  Bool bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit         = false;

  //Get the MVVariance distance for this partition
  Double dMVVarDistance = getCurrentH264FrameMI()->getMVVariance()->getVarianceDistance(uiLevel, uiRow, uiCol);

  //If the variance is negative, it means that I cannot say anything about it - test everything.
  if (dMVVarDistance < 0)
  {
    bTestInter_2Nx2N     = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = true;
	bIsSplit             = true;
  } else
  {
	if (m_pcConfig->getMVVar_Max_Threshold(uiLevel) < 0) // This means that the max threshold is NOT USED.
	{
	  //If the variance is lower than the min threshold, then I will not split or test smaller partitions.
      if (dMVVarDistance <= m_pcConfig->getMVVar_Min_Threshold(uiLevel))
      {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = false;
	    bTestInter_nLnRxnUnD = false;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = false;
      } else
	  {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = true;
	    bTestInter_nLnRxnUnD = true;
    	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	    bTestIntra           = false;
	    bIsSplit             = true;
	  }
	} else
	{
	  //If the variance is lower than the min threshold, then I will not split or test smaller partitions.
      if (dMVVarDistance <= m_pcConfig->getMVVar_Min_Threshold(uiLevel))  //LOWER THAN THE MIN THRESHOLD
      {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = false;
	    bTestInter_nLnRxnUnD = false;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = false;
      } else if (dMVVarDistance > m_pcConfig->getMVVar_Max_Threshold(uiLevel)) //HIGHER THAN THE MAX THRESHOLD
	  {
	    bTestInter_2Nx2N     = (uiLevel == m_uiMaxLevel); //If this is already the maximum level, then
	    bTestInter_Nx2N_2NxN = (uiLevel == m_uiMaxLevel); //these modes are tested anyway (since it cannot split further).
	    bTestInter_nLnRxnUnD = (uiLevel == m_uiMaxLevel);
    	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	    bTestIntra           = false;
	    bIsSplit             = true;
	  } else //BETWEEN THE MIN AND MAX THRESHOLDS
	  {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = true;
	    bTestInter_nLnRxnUnD = true;
    	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	    bTestIntra           = false;
	    bIsSplit             = true;
	  }
	}
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_TrMVPhaseVariance(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The SKIP is always tested.
  Bool bTestSkip        = true;

  Bool bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit         = false;

  //Get the MVVariance distance for this partition
  Double dMVPhaseVar = getCurrentH264FrameMI()->getMVVariance()->getPhaseVariance(uiLevel, uiRow, uiCol);

  //If the variance is negative, it means that I cannot say anything about it - test everything.
  if (dMVPhaseVar < 0)
  {
    bTestInter_2Nx2N     = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = true;
	bIsSplit             = true;
  } else
  {
	  if (m_pcConfig->getMVPhaseVar_Max_Threshold(uiLevel) < 0) // This means that the max threshold is NOT USED.
	{
	  //If the variance is lower than the min threshold, then I will not split or test smaller partitions.
	  if (dMVPhaseVar <= m_pcConfig->getMVPhaseVar_Min_Threshold(uiLevel))
      {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = false;
	    bTestInter_nLnRxnUnD = false;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = false;
      } else
	  {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = true;
	    bTestInter_nLnRxnUnD = true;
    	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	    bTestIntra           = false;
	    bIsSplit             = true;
	  }
	} else
	{
	  //If the variance is lower than the min threshold, then I will not split or test smaller partitions.
      if (dMVPhaseVar <= m_pcConfig->getMVPhaseVar_Min_Threshold(uiLevel))  //LOWER THAN THE MIN THRESHOLD
      {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = false;
	    bTestInter_nLnRxnUnD = false;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = false;
      } else if (dMVPhaseVar > m_pcConfig->getMVPhaseVar_Max_Threshold(uiLevel)) //HIGHER THAN THE MAX THRESHOLD
	  {
	    bTestInter_2Nx2N     = (uiLevel == m_uiMaxLevel); //If this is already the maximum level, then
	    bTestInter_Nx2N_2NxN = (uiLevel == m_uiMaxLevel); //these modes are tested anyway (since it cannot split further).
	    bTestInter_nLnRxnUnD = (uiLevel == m_uiMaxLevel);
    	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	    bTestIntra           = false;
	    bIsSplit             = true;
	  } else //BETWEEN THE MIN AND MAX THRESHOLDS
	  {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = true;
	    bTestInter_nLnRxnUnD = true;
    	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	    bTestIntra           = false;
	    bIsSplit             = true;
	  }
	}
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_TrNDCT_Coefficients(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The SKIP is always tested.
  Bool bTestSkip        = true;

  Bool bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit         = false;

  //Get the number of dct coefficients for this partition
  Int iNDCTCoefficients = (Int) getCurrentH264FrameMI()->getTransformInfo()->getNumberDCTCoeffs(uiLevel,uiRow,uiCol);

  //If there is an intra block, then I shall test all options.
  //Checks if the INTRA mode will be tested.
  UInt uiPSize    = (m_uiLCUSize >> uiLevel);
  UInt uiSize_4x4 = (uiPSize >> 2);
  UInt uiRow_4x4  = ((uiRow * uiPSize) >> 2);
  UInt uiCol_4x4  = ((uiCol * uiPSize) >> 2);

  Bool isIntra = getCurrentH264FrameMI()->hasIntra(uiRow_4x4, uiCol_4x4, uiSize_4x4);

  if (isIntra)
  {
    bTestInter_2Nx2N     = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = true;
	bIsSplit             = true;
  } else if (iNDCTCoefficients <= m_pcConfig->getNDCT_Min_Threshold(uiLevel))  //LOWER THAN THE MIN THRESHOLD
  {
    bTestInter_2Nx2N     = true;
    bTestInter_Nx2N_2NxN = false;
    bTestInter_nLnRxnUnD = false;
    bTestInter_NxN       = false;
	bTestIntra           = false;
    bIsSplit             = false;
  } else if ((m_pcConfig->getNDCT_Max_Threshold(uiLevel) != -1) && (iNDCTCoefficients > m_pcConfig->getNDCT_Max_Threshold(uiLevel))) //HIGHER THAN THE MAX THRESHOLD
  {
    bTestInter_2Nx2N     = (uiLevel == m_uiMaxLevel); //If this is already the maximum level, then
    bTestInter_Nx2N_2NxN = (uiLevel == m_uiMaxLevel); //these modes are tested anyway (since it cannot split further).
    bTestInter_nLnRxnUnD = (uiLevel == m_uiMaxLevel);
   	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = false;
    bIsSplit             = true;
  } else //BETWEEN THE MIN AND MAX THRESHOLDS
  {
    bTestInter_2Nx2N     = true;
    bTestInter_Nx2N_2NxN = true;
    bTestInter_nLnRxnUnD = true;
   	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = false;
    bIsSplit             = true;
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_TrEDCT_Coefficients(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The SKIP is always tested.
  Bool bTestSkip        = true;

  Bool bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit         = false;

  //Get the number of dct coefficients for this partition
  Int iEDCTCoefficients = (Int) getCurrentH264FrameMI()->getTransformInfo()->getEnergyDCTCoeffs(uiLevel,uiRow,uiCol);

  //If there is an intra block, then I shall test all options.
  //Checks if the INTRA mode will be tested.
  UInt uiPSize    = (m_uiLCUSize >> uiLevel);
  UInt uiSize_4x4 = (uiPSize >> 2);
  UInt uiRow_4x4  = ((uiRow * uiPSize) >> 2);
  UInt uiCol_4x4  = ((uiCol * uiPSize) >> 2);

  Bool isIntra = getCurrentH264FrameMI()->hasIntra(uiRow_4x4, uiCol_4x4, uiSize_4x4);

  if (isIntra)
  {
    bTestInter_2Nx2N     = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = true;
	bIsSplit             = true;
  } else if (iEDCTCoefficients <= m_pcConfig->getEDCT_Min_Threshold(uiLevel))  //LOWER THAN THE MIN THRESHOLD
  {
    bTestInter_2Nx2N     = true;
    bTestInter_Nx2N_2NxN = false;
    bTestInter_nLnRxnUnD = false;
    bTestInter_NxN       = false;
	bTestIntra           = false;
    bIsSplit             = false;
  } else if ((m_pcConfig->getEDCT_Max_Threshold(uiLevel) != -1) && (iEDCTCoefficients > m_pcConfig->getEDCT_Max_Threshold(uiLevel))) //HIGHER THAN THE MAX THRESHOLD
  {
    bTestInter_2Nx2N     = (uiLevel == m_uiMaxLevel); //If this is already the maximum level, then
    bTestInter_Nx2N_2NxN = (uiLevel == m_uiMaxLevel); //these modes are tested anyway (since it cannot split further).
    bTestInter_nLnRxnUnD = (uiLevel == m_uiMaxLevel);
   	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = false;
    bIsSplit             = true;
  } else //BETWEEN THE MIN AND MAX THRESHOLDS
  {
    bTestInter_2Nx2N     = true;
    bTestInter_Nx2N_2NxN = true;
    bTestInter_nLnRxnUnD = true;
   	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = false;
    bIsSplit             = true;
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_TrMVVariance_NDCTCoefficients(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The SKIP is always tested.
  Bool bTestSkip        = true;

  Bool bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit         = false;

  //Get the number of dct coefficients for this partition
  Int   iNDCTCoefficients = (Int) getCurrentH264FrameMI()->getTransformInfo()->getNumberDCTCoeffs(uiLevel,uiRow,uiCol);

  //Get the MVVariance distance for this partition
  Double dMVVarDistance = getCurrentH264FrameMI()->getMVVariance()->getVarianceDistance(uiLevel, uiRow, uiCol);

  //If the variance is negative, it means that I cannot say anything about it - test everything.
  if (dMVVarDistance < 0)
  {
    bTestInter_2Nx2N     = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = true;
	bIsSplit             = true;
  } else
  {
    Bool isLower  = ((dMVVarDistance <= m_pcConfig->getMVVar_Min_Threshold(uiLevel)) && (iNDCTCoefficients <= m_pcConfig->getNDCT_Min_Threshold(uiLevel)));

	Bool isLarger;

	Double dMVVar_Max_T = m_pcConfig->getMVVar_Max_Threshold(uiLevel);
	Int    iNDCT_Max_T  = m_pcConfig->getNDCT_Max_Threshold(uiLevel);

	Bool   hasMaxTMVVar = (dMVVar_Max_T >= 0);
	Bool   hasMaxTNDCT  = (iNDCT_Max_T  >= 0);

	isLarger = (hasMaxTMVVar && hasMaxTNDCT && (dMVVarDistance > dMVVar_Max_T) && (iNDCTCoefficients > iNDCT_Max_T));
	isLarger = isLarger || (hasMaxTMVVar && !hasMaxTNDCT && (dMVVarDistance > dMVVar_Max_T));
    isLarger = isLarger || (!hasMaxTMVVar && hasMaxTNDCT && (iNDCTCoefficients > iNDCT_Max_T));

	//If the variance is lower than the min threshold, then I will not split or test smaller partitions.
    if (isLower)  //LOWER THAN THE MIN THRESHOLD
    {
	  bTestInter_2Nx2N     = true;
	  bTestInter_Nx2N_2NxN = false;
	  bTestInter_nLnRxnUnD = false;
	  bTestInter_NxN       = false;
	  bTestIntra           = false;
	  bIsSplit             = false;
    } else if (isLarger) //HIGHER THAN THE MAX THRESHOLD
	{
	  bTestInter_2Nx2N     = (uiLevel == m_uiMaxLevel); //If this is already the maximum level, then
	  bTestInter_Nx2N_2NxN = (uiLevel == m_uiMaxLevel); //these modes are tested anyway (since it cannot split further).
	  bTestInter_nLnRxnUnD = (uiLevel == m_uiMaxLevel);
      bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	  bTestIntra           = false;
	  bIsSplit             = true;
	} else //BETWEEN THE MIN AND MAX THRESHOLDS
	{
	  bTestInter_2Nx2N     = true;
	  bTestInter_Nx2N_2NxN = true;
	  bTestInter_nLnRxnUnD = true;
      bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	  bTestIntra           = false;
	  bIsSplit             = true;
	}
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_Tr_LDF_v1(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The weights for the discriminant functions for each class/depth.
  Double* dWeightsClass1;
  Double* dWeightsClass2;

  UInt uiNumberOfFeatures = getNumberOfFeatures();

  Double dValueClass1, dValueClass2;

  Double *dFeatures = NULL;

  //In this case, the CUs with variance higher than this are automatically split.
  Double dMVVarianceThreshold_forSPLIT = m_pcConfig->getMVVar_Max_Threshold(uiLevel);

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //Get the current frame number.
  UInt uiCurrFrame = pcH264Frame->getFrameNumber();

  //The SKIP is always tested.
  Bool bTestSkip        = true;
  Bool bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit         = false;

  Double dMVVarDistance = pcH264Frame->getMVVariance()->getVarianceDistance(uiLevel, uiRow, uiCol);

  //The level of the 4x4 block.
  UInt uiLevel_4x4 = pcH264Frame->getTransformInfo()->getLevel_4x4();

  //The current coordinates in 4x4 blocks.
  UInt uiRow_4x4  = uiRow << (uiLevel_4x4 - uiLevel);
  UInt uiCol_4x4  = uiCol << (uiLevel_4x4 - uiLevel);
  UInt uiSize_4x4 =     1 << (uiLevel_4x4 - uiLevel);

  //Checks if there is a intra block in this CU.
  Bool isIntra = pcH264Frame->hasIntra(uiRow_4x4, uiCol_4x4, uiSize_4x4);

  //If there is an intra block, then I shall test all options.
  if (isIntra)
  {
    bTestInter_2Nx2N     = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	bTestIntra           = true;
	bIsSplit             = true;
  } else //Otherwise, I will use the discriminative functions.
  {
    if ((dMVVarianceThreshold_forSPLIT != -1) && (dMVVarDistance > dMVVarianceThreshold_forSPLIT))
	{
	  //MVVarianceDistance higher than the threshold, SPLIT!.
	  bTestInter_2Nx2N     = false;
	  bTestInter_Nx2N_2NxN = false;
	  bTestInter_nLnRxnUnD = false;
	  bTestInter_NxN       = false;
	  bTestIntra           = false;
	  bIsSplit             = true;
	} else
	{
	  //Computes the features.
      dFeatures = computeFeaturesLDF_v1(uiLevel, uiRow, uiCol);

      //Get the weights.
	  dWeightsClass1 = m_pcConfig->getWeigthsForLDF(uiLevel,0);
	  dWeightsClass2 = m_pcConfig->getWeigthsForLDF(uiLevel,1);

	  if(m_pcConfig->useFeatureStandardization()) //Bruno, modification, 29/1/13
	  {
		UInt uiFeature;
		Double *dMeanN;
        Double *dStdN;

		dMeanN = m_pcConfig->getMeanForLDF(uiLevel);
		dStdN  = m_pcConfig->getStdForLDF(uiLevel);

		for (uiFeature= 0; uiFeature < uiNumberOfFeatures ; uiFeature++)
		{
		  //EDUARDO -> avoid division by zero.
		  dFeatures[uiFeature] = ((dStdN[uiFeature] != 0.0) ? ((dFeatures[uiFeature] - dMeanN[uiFeature])/dStdN[uiFeature]) : dFeatures[uiFeature]);
		}
	  }


	  //Compute the value for the 2 classes.
	  dValueClass1 = xComputeDotProduct(dFeatures, dWeightsClass1, uiNumberOfFeatures);
	  dValueClass2 = xComputeDotProduct(dFeatures, dWeightsClass2, uiNumberOfFeatures);

	  //Classes:
	  //Class1: SPLIT
	  //Class2: NOT SPLIT

	  //DEBUG.
	  /*
	  printf("Row,Col = (%d,%d).\n",uiRow,uiCol);
	  printf("=========================\n");
	  printf("CLASS 1\n");
	  printf("Features | Weights \n");
	  for (UInt ui_k = 0; ui_k < uiNumberOfFeatures; ui_k++)
	  {
	    printf("%5.2f | %5.10f \n",dFeatures[ui_k],dWeightsClass1[ui_k]);
	  }
	  printf("Dot Product:  --> %5.10f.\n",dValueClass1);
	  printf("-------------------------\n");
	  printf("CLASS 2\n");
	  printf("Features | Weights \n");
	  for (UInt ui_k = 0; ui_k < uiNumberOfFeatures; ui_k++)
	  {
	    printf("%5.2f | %5.10f \n",dFeatures[ui_k],dWeightsClass2[ui_k]);
	  }
	  printf("Dot Product:  --> %5.10f.\n",dValueClass2);
	  printf("BREAKPOINT.\n");
	  */

	  //Make the decision.
	  if (dValueClass1 > dValueClass2)
	  {
	    //Class 1 wins, SPLIT!.
	    bTestInter_2Nx2N     = false;
	    bTestInter_Nx2N_2NxN = false;
	    bTestInter_nLnRxnUnD = false;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = true;
#if TR_DEBUG_WRITE_DECISION_PROFILE
		//Writes the decision profile.
		if (m_pcTrainingData != NULL) m_pcTrainingData->addDecisionProfile_LDF_v1(uiLevel,0);
#endif
	  } else if (dValueClass2 > dValueClass1)
	  {
	    //Class 2 wins, 2Nx2N or SKIP.
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = true;
	    bTestInter_nLnRxnUnD = true;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = false;
#if TR_DEBUG_WRITE_DECISION_PROFILE
		//Writes the decision profile.
		if (m_pcTrainingData != NULL) m_pcTrainingData->addDecisionProfile_LDF_v1(uiLevel,1);
#endif
  	  } else
	  {
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = true;
	    bTestInter_nLnRxnUnD = true;
	    bTestInter_NxN       = (uiLevel == m_uiMaxLevel);
	    bTestIntra           = true;
	    bIsSplit             = true;
	  }
	}
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  //Free the features.
  free(dFeatures);

  //DELPHI Transcoder
  //If this function was called and the delphi transcoder is set to WRITE
  //then I need to test all options for depths 0 and 1 (for depth 2, this function is never called).
  if (m_pcConfig->getDelphiMode() == TR_DELPHI_WRITE)
  {
	//Set all modes.
    modeDecision->setTestModesInBatch(true,true,true,true,(uiLevel == m_uiMaxLevel),bTestIntra);
	bIsSplit = true;
  }

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_Tr_LDF_v2(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //TODO: WRITE THIS FUNCTION
  printf("WARNING: WRITE THIS FUNCTION: xModeDecision_Tr_LDF_v2() \n");
  return true;
}

/**
 */
Bool TTrEngine::xModeDecision_Tr_LDF_v4(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The possible modes
  Bool bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit         = false;

  Double *dFeatures = NULL;

  //Computes the features.
  dFeatures = computeFeaturesLDF_v4(uiLevel, uiRow, uiCol);

  //Checks if there is intra MBs here.
  Bool isIntra = ((computeFeaturesForIntraMBs()) ? isAllIntraMBs(uiLevel, uiRow, uiCol) : hasIntraMBs(uiLevel, uiRow, uiCol));

  /*
  printf("================\n");
  printf("ALL original Features: \n");
  for (UInt k = 0; k < m_pcTrainingData->getNumberOfFeatures() ; k++)
  {
	printf("Feature[%2d] = %3.2f \n",k,dFeatures[k]);
  }
  printf("================\n");
  */

  //Checks if it is intra
  if (isIntra == false)
  {
    //Then Classify the feature.
    UInt uiClassification;

    //This is the correct new code
    //STAGE 0: SKIP (0) - NOT SKIP (1)
    uiClassification = m_pcTrainingData->classifyFeature_LDF_v4(0,uiLevel,dFeatures);
    //printf("STAGE %d - Classification = %d .\n",0,uiClassification);

    if (uiClassification == 0)
    {
	  bTestSkip            = true;
	  bTestInter_2Nx2N     = false;
	  bTestInter_Nx2N_2NxN = false;
  	  bTestInter_nLnRxnUnD = false;
	  bTestInter_NxN       = false;
	  bTestIntra           = false;
	  bIsSplit             = false;
#if TR_DEBUG_WRITE_DECISION_PROFILE
	  //Writes the decision profile.
	  m_pcTrainingData->addDecisionProfile_LDF_v4(uiLevel,1);
#endif
    } else
    {
	  //STAGE 1: SPLIT (0) - NOT SPLIT (1)
	  uiClassification = m_pcTrainingData->classifyFeature_LDF_v4(1,uiLevel,dFeatures);
	  //printf("STAGE %d - Classification = %d .\n",1,uiClassification);

	  if (uiClassification == 0)
	  {
	    bTestSkip            = false;
	    bTestInter_2Nx2N     = false;
	    bTestInter_Nx2N_2NxN = false;
	    bTestInter_nLnRxnUnD = false;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = true;
#if TR_DEBUG_WRITE_DECISION_PROFILE
		//Writes the decision profile.
		m_pcTrainingData->addDecisionProfile_LDF_v4(uiLevel,0);
#endif
	  } else
	  {
	    //STAGE 2: INTER (0) - INTRA (1)
	    uiClassification = m_pcTrainingData->classifyFeature_LDF_v4(2,uiLevel,dFeatures);
	    //printf("STAGE %d - Classification = %d .\n",2,uiClassification);

	    if (uiClassification == 0)
	    {
		  bTestSkip            = false;
	      bTestInter_2Nx2N     = true;
	      bTestInter_Nx2N_2NxN = true;
	      bTestInter_nLnRxnUnD = true;
	      bTestInter_NxN       = false;
	      bTestIntra           = false;
	      bIsSplit             = false;
#if TR_DEBUG_WRITE_DECISION_PROFILE
		  //Writes the decision profile.
		  m_pcTrainingData->addDecisionProfile_LDF_v4(uiLevel,2);
#endif
	    } else
	    {
		  //STAGE 3: SPLIT (0) - SKIP (1) - INTER (2) - INTRA (3)
	      uiClassification = m_pcTrainingData->classifyFeature_LDF_v4(3,uiLevel,dFeatures);
		  //printf("STAGE %d - Classification = %d .\n",3,uiClassification);

		  switch (uiClassification)
		  {
		    case 0:
			  bTestSkip            = false;
	          bTestInter_2Nx2N     = false;
	          bTestInter_Nx2N_2NxN = false;
	          bTestInter_nLnRxnUnD = false;
	          bTestInter_NxN       = false;
	          bTestIntra           = false;
	          bIsSplit             = true;
#if TR_DEBUG_WRITE_DECISION_PROFILE
		      //Writes the decision profile.
		      m_pcTrainingData->addDecisionProfile_LDF_v4(uiLevel,0);
#endif
			  break;

		    case 1:
			  bTestSkip            = true;
	          bTestInter_2Nx2N     = false;
	          bTestInter_Nx2N_2NxN = false;
	          bTestInter_nLnRxnUnD = false;
	          bTestInter_NxN       = false;
	          bTestIntra           = false;
	          bIsSplit             = false;
#if TR_DEBUG_WRITE_DECISION_PROFILE
		      //Writes the decision profile.
		      m_pcTrainingData->addDecisionProfile_LDF_v4(uiLevel,1);
#endif
			  break;

		    case 2:
			  bTestSkip            = false;
	          bTestInter_2Nx2N     = true;
	          bTestInter_Nx2N_2NxN = true;
	          bTestInter_nLnRxnUnD = true;
	          bTestInter_NxN       = false;
	          bTestIntra           = false;
	          bIsSplit             = false;
#if TR_DEBUG_WRITE_DECISION_PROFILE
		      //Writes the decision profile.
		      m_pcTrainingData->addDecisionProfile_LDF_v4(uiLevel,2);
#endif
			  break;

		    case 3:
			  bTestSkip            = false;
	          bTestInter_2Nx2N     = false;
	          bTestInter_Nx2N_2NxN = false;
	          bTestInter_nLnRxnUnD = false;
	          bTestInter_NxN       = false;
	          bTestIntra           = true;
	          bIsSplit             = false;
#if TR_DEBUG_WRITE_DECISION_PROFILE
		      //Writes the decision profile.
		      m_pcTrainingData->addDecisionProfile_LDF_v4(uiLevel,3);
#endif
			  break;

		    default:
			  //This should never happen!!
			  bTestSkip            = true;
	          bTestInter_2Nx2N     = true;
	          bTestInter_Nx2N_2NxN = true;
	          bTestInter_nLnRxnUnD = true;
	          bTestInter_NxN       = false;
	          bTestIntra           = true;
	          bIsSplit             = true;
			  break;

		  }
	    }
	  }
    }
  } else
  {
	bTestSkip            = true; //Maybe I do not need to test all this. Not sure.
	bTestInter_2Nx2N     = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN       = false;
	bTestIntra           = true;
	bIsSplit             = true;
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  //Free the features.
  free(dFeatures);

  /*
  printf("Mode Decision: \n");
  printf("bTestSkip            = %s \n",bTestSkip ? "true" : "false");
  printf("bTestInter_2Nx2N     = %s \n",bTestInter_2Nx2N ? "true" : "false");
  printf("bTestInter_Nx2N_2NxN = %s \n",bTestInter_Nx2N_2NxN ? "true" : "false");
  printf("bTestInter_nLnRxnUnD = %s \n",bTestInter_nLnRxnUnD ? "true" : "false");
  printf("bTestInter_NxN       = %s \n",bTestInter_NxN ? "true" : "false");
  printf("bTestIntra           = %s \n",bTestIntra ? "true" : "false");
  printf("bIsSplit             = %s \n",bIsSplit ? "true" : "false");
  printf("BREAKPOINT.\n");
  */

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_Tr_LDF_v5(UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The possible modes
  Bool bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit         = false;

  Double *dFeatures = NULL;

  //Computes the features.
  dFeatures = computeFeaturesLDF_v5(uiLevel, uiRow, uiCol);

  //Checks if there is intra MBs here.
  Bool isIntra = ((computeFeaturesForIntraMBs()) ? isAllIntraMBs(uiLevel, uiRow, uiCol) : hasIntraMBs(uiLevel, uiRow, uiCol));

  /*
  printf("================\n");
  printf("ALL original Features: \n");
  for (UInt k = 0; k < m_pcTrainingData->getNumberOfFeatures() ; k++)
  {
	printf("Feature[%2d] = %3.2f \n",k,dFeatures[k]);
  }
  printf("================\n");
  */

  //Checks if it is intra
  if (isIntra == false)
  {
    //Then Classify the feature.
    UInt uiClassification;

    //STAGE 0: SPLIT (0) - NOT SPLIT (1)
    uiClassification = m_pcTrainingData->classifyFeature_LDF_v5(0, uiLevel, uiQP, dFeatures);
    //printf("STAGE %d - Classification = %d .\n",0,uiClassification);
    if (uiClassification == 0)
    {
	  bTestSkip            = false;
	  bTestInter_2Nx2N     = false;
	  bTestInter_Nx2N_2NxN = false;
	  bTestInter_nLnRxnUnD = false;
	  bTestInter_NxN       = false;
	  bTestIntra           = false;
	  bIsSplit             = true;
#if TR_DEBUG_WRITE_DECISION_PROFILE
	  //Writes the decision profile.
	  m_pcTrainingData->addDecisionProfile_LDF_v5(uiLevel, uiQP, 0);
#endif
    } else
    {
	  //STAGE 1: SKIP (0) - NOT SKIP (1)
	  uiClassification = m_pcTrainingData->classifyFeature_LDF_v5(1, uiLevel, uiQP, dFeatures);
  	  //printf("STAGE %d - Classification = %d .\n",1,uiClassification);

#if TR_DEBUG_WRITE_DECISION_PROFILE
	  //Writes the decision profile.
	  m_pcTrainingData->addDecisionProfile_LDF_v5(uiLevel, uiQP, 1);
#endif
	  if (uiClassification == 0)
	  {
	    bTestSkip            = true;
	    bTestInter_2Nx2N     = false;
	    bTestInter_Nx2N_2NxN = false;
	    bTestInter_nLnRxnUnD = false;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = false;
	  } else
	  {
	    bTestSkip            = false;
	    bTestInter_2Nx2N     = true;
	    bTestInter_Nx2N_2NxN = true;
	    bTestInter_nLnRxnUnD = true;
	    bTestInter_NxN       = false;
	    bTestIntra           = false;
	    bIsSplit             = false;
	  }
    }
  } else
  {
	bTestSkip            = true; //Maybe I do not need to test all this. Not sure.
	bTestInter_2Nx2N     = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN       = false;
	bTestIntra           = true;
	bIsSplit             = true;
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip,bTestInter_2Nx2N,bTestInter_Nx2N_2NxN,bTestInter_nLnRxnUnD,bTestInter_NxN,bTestIntra);

  //Free the features.
  free(dFeatures);

  /*
  printf("Mode Decision: \n");
  printf("bTestSkip            = %s \n",bTestSkip ? "true" : "false");
  printf("bTestInter_2Nx2N     = %s \n",bTestInter_2Nx2N ? "true" : "false");
  printf("bTestInter_Nx2N_2NxN = %s \n",bTestInter_Nx2N_2NxN ? "true" : "false");
  printf("bTestInter_nLnRxnUnD = %s \n",bTestInter_nLnRxnUnD ? "true" : "false");
  printf("bTestInter_NxN       = %s \n",bTestInter_NxN ? "true" : "false");
  printf("bTestIntra           = %s \n",bTestIntra ? "true" : "false");
  printf("bIsSplit             = %s \n",bIsSplit ? "true" : "false");
  printf("BREAKPOINT.\n");
  */

  return bIsSplit;
}

/**
*/
Bool TTrEngine::xModeDecision_Tr_LDF_v6(UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  //The possible modes
  Bool bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
  Bool bIsSplit = false;

  Double *dFeatures = NULL;

  //Computes the features.
  //It uses the same features as v6.
  dFeatures = computeFeaturesLDF_v6(uiLevel, uiRow, uiCol);

  //Checks if there is intra MBs here.
  Bool isIntra = ((computeFeaturesForIntraMBs()) ? isAllIntraMBs(uiLevel, uiRow, uiCol) : hasIntraMBs(uiLevel, uiRow, uiCol));

  /*
  printf("================\n");
  printf("ALL original Features: \n");
  for (UInt k = 0; k < m_pcTrainingData->getNumberOfFeatures() ; k++)
  {
  printf("Feature[%2d] = %3.2f \n",k,dFeatures[k]);
  }
  printf("================\n");
  */

  //Checks if it is intra
  if (isIntra == false)
  {
	//Then Classify the feature.
	UInt uiClassification;

	//STAGE 0: SPLIT (0) - NOT SPLIT (1)
	uiClassification = m_pcTrainingData->classifyFeature_LDF_v6(0, uiLevel, uiQP, dFeatures);
	//printf("STAGE %d - Classification = %d .\n",0,uiClassification);
	if (uiClassification == 0)
	{
	  bTestSkip = false;
	  bTestInter_2Nx2N = false;
	  bTestInter_Nx2N_2NxN = false;
	  bTestInter_nLnRxnUnD = false;
	  bTestInter_NxN = false;
	  bTestIntra = false;
  	  bIsSplit = true;
#if TR_DEBUG_WRITE_DECISION_PROFILE
 	  //Writes the decision profile.
	  m_pcTrainingData->addDecisionProfile_LDF_v5(uiLevel, uiQP, 0);
#endif
	}
	else
	{
	  //STAGE 1: SKIP (0) - NOT SKIP (1)
	  //This second stage is NOT being used!
	  uiClassification = m_pcTrainingData->classifyFeature_LDF_v6(1, uiLevel, uiQP, dFeatures);
	  //printf("STAGE %d - Classification = %d .\n",1,uiClassification);

#if TR_DEBUG_WRITE_DECISION_PROFILE
	  //Writes the decision profile.
	  m_pcTrainingData->addDecisionProfile_LDF_v5(uiLevel, uiQP, 1);
#endif
 	  if (uiClassification == 0)
  	  {
		bTestSkip = true;
		bTestInter_2Nx2N = true;
		bTestInter_Nx2N_2NxN = true;
		bTestInter_nLnRxnUnD = true;
		bTestInter_NxN = false;
		bTestIntra = false;
		bIsSplit = false;
	  }
	  else
  	  {
		bTestSkip = true;
		bTestInter_2Nx2N = true;
		bTestInter_Nx2N_2NxN = true;
		bTestInter_nLnRxnUnD = true;
		bTestInter_NxN = false;
		bTestIntra = false;
		bIsSplit = false;
	  }
	}
  }
  else
  {
	bTestSkip = true; //Maybe I do not need to test all this. Not sure.
	bTestInter_2Nx2N = true;
	bTestInter_Nx2N_2NxN = true;
	bTestInter_nLnRxnUnD = true;
	bTestInter_NxN = false;
	bTestIntra = true;
	bIsSplit = true;
  }

  //Set all modes.
  modeDecision->setTestModesInBatch(bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra);

  //Free the features.
  free(dFeatures);

  /*
  printf("Mode Decision: \n");
  printf("bTestSkip            = %s \n",bTestSkip ? "true" : "false");
  printf("bTestInter_2Nx2N     = %s \n",bTestInter_2Nx2N ? "true" : "false");
  printf("bTestInter_Nx2N_2NxN = %s \n",bTestInter_Nx2N_2NxN ? "true" : "false");
  printf("bTestInter_nLnRxnUnD = %s \n",bTestInter_nLnRxnUnD ? "true" : "false");
  printf("bTestInter_NxN       = %s \n",bTestInter_NxN ? "true" : "false");
  printf("bTestIntra           = %s \n",bTestIntra ? "true" : "false");
  printf("bIsSplit             = %s \n",bIsSplit ? "true" : "false");
  printf("BREAKPOINT.\n");
  */

  return bIsSplit;
}

/**
*/
Bool TTrEngine::xModeDecision_Tr_PIBIC_v1(UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
	//The possible modes
	Bool bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
	Bool bIsSplit = false;

	Double *dFeatures = NULL;

	//Computes the features.
	//It uses the same features as PIBIC_v1.
	dFeatures = computeFeaturesPIBIC_v1(uiLevel, uiRow, uiCol);

	//Checks if there is intra MBs here.
	Bool isIntra = ((computeFeaturesForIntraMBs()) ? isAllIntraMBs(uiLevel, uiRow, uiCol) : hasIntraMBs(uiLevel, uiRow, uiCol));

	/*
	printf("================\n");
	printf("ALL original Features: \n");
	for (UInt k = 0; k < m_pcTrainingData->getNumberOfFeatures() ; k++)
	{
	printf("Feature[%2d] = %3.2f \n",k,dFeatures[k]);
	}
	printf("================\n");
	*/

	//Checks if it is intra
	if (isIntra == false)
	{
		//Then Classify the feature.
		UInt uiClassification;

		//STAGE 0: SPLIT (0) - NOT SPLIT (1)
		uiClassification = m_pcTrainingData->classifyFeature_LDF_v6(0, uiLevel, uiQP, dFeatures);
		//printf("STAGE %d - Classification = %d .\n",0,uiClassification);
		if (uiClassification == 0)
		{
			bTestSkip = false;
			bTestInter_2Nx2N = false;
			bTestInter_Nx2N_2NxN = false;
			bTestInter_nLnRxnUnD = false;
			bTestInter_NxN = false;
			bTestIntra = false;
			bIsSplit = true;
#if TR_DEBUG_WRITE_DECISION_PROFILE
			//Writes the decision profile.
			m_pcTrainingData->addDecisionProfile_LDF_v5(uiLevel, uiQP, 0);
#endif
		}
		else
		{
			bTestSkip = true;
			bTestInter_2Nx2N = true;
			bTestInter_Nx2N_2NxN = true;
			bTestInter_nLnRxnUnD = true;
			bTestInter_NxN = false;
			bTestIntra = false;
			bIsSplit = false;
		}
	}
	else
	{
		bTestSkip = true; //Maybe I do not need to test all this. Not sure.
		bTestInter_2Nx2N = true;
		bTestInter_Nx2N_2NxN = true;
		bTestInter_nLnRxnUnD = true;
		bTestInter_NxN = false;
		bTestIntra = true;
		bIsSplit = true;
	}

	//Set all modes.
	modeDecision->setTestModesInBatch(bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra);

	//Free the features.
	free(dFeatures);

	/*
	printf("Mode Decision: \n");
	printf("bTestSkip            = %s \n",bTestSkip ? "true" : "false");
	printf("bTestInter_2Nx2N     = %s \n",bTestInter_2Nx2N ? "true" : "false");
	printf("bTestInter_Nx2N_2NxN = %s \n",bTestInter_Nx2N_2NxN ? "true" : "false");
	printf("bTestInter_nLnRxnUnD = %s \n",bTestInter_nLnRxnUnD ? "true" : "false");
	printf("bTestInter_NxN       = %s \n",bTestInter_NxN ? "true" : "false");
	printf("bTestIntra           = %s \n",bTestIntra ? "true" : "false");
	printf("bIsSplit             = %s \n",bIsSplit ? "true" : "false");
	printf("BREAKPOINT.\n");
	*/

	return bIsSplit;
}

/**
*/
Bool TTrEngine::xModeDecision_Tr_PIBIC_v2(UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
	//The possible modes
	Bool bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
	Bool bIsSplit = false;

	Double *dFeatures = NULL;

	//Computes the features.
	//It uses the same features as PIBIC_v2.
	dFeatures = computeFeaturesPIBIC_v2(uiLevel, uiRow, uiCol);

	//Checks if there is intra MBs here.
	Bool isIntra = ((computeFeaturesForIntraMBs()) ? isAllIntraMBs(uiLevel, uiRow, uiCol) : hasIntraMBs(uiLevel, uiRow, uiCol));

	/*
	printf("================\n");
	printf("ALL original Features: \n");
	for (UInt k = 0; k < m_pcTrainingData->getNumberOfFeatures() ; k++)
	{
	printf("Feature[%2d] = %3.2f \n",k,dFeatures[k]);
	}
	printf("================\n");
	*/

	//Checks if it is intra
	if (isIntra == false)
	{
		//Then Classify the feature.
		UInt uiClassification;

		//STAGE 0: SPLIT (0) - NOT SPLIT (1)
		uiClassification = m_pcTrainingData->classifyFeature_LDF_v6(0, uiLevel, uiQP, dFeatures);
		//printf("STAGE %d - Classification = %d .\n",0,uiClassification);
		if (uiClassification == 0)
		{
			bTestSkip = false;
			bTestInter_2Nx2N = false;
			bTestInter_Nx2N_2NxN = false;
			bTestInter_nLnRxnUnD = false;
			bTestInter_NxN = false;
			bTestIntra = false;
			bIsSplit = true;
#if TR_DEBUG_WRITE_DECISION_PROFILE
			//Writes the decision profile.
			m_pcTrainingData->addDecisionProfile_LDF_v5(uiLevel, uiQP, 0);
#endif
		}
		else
		{
			bTestSkip = true;
			bTestInter_2Nx2N = true;
			bTestInter_Nx2N_2NxN = true;
			bTestInter_nLnRxnUnD = true;
			bTestInter_NxN = false;
			bTestIntra = false;
			bIsSplit = false;
		}
	}
	else
	{
		bTestSkip = true; //Maybe I do not need to test all this. Not sure.
		bTestInter_2Nx2N = true;
		bTestInter_Nx2N_2NxN = true;
		bTestInter_nLnRxnUnD = true;
		bTestInter_NxN = false;
		bTestIntra = true;
		bIsSplit = true;
	}

	//Set all modes.
	modeDecision->setTestModesInBatch(bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra);

	//Free the features.
	free(dFeatures);

	/*
	printf("Mode Decision: \n");
	printf("bTestSkip            = %s \n",bTestSkip ? "true" : "false");
	printf("bTestInter_2Nx2N     = %s \n",bTestInter_2Nx2N ? "true" : "false");
	printf("bTestInter_Nx2N_2NxN = %s \n",bTestInter_Nx2N_2NxN ? "true" : "false");
	printf("bTestInter_nLnRxnUnD = %s \n",bTestInter_nLnRxnUnD ? "true" : "false");
	printf("bTestInter_NxN       = %s \n",bTestInter_NxN ? "true" : "false");
	printf("bTestIntra           = %s \n",bTestIntra ? "true" : "false");
	printf("bIsSplit             = %s \n",bIsSplit ? "true" : "false");
	printf("BREAKPOINT.\n");
	*/

	return bIsSplit;
}

/**
*/

Bool TTrEngine::xModeDecision_Tr_DecisionTrees_v1(UInt uiQP, UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
	//The possible modes
	Bool bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra;
	Bool bIsSplit = false;

	Double *dFeatures = NULL;

	//printf("Number of features: %d .\n", getNumberOfFeatures());

	//Computes the features.
	dFeatures = computeFeaturesLDF_v5(uiLevel, uiRow, uiCol);
    //printf("number of features: %d",getNumberOfFeatures());

	//Checks if there is intra MBs here.
	Bool isIntra = ((computeFeaturesForIntraMBs()) ? isAllIntraMBs(uiLevel, uiRow, uiCol) : hasIntraMBs(uiLevel, uiRow, uiCol));

	/*
	printf("================\n");
	printf("ALL original Features: \n");
	for (UInt k = 0; k < getNumberOfFeatures(); k++)
	{
		printf("Feature[%2d] = %3.2f \n", k, dFeatures[k]);
	}
	printf("================\n");
	*/
    //frame atual: m_uiCurrFrame
	//Checks if it is intra
	if (isIntra == false || m_uiCurrFrame > 12)
	{
		//Then Classify the feature.
		UInt uiClassification;

            //printf("Frame: %d, inference.", "%d\n", m_uiCurrFrame);
			if (uiLevel == 0)
			{
                //Double dFeatures_aux[6] = {dFeatures[1],dFeatures[3],dFeatures[5],dFeatures[6],dFeatures[7],dFeatures[8]};
				switch (uiQP)
				{
				case 23: uiClassification = xdecide_depth0_QP22_v3(dFeatures);
					     break;
				case 24: uiClassification = xdecide_depth0_QP22_v3(dFeatures);
					     break;
				case 25: uiClassification = xdecide_depth0_QP22_v3(dFeatures);
					     break;
				case 28: uiClassification = xdecide_depth0_QP27_v3(dFeatures);
					     break;
				case 29: uiClassification = xdecide_depth0_QP27_v3(dFeatures);
					     break;
				case 30: uiClassification = xdecide_depth0_QP27_v3(dFeatures);
					     break;
				case 33: uiClassification = xdecide_depth0_QP32_v3(dFeatures);
					     break;
				case 34: uiClassification = xdecide_depth0_QP32_v3(dFeatures);
				 	     break;
				case 35: uiClassification = xdecide_depth0_QP32_v3(dFeatures);
					     break;
				case 38: uiClassification = xdecide_depth0_QP37_v3(dFeatures);
					     break;
				case 39: uiClassification = xdecide_depth0_QP37_v3(dFeatures);
					     break;
				case 40: uiClassification = xdecide_depth0_QP37_v3(dFeatures);
					     break;
				default: printf("Não existe uma árvore estática definida para este valor de QP");
				}
			}

			if (uiLevel == 1)
			{
                Double dFeatures_aux[6] = {dFeatures[5],dFeatures[6],dFeatures[8]};
				switch (uiQP)
				{
				case 23: uiClassification = xdecide_depth1_QP22_v3(dFeatures);
					break;
				case 24: uiClassification = xdecide_depth1_QP22_v3(dFeatures);
					break;
				case 25: uiClassification = xdecide_depth1_QP22_v3(dFeatures);
					break;
				case 28: uiClassification = xdecide_depth1_QP27_v3(dFeatures);
					break;
				case 29: uiClassification = xdecide_depth1_QP27_v3(dFeatures);
					break;
				case 30: uiClassification = xdecide_depth1_QP27_v3(dFeatures);
					break;
				case 33: uiClassification = xdecide_depth1_QP32_v3(dFeatures);
					break;
				case 34: uiClassification = xdecide_depth1_QP32_v3(dFeatures);
					break;
				case 35: uiClassification = xdecide_depth1_QP32_v3(dFeatures);
					break;
				case 38: uiClassification = xdecide_depth1_QP37_v3(dFeatures);
					break;
				case 39: uiClassification = xdecide_depth1_QP37_v3(dFeatures);
					break;
				case 40: uiClassification = xdecide_depth1_QP37_v3(dFeatures);
					break;
				default: printf("Não existe uma árvore estática definida para este valor de QP");
				}
			}



		//printf("STAGE %d - Classification = %d .\n",0,uiClassification);
		if (uiClassification == 0)
		{
			bTestSkip = false;
			bTestInter_2Nx2N = false;
			bTestInter_Nx2N_2NxN = false;
			bTestInter_nLnRxnUnD = false;
			bTestInter_NxN = false;
			bTestIntra = false;
			bIsSplit = true;
		}
		else
		{
			bTestSkip = true;
			bTestInter_2Nx2N = true;
			bTestInter_Nx2N_2NxN = true;
			bTestInter_nLnRxnUnD = true;
			bTestInter_NxN = false;
			bTestIntra = false;
			bIsSplit = false;
		}
	}
	else
	{
		bTestSkip = true; //Maybe I do not need to test all this. Not sure.
		bTestInter_2Nx2N = true;
		bTestInter_Nx2N_2NxN = true;
		bTestInter_nLnRxnUnD = true;
		bTestInter_NxN = false;
		bTestIntra = true;
		bIsSplit = true;
	}

	//Set all modes.
	modeDecision->setTestModesInBatch(bTestSkip, bTestInter_2Nx2N, bTestInter_Nx2N_2NxN, bTestInter_nLnRxnUnD, bTestInter_NxN, bTestIntra);

	//Free the features.
	free(dFeatures);

	return bIsSplit;
}


Bool TTrEngine::checkEarlyTermination_LDF_v5(UInt uiDepth, UInt uiRow, UInt uiCol, UInt uiQP, UInt uiMode, Double dBestCost, TTrModeDecision *modeDecision)
{
  //Get the sieve for the current Depth, QP, mode
  Double dSieve = m_pcTrainingData->getSieve_LDF_v5(uiDepth, uiQP, uiMode);

  //Get the decision to early terminate.
  Bool bEarlyTerminate = ((dSieve < 0) ? false : (dBestCost < dSieve));

  //printf("BestCost        : %f \n",dBestCost);
  //printf("Sieve           : %f \n",dSieve);
  //printf("Early Terminate : %s \n",bEarlyTerminate ? "yes" : "no");

#if TR_DEBUG_WRITE_DECISION_PROFILE
  //Writes the decision profile.
  if (bEarlyTerminate)
  {
    m_pcTrainingData->addSieveProfile_LDF_v5(uiDepth, uiQP, uiMode - 1);
  } else if (uiMode == 3)
  {
	//Falls through
	m_pcTrainingData->addSieveProfile_LDF_v5(uiDepth, uiQP, 3);
  }
#endif

  //If the decision IS to earl terminate, nothing else needs to be done. Otherwise...
  if (bEarlyTerminate == false)
  {
	//Now, it depends on the current mode (AND depth).
	switch (uiMode)
	{
	  //SKIP - This is easy - test the inter for the same depth.
	  case 1:
		modeDecision->setTestModesInBatch(true,true,true,true,false,false);
		break;

	  //INTER - This is easy - test the intra for the same depth.
	  case 2:
		modeDecision->setTestModesInBatch(true,true,true,true,false,true);
		break;

	  //INTRA - This is complex - I should now allow the SPLIT!.
	  case 3:
		//Sets the partition to split.
        modeDecision->setSplit(true);

		//Calls the setModeDecision for each of the children!
	    setModeDecision(uiQP, uiDepth + 1, 2 * uiRow    , 2 * uiCol    , modeDecision->getChild(0));
	    setModeDecision(uiQP, uiDepth + 1, 2 * uiRow    , 2 * uiCol + 1, modeDecision->getChild(1));
	    setModeDecision(uiQP, uiDepth + 1, 2 * uiRow + 1, 2 * uiCol    , modeDecision->getChild(2));
	    setModeDecision(uiQP, uiDepth + 1, 2 * uiRow + 1, 2 * uiCol + 1, modeDecision->getChild(3));
		break;
	}
  }

  //Return the early termination decision
  return bEarlyTerminate;
}

/**
 */
Bool TTrEngine::xModeDecision_TrDepthBased(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  /*ANOTHER SOLUTION WOULD BE TO CHECK THE PARENT FRAME BOUNDARY IN HERE.
    - For that, I need:
     - the frame size (TTrEngine has that)
	 - the uiLevel, uiRow, uiCol of the parent (easy to obtain from the parameters).
	 - the partition size (easy to obtain from above)
	MAYBE THIS IS THE BETTER SOLUTION.
	*/

  //First of all, I need to check if the parent partition was valid.
  Bool bIsParentValid = true;

  if (uiLevel != 0)
  {
    UInt uiParentLevel = uiLevel - 1;
	UInt uiParentRow   = uiRow >> 1;
	UInt uiParentCol   = uiCol >> 1;
	UInt uiParentSize  = (m_uiLCUSize >> uiParentLevel);

	UInt uiParentRow_px = uiParentRow * uiParentSize;
	UInt uiParentCol_px = uiParentCol * uiParentSize;

	bIsParentValid = (( (uiParentRow_px + uiParentSize -1) <= (m_uiFrameHeight) ) &&  ( (uiParentCol_px + uiParentSize -1) <= (m_uiFrameWidth) ));
  }

  //Checks if some depth beyond this will be tested.
  Bool bIsSplit = false;
  for (UInt ui_k = uiLevel+1; ui_k < TR_MAX_DEPTH; ui_k++)
  {
	bIsSplit = m_pcConfig->isTestThisDepth(ui_k);
	if (bIsSplit) break;
  }

  //Checks if this depth is tested.
  //This depth will be tested:
  // - if the config file says so; OR
  // - if the parent partition is NOT valid (i.e., the parent partition is outside the frame boundaries) AND the smaller partitions will NOT be tested.
  Bool bTestThisDepth = m_pcConfig->isTestThisDepth(uiLevel) || (!bIsParentValid && !bIsSplit)   ;

  //The NxN mode is ONLY tested in the final level.
  Bool bTestInter_NxN = (uiLevel == m_uiMaxLevel) && bTestThisDepth;

  //Set all modes to true.
  modeDecision->setTestModesInBatch(bTestThisDepth,bTestThisDepth,bTestThisDepth,bTestThisDepth,bTestInter_NxN,bTestThisDepth);

  return bIsSplit;
}

/**
 */
Bool TTrEngine::xModeDecision_FromH264MB(UInt uiLevel, UInt uiRow, UInt uiCol, TTrModeDecision* modeDecision)
{
  Bool bTestModes[12] = {false, false, false, false,false, false, false, false,false, false, false, false};
  Bool bIsSplit                = false;

  //A pointer to the frame object.
  TTrH264FrameMI *pcH264Frame = getCurrentH264FrameMI();

  //If this function was called, then the level is either 16x16 or 8x8.
  UInt uiLevel_16x16, uiRow_16x16, uiCol_16x16;
  UInt uiBlock8x8;

  //Find out the level for 16x16 partitions.
  uiLevel_16x16 = 0;
  while ((m_uiLCUSize >> uiLevel_16x16) != 16) { uiLevel_16x16++; }

  uiRow_16x16 = ((uiLevel_16x16 == uiLevel) ? uiRow : (uiRow >> 1));
  uiCol_16x16 = ((uiLevel_16x16 == uiLevel) ? uiCol : (uiCol >> 1));

  //Now it depends on the level.
  if (uiLevel == uiLevel_16x16)
  {
    //Gets the H.264 partition.
    H264MBModeTypes cMBType = pcH264Frame->getFrameMBInfo()->getMBType(uiRow_16x16, uiCol_16x16);

    switch (m_pcConfig->getH264MBTranscodingMethod())
	{
	  case TR_REUSE_H264MB_EQUAL:
	    //Now, it makes the decision according to this.
	    switch (cMBType)
        {
          case H264_PSKIP:
		    bTestModes[SKIP_MERGE] = true;
		    break;

          case H264_P16x16:
            bTestModes[INTER_2Nx2N] = true;
		    break;

          case H264_P16x8:
		    bTestModes[INTER_2NxN] = true;
		    break;

	      case H264_P8x16:
		    bTestModes[INTER_Nx2N] = true;
		    break;

		  case H264_P8x8:
		    bIsSplit = true;
		    break;

		  case H264_I4MB:
		  case H264_SI4MB:
		  case H264_I8MB:
			bIsSplit = true;
			break;

		  case H264_I16MB:
		  case H264_IBLOCK:
		    bTestModes[INTRA_2Nx2N] = true;
		    break;

		  case H264_IPCM:
            bTestModes[INTRA_PCM] = true;
			break;
        }
		break;

	  case TR_REUSE_H264MB_LARGER:
		switch (cMBType)
        {
          case H264_PSKIP:
		    bTestModes[SKIP_MERGE]  = true;
		    break;

          case H264_P16x16:
		    bTestModes[SKIP_MERGE]  = true;
            bTestModes[INTER_2Nx2N] = true;
		    break;

          case H264_P16x8:
		    bTestModes[SKIP_MERGE]  = true;
            bTestModes[INTER_2Nx2N] = true;
		    bTestModes[INTER_2NxN]  = true;
			bTestModes[INTER_2NxnU] = true;
			bTestModes[INTER_2NxnD] = true;
		    break;

	      case H264_P8x16:
			bTestModes[SKIP_MERGE]  = true;
            bTestModes[INTER_2Nx2N] = true;
		    bTestModes[INTER_Nx2N]  = true;
			bTestModes[INTER_nLx2N] = true;
			bTestModes[INTER_nRx2N] = true;
		    break;

		  case H264_P8x8:
		    bTestModes[SKIP_MERGE]  = true;
            bTestModes[INTER_2Nx2N] = true;
		    bTestModes[INTER_2NxN]  = true;
			bTestModes[INTER_2NxnU] = true;
			bTestModes[INTER_2NxnD] = true;
			bTestModes[INTER_Nx2N]  = true;
			bTestModes[INTER_nLx2N] = true;
			bTestModes[INTER_nRx2N] = true;
		    bIsSplit = true;
		    break;

		  case H264_I4MB:
		  case H264_SI4MB:
		  case H264_I8MB:
		  case H264_I16MB:
		  case H264_IBLOCK:
		  case H264_IPCM:
			bIsSplit = true;
			bTestModes[SKIP_MERGE]  = true;
            bTestModes[INTER_2Nx2N] = true;
            bTestModes[INTER_Nx2N]  = true;
            bTestModes[INTER_2NxN]  = true;
            bTestModes[INTER_2NxnU] = true;
            bTestModes[INTER_2NxnD] = true;
            bTestModes[INTER_nLx2N] = true;
            bTestModes[INTER_nRx2N] = true;
            bTestModes[INTER_NxN]   = true;
            bTestModes[INTRA_2Nx2N] = true;
            bTestModes[INTRA_NxN]   = true;
            bTestModes[INTRA_PCM]   = true;
			break;
        }
		break;

	  case TR_REUSE_H264MB_SKIP:
	    switch (cMBType)
        {
          case H264_PSKIP:
		    bTestModes[SKIP_MERGE]  = true;
			bTestModes[INTER_2Nx2N] = true;
		    break;

          case H264_P16x16:
		  case H264_P16x8:
		  case H264_P8x16:
		    bTestModes[SKIP_MERGE]  = true;
            bTestModes[INTER_2Nx2N] = true;
		    bTestModes[INTER_2NxN]  = true;
			bTestModes[INTER_2NxnU] = true;
			bTestModes[INTER_2NxnD] = true;
			bTestModes[INTER_Nx2N]  = true;
			bTestModes[INTER_nLx2N] = true;
			bTestModes[INTER_nRx2N] = true;
		    break;

		  case H264_P8x8:
		    bTestModes[SKIP_MERGE]  = true;
            bTestModes[INTER_2Nx2N] = true;
		    bTestModes[INTER_2NxN]  = true;
			bTestModes[INTER_2NxnU] = true;
			bTestModes[INTER_2NxnD] = true;
			bTestModes[INTER_Nx2N]  = true;
			bTestModes[INTER_nLx2N] = true;
			bTestModes[INTER_nRx2N] = true;
		    bIsSplit = true;
		    break;

		  case H264_I4MB:
		  case H264_SI4MB:
		  case H264_I8MB:
		  case H264_I16MB:
		  case H264_IBLOCK:
		  case H264_IPCM:
			bIsSplit = true;
			bTestModes[SKIP_MERGE]  = true;
            bTestModes[INTER_2Nx2N] = true;
            bTestModes[INTER_Nx2N]  = true;
            bTestModes[INTER_2NxN]  = true;
            bTestModes[INTER_2NxnU] = true;
            bTestModes[INTER_2NxnD] = true;
            bTestModes[INTER_nLx2N] = true;
            bTestModes[INTER_nRx2N] = true;
            bTestModes[INTER_NxN]   = true;
            bTestModes[INTRA_2Nx2N] = true;
            bTestModes[INTRA_NxN]   = true;
            bTestModes[INTRA_PCM]   = true;
			break;
        }
		break;
	}
  }
  else
  {
    UInt y, x;

	y = uiRow - ((uiRow >> 1) << 1);
    x = uiCol - ((uiCol >> 1) << 1);

	uiBlock8x8 = ((y == 0) ? 0 : 2) + ((x == 0) ? 0 : 1);

	//Gets the H.264 partition.
    H264MBModeTypes cMBType = pcH264Frame->getFrameMBInfo()->getMBType(uiRow_16x16, uiCol_16x16);
	//Gets the H.264 SMB partition.
    H264MBModeTypes cSMBType = pcH264Frame->getFrameMBInfo()->getSMBType(uiRow_16x16, uiCol_16x16,uiBlock8x8);

    switch (m_pcConfig->getH264MBTranscodingMethod())
	{
	  case TR_REUSE_H264MB_EQUAL:
	    if (cMBType == H264_P8x8)
		{
		  switch (cSMBType)
		  {
		    case H264_BSKIP_DIRECT:
			  bTestModes[SKIP_MERGE] = true;
			  break;

		    case H264_SMB8x8:
			  bTestModes[INTER_2Nx2N] = true;
			  break;

		    case H264_SMB8x4:
			  bTestModes[INTER_2NxN] = true;
			  break;

		    case H264_SMB4x8:
			  bTestModes[INTER_Nx2N] = true;
			  break;

		    case H264_SMB4x4:
			  //I cannot trust in the INTER NxN, thus I have to test other options as well.
			  bTestModes[INTER_2NxN] = true;
			  bTestModes[INTER_Nx2N] = true;
			  bTestModes[INTER_NxN] = true;
			  break;
		  }
		} else //Then, it had to be intra.
		{
		  bTestModes[INTRA_2Nx2N] = true;
          bTestModes[INTRA_NxN]   = (cMBType == H264_I4MB);
		}
		break;

	  case TR_REUSE_H264MB_LARGER:
	    if (cMBType == H264_P8x8)
		{
		  switch (cSMBType)
		  {
		    case H264_BSKIP_DIRECT:
			  bTestModes[SKIP_MERGE] = true;
			  break;

		    case H264_SMB8x8:
			  bTestModes[SKIP_MERGE]  = true;
			  bTestModes[INTER_2Nx2N] = true;
			  break;

		    case H264_SMB8x4:
			  bTestModes[SKIP_MERGE]  = true;
			  bTestModes[INTER_2Nx2N] = true;
			  bTestModes[INTER_2NxN]  = true;
			  bTestModes[INTER_2NxnU] = true;
              bTestModes[INTER_2NxnD] = true;
			  break;

		    case H264_SMB4x8:
			  bTestModes[SKIP_MERGE]  = true;
			  bTestModes[INTER_2Nx2N] = true;
			  bTestModes[INTER_Nx2N]  = true;
			  bTestModes[INTER_nLx2N] = true;
              bTestModes[INTER_nRx2N] = true;
			  break;

		    case H264_SMB4x4:
			  bTestModes[SKIP_MERGE]  = true;
			  bTestModes[INTER_2Nx2N] = true;
			  bTestModes[INTER_2NxN]  = true;
			  bTestModes[INTER_2NxnU] = true;
              bTestModes[INTER_2NxnD] = true;
			  bTestModes[INTER_Nx2N]  = true;
			  bTestModes[INTER_nLx2N] = true;
              bTestModes[INTER_nRx2N] = true;
			  bTestModes[INTER_NxN]   = true;
			  break;
		  }
		} else //Then, it had to be intra.
		{
		  bTestModes[SKIP_MERGE]  = true;
          bTestModes[INTER_2Nx2N] = true;
          bTestModes[INTER_Nx2N]  = true;
          bTestModes[INTER_2NxN]  = true;
          bTestModes[INTER_2NxnU] = true;
          bTestModes[INTER_2NxnD] = true;
          bTestModes[INTER_nLx2N] = true;
          bTestModes[INTER_nRx2N] = true;
          bTestModes[INTER_NxN]   = true;
          bTestModes[INTRA_2Nx2N] = true;
          bTestModes[INTRA_NxN]   = true;
          bTestModes[INTRA_PCM]   = true;
		}
		break;

	  case TR_REUSE_H264MB_SKIP:
	    if (cMBType == H264_P8x8)
		{
		  switch (cSMBType)
		  {
		    case H264_BSKIP_DIRECT:
			case H264_SMB8x8:
			  bTestModes[SKIP_MERGE]  = true;
			  bTestModes[INTER_2Nx2N] = true;
			  break;

		    case H264_SMB8x4:
			  bTestModes[SKIP_MERGE]  = true;
			  bTestModes[INTER_2Nx2N] = true;
			  bTestModes[INTER_2NxN]  = true;
			  bTestModes[INTER_2NxnU] = true;
              bTestModes[INTER_2NxnD] = true;
			  break;

		    case H264_SMB4x8:
			  bTestModes[SKIP_MERGE]  = true;
			  bTestModes[INTER_2Nx2N] = true;
			  bTestModes[INTER_Nx2N]  = true;
			  bTestModes[INTER_nLx2N] = true;
              bTestModes[INTER_nRx2N] = true;
			  break;

		    case H264_SMB4x4:
			  bTestModes[SKIP_MERGE]  = true;
			  bTestModes[INTER_2Nx2N] = true;
			  bTestModes[INTER_2NxN]  = true;
			  bTestModes[INTER_2NxnU] = true;
              bTestModes[INTER_2NxnD] = true;
			  bTestModes[INTER_Nx2N]  = true;
			  bTestModes[INTER_nLx2N] = true;
              bTestModes[INTER_nRx2N] = true;
			  bTestModes[INTER_NxN]   = true;
			  break;
		  }
		} else //Then, it had to be intra.
		{
		  bTestModes[SKIP_MERGE]  = true;
          bTestModes[INTER_2Nx2N] = true;
          bTestModes[INTER_Nx2N]  = true;
          bTestModes[INTER_2NxN]  = true;
          bTestModes[INTER_2NxnU] = true;
          bTestModes[INTER_2NxnD] = true;
          bTestModes[INTER_nLx2N] = true;
          bTestModes[INTER_nRx2N] = true;
          bTestModes[INTER_NxN]   = true;
          bTestModes[INTRA_2Nx2N] = true;
          bTestModes[INTRA_NxN]   = true;
          bTestModes[INTRA_PCM]   = true;
		}
		break;
	}
  }

  //Sets the correct modes.
  modeDecision->setTestMode(SKIP_MERGE  , bTestModes[SKIP_MERGE]  );
  modeDecision->setTestMode(INTER_2Nx2N , bTestModes[INTER_2Nx2N] );
  modeDecision->setTestMode(INTER_Nx2N  , bTestModes[INTER_Nx2N]  );
  modeDecision->setTestMode(INTER_2NxN  , bTestModes[INTER_2NxN]  );
  modeDecision->setTestMode(INTER_2NxnU , bTestModes[INTER_2NxnU] );
  modeDecision->setTestMode(INTER_2NxnD , bTestModes[INTER_2NxnD] );
  modeDecision->setTestMode(INTER_nLx2N , bTestModes[INTER_nLx2N] );
  modeDecision->setTestMode(INTER_nRx2N , bTestModes[INTER_nRx2N] );
  modeDecision->setTestMode(INTER_NxN   , bTestModes[INTER_NxN]   );
  modeDecision->setTestMode(INTRA_2Nx2N , bTestModes[INTRA_2Nx2N] );
  modeDecision->setTestMode(INTRA_NxN   , bTestModes[INTRA_NxN]   );
  modeDecision->setTestMode(INTRA_PCM   , bTestModes[INTRA_PCM]   );
  return bIsSplit;

}

/**
 */
Double TTrEngine::xComputeDotProduct(Double *dFeatures, const Double *dWeights, UInt uiNFeatures)
{
  Double dValue = 0.0;
  for (UInt ui_k = 0; ui_k < uiNFeatures; ui_k++)
  {
    //printf("%f + ",dValue);
    dValue += dFeatures[ui_k] * dWeights[ui_k];
	//printf("%f * %f = %f.\n",dFeatures[ui_k],dWeights[ui_k],dValue);
  }
  return dValue;
}

/** Calcula o número de feature utilizado.
 */
UInt TTrEngine::xComputeNumberOfFeatures()
{
  UInt uiNFeatures = 0;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_ONE))                              uiNFeatures++;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_PARTITIONS_2Nx2N))       uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2Nx2N))       uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_X_2Nx2N))              uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2Nx2N))              uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2Nx2N))          uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2Nx2N)) uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2Nx2N)) uiNFeatures++;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxN))        uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_Nx2N))        uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN))         uiNFeatures += 4;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxnU))       uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_2NxnD))       uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_nLx2N))       uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_nRx2N))       uiNFeatures += 2;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxN))               uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_X_Nx2N))               uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_X_NxN))                uiNFeatures += 4;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxnU))              uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_X_2NxnD))              uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_X_nLx2N))              uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_X_nRx2N))              uiNFeatures += 2;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxN))               uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_Y_Nx2N))               uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_Y_NxN))                uiNFeatures += 4;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxnU))              uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_Y_2NxnD))              uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_Y_nLx2N))              uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_Y_nRx2N))              uiNFeatures += 2;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2NxN))           uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_Nx2N))           uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_NxN))            uiNFeatures += 4;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2NxnU))          uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_2NxnD))          uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_nLx2N))          uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_nRx2N))          uiNFeatures += 2;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2NxN))  uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_Nx2N))  uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN))   uiNFeatures += 4;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2NxnU)) uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_2NxnD)) uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_nLx2N)) uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_nRx2N)) uiNFeatures += 2;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2NxN))  uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_Nx2N))  uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_NxN))   uiNFeatures += 4;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2NxnU)) uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_2NxnD)) uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_nLx2N)) uiNFeatures += 2;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_ENERGY_OF_DCT_COEFFICIENTS_nRx2N)) uiNFeatures += 2;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MB_DISTRIBUTION_9_CLASSES))        uiNFeatures += 9;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_2Nx2N))  uiNFeatures += 4;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MB_DISTRIBUTION_4_CLASSES_NxN))    uiNFeatures += 16;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MEAN))                              uiNFeatures++;

  if (m_pcConfig->computeFeature(TR_HEVCFEATURE_COST))                             uiNFeatures += 11;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_VARIANCE_DISTANCE_NxN_MAX))       uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_VARIANCE_NxN_MAX))          uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_NUMBER_OF_DCT_COEFFICIENTS_NxN_MAX)) uiNFeatures++;

  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_TRIGONOMETRIC_VARIANCE_2Nx2N))   uiNFeatures++;
  if (m_pcConfig->computeFeature(TR_H264FEATURE_MV_PHASE_TRIGONOMETRIC_VARIANCE_NxN_MAX)) uiNFeatures++;

  return uiNFeatures;
}

//=====================================================================================
// Transcoder_v1 trees (Matlab Default Decision Trees)
UInt TTrEngine::xdecide_depth0_QP22_v1(Double *dFeatures)
{
	if (dFeatures[4] >= 27.5)
	{
		if (dFeatures[4] >= 80.5)
			return 0;
		else
		{
			if (dFeatures[8] >= 0.03125)
				return 0;
			else
			{
				if (dFeatures[4] >= 45.5)
					return 0;
				else
				{
					if (dFeatures[3] >= 0.00112)
						return 0;
					else
						return 1;
				}
			}
		}
	}
	else
	{
		if (dFeatures[6] >= 0.84375)
			return 1;
		else
		{
			if (dFeatures[4] >= 9.5)
			{
				if (dFeatures[8] >= 0.03125)
					return 0;
				else
				{
					if (dFeatures[7] >= 0.28125)
						return 0;
					else
						return 1;
				}
			}
			else
			{
				if (dFeatures[8] >= 0.03125)
					return 0;
				else
					return 1;
			}
		}
	}
}

UInt TTrEngine::xdecide_depth0_QP27_v1(Double *dFeatures)
{
	if (dFeatures[6] >= 0.71875)
	{
		if (dFeatures[6] >= 0.90625)
			return 1;
		else
		{
			if (dFeatures[3] >= 0.005428)
			{
				if (dFeatures[8] >= 0.03125)
					return 0;
				else
					return 1;
			}
			else
				return 1;
		}
	}
	else
	{
		if (dFeatures[8] >= 0.03125)
			return 0;
		else
		{
			if (dFeatures[6] >= 0.46875)
			{
				if (dFeatures[5] >= 32.5)
					return 0;
				else
				{
					if (dFeatures[2] >= 0.001096)
						return 0;
					else
						return 1;
				}
			}
			else
				return 0;
		}
	}
}

UInt TTrEngine::xdecide_depth0_QP32_v1(Double *dFeatures)
{
	if (dFeatures[6] >= 0.71875)
	{
		if (dFeatures[6] >= 0.90625)
			return 1;
		else
		{
			if (dFeatures[3] >= 0.0014465)
			{
				if (dFeatures[8] >= 0.03125)
					return 0;
				else
					return 1;
			}
			else
				return 1;
		}
	}
	else
	{
		if (dFeatures[6] >= 0.46875)
		{
			if (dFeatures[8] >= 0.03125)
				return 0;
			else
			{
				if (dFeatures[5] >= 16.5)
					return 0;
				else
				{
					if (dFeatures[3] >= 0.0000275)
						return 0;
					else
						return 1;
				}
			}
		}
		else
		{
			if (dFeatures[8] >= 0.03125)
				return 0;
			else
			{
				if (dFeatures[3] >= 0.0000015)
					return 0;
				else
				{
					if (dFeatures[4] >= 48.5)
						return 0;
					else
						return 1;
				}
			}
		}
	}
}


UInt TTrEngine::xdecide_depth0_QP37_v1(Double *dFeatures)
{
	if (dFeatures[6] >= 0.65625)
	{
		if (dFeatures[7] >= 0.03125)
		{
			if (dFeatures[6] >= 0.84375)
				return 1;
			else
			{
				if (dFeatures[0] >= 104.383)
				{
					if (dFeatures[4] >= 16.5)
						return 0;
					else
						return 1;
				}
				else
				{
					if (dFeatures[8] >= 0.03125)
						return 0;
					else
						return 1;
				}
			}
		}
		else
			return 1;
	}
	else
	{
		if (dFeatures[3] >= 0.0002685)
		{
			if (dFeatures[6] >= 0.40625)
			{
				if (dFeatures[8] >= 0.03125)
					return 0;
				else
				{
					if (dFeatures[6] >= 0.53125)
						return 1;
					else
						return 0;
				}
			}
			else
				return 0;
		}
		else
		{
			if (dFeatures[5] >= 16.5)
			{
				if (dFeatures[5] >= 32.5)
					return 0;
				else
					return 1;
			}
			else
				return 1;
		}
	}
}

UInt TTrEngine::xdecide_depth1_QP22_v1(Double *dFeatures)
{
	if (dFeatures[5] >= 24.5)
	{
		if (dFeatures[8] >= 0.375)
			return 0;
		else
		{
			if (dFeatures[5] >= 38.5)
				return 0;
			else
			{
				if (dFeatures[8] >= 0.125)
					return 0;
				else
					return 1;
			}
		}
	}
	else
	{
		if (dFeatures[8] >= 0.125)
		{
			if (dFeatures[8] >= 0.375)
				return 0;
			else
			{
				if (dFeatures[0] >= 1.35696)
					return 1;
				else
				{
					if (dFeatures[5] >= 10.5)
						return 0;
					else
						return 1;
				}
			}
		}
		else
			return 1;
	}
}

UInt TTrEngine::xdecide_depth1_QP27_v1(Double *dFeatures)
{
	if (dFeatures[8] >= 0.125)
	{
		if (dFeatures[5] >= 12.5)
		{
			if (dFeatures[8] >= 0.375)
				return 0;
			else
			{
				if (dFeatures[5] >= 23.5)
					return 0;
				else
				{
					if (dFeatures[9] >= 0.125)
						return 1;
					else
						return 0;

				}
			}
		}
		else
		{
			if (dFeatures[8] >= 0.375)
				return 0;
			else
				return 1;
		}
	}
	else
	{
		if (dFeatures[5] >= 2.5)
		{
			if (dFeatures[5] >= 21.5)
			{
				if (dFeatures[5] >= 32.5)
					return 0;
				else
					return 1;
			}
			else
				return 1;
		}
		else
			return 1;
	}
}

UInt TTrEngine::xdecide_depth1_QP32_v1(Double *dFeatures)
{
	if (dFeatures[3] >= 0.0000005)
	{
		if (dFeatures[8] >= 0.125)
		{
			if (dFeatures[5] >= 19.5)
				return 0;
			else
			{
				if (dFeatures[8] >= 0.375)
					return 0;
				else
					return 1;
			}
		}
		else
		{
			if (dFeatures[5] >= 4.5)
			{
				if (dFeatures[5] >= 18.5)
				{
					if (dFeatures[5] >= 33.5)
						return 0;
					else
						return 1;
				}
				else
					return 1;
			}
			else
				return 1;
		}
	}
	else
	{
		if (dFeatures[5] >= 2.5)
		{
			if (dFeatures[5] >= 17.5)
			{
				if (dFeatures[5] >= 24.5)
					return 0;
				else
					return 1;
			}
			else
				return 1;
		}
		else
			return 1;
	}
}

UInt TTrEngine::xdecide_depth1_QP37_v1(Double *dFeatures)
{
	if (dFeatures[3] >= 0.0000005)
		return 1;
	else
	{
		if (dFeatures[4] >= 0.5)
		{
			if (dFeatures[7] >= 0.125)
			{
				if (dFeatures[5] >= 16.5)
				{
					if (dFeatures[5] >= 23.5)
						return 0;
					else
						return 1;
				}
				else
					return 1;
			}
			else
				return 1;
		}
		else
			return 1;
	}

}


//============================================================================
//Transcoder v2 Trees (Matlab Cost Based Decision Trees)

UInt TTrEngine::xdecide_depth0_QP22_v2(Double *dFeatures)
{
	if (dFeatures[4] >= 25)
		return 0;
	else
	{
		if (dFeatures[6] >= 0.852)
		{
			if (dFeatures[4] >= 2)
				return 0;
			else
				return 1;
		}
		else
			return 0;
	}
}

UInt TTrEngine::xdecide_depth1_QP22_v2(Double *dFeatures)
{
	if (dFeatures[5] >= 24)
		return 0;
	else
	{
		if (dFeatures[8] >= 0.0526)
		{
			if (dFeatures[8] >= 0.2895)
				return 0;
			else
			{
				if (dFeatures[0] >= 1.2917)
				{
					if (dFeatures[2] >= 0.0319)
						return 0;
					else
						return 1;
				}
				else
					return 0;
			}
		}
		else
		{
			if (dFeatures[5] >= 5)
			{
				if (dFeatures[2] >= 0.0056)
				{
					if (dFeatures[5] >= 10)
					{
						if (dFeatures[0] >= 2.5175)
							return 1;
						else
							return 0;
					}
					else
						return 1;
				}
				else
					return 1;
			}
			else
				return 1;
		}
	}
}

UInt TTrEngine::xdecide_depth0_QP27_v2(Double *dFeatures)
{
	if (dFeatures[6] >= 0.7368)
	{
		if (dFeatures[6] >= 0.8816)
			return 1;
		else
		{
			if (dFeatures[3] >= 0.0047)
			{
				if (dFeatures[8] >= 0.0132)
					return 0;
				else
				{
					if (dFeatures[4] >= 3)
						return 0;
					else
						return 1;
				}
			}
			else
				return 1;
		}
	}
	else
		return 0;
}

UInt TTrEngine::xdecide_depth1_QP27_v2(Double *dFeatures)
{
	if (dFeatures[8] >= 0.0526)
	{
		if (dFeatures[5] >= 14)
			return 0;
		else
		{
			if (dFeatures[8] >= 0.2895)
				return 0;
			else
			{
				if (dFeatures[5] >= 5)
					return 0;
				else
					return 1;
			}
		}
	}
	else
	{
		if (dFeatures[5] >= 4)
		{
			if (dFeatures[5] >= 19)
			{
				if (dFeatures[5] >= 26)
					return 0;
				else
				{
					if (dFeatures[2] >= 0.001)
						return 0;
					else
						return 1;
				}
			}
			else
			{
				if (dFeatures[2] >= 0.0058)
				{
					if (dFeatures[5] >= 9)
					{
						if (dFeatures[9] >= 0.0395)
							return 1;
						else
						{
							if (dFeatures[6] >= 0.0395)
								return 0;
							else
								return 1;
						}
					}
					else
						return 1;
				}
				else
					return 1;
			}
		}
		else
			return 1;
	}
}

UInt TTrEngine::xdecide_depth0_QP32_v2(Double *dFeatures)
{
	if (dFeatures[6] >= 0.7368)
	{
		if (dFeatures[6] >= 0.8816)
			return 1;
		else
		{
			if (dFeatures[3] >= 0.0024)
				return 0;
			else
			{
				if (dFeatures[5] >= 17)
					return 0;
				else
					return 1;
			}
		}
	}
	else
	{
		if (dFeatures[6] >= 0.4704)
		{
			if (dFeatures[8] >= 0.0197)
				return 0;
			else
			{
				if (dFeatures[5] >= 17)
					return 0;
				else
				{
					if (dFeatures[3] >= 0.000326)
						return 0;
					else
						return 1;
				}
			}
		}
		else
			return 0;
	}
}

UInt TTrEngine::xdecide_depth1_QP32_v2(Double *dFeatures)
{
	if (dFeatures[5] >= 3)
	{
		if (dFeatures[8] >= 0.0526)
			return 0;
		else
		{
			if (dFeatures[5] >= 19)
				return 0;
			else
				return 1;
		}
	}
	else
		return 1;
}

UInt TTrEngine::xdecide_depth0_QP37_v2(Double *dFeatures)
{
	if (dFeatures[6] >= 0.6316)
	{
		if (dFeatures[7] >= 0.0164)
		{
			if (dFeatures[6] >= 0.8191)
				return 1;
			else
			{
				if (dFeatures[0] >= 99.8114)
				{
					if (dFeatures[4] >= 1)
						return 0;
					else
						return 1;
				}
				else
				{
					if (dFeatures[8] >= 0.0132)
						return 0;
					else
						return 1;
				}
			}
		}
		else
		{
			if (dFeatures[5] >= 17)
				return 1;
			else
			{
				if (dFeatures[6] >= 0.9507)
					return 1;
				else
				{
					if (dFeatures[8] >= 0.0132)
						return 0;
					else
						return 1;
				}
			}
		}
	}
	else
	{
		if (dFeatures[3] >= 0.0011)
		{
			if (dFeatures[6] >= 0.3947)
			{
				if (dFeatures[8] >= 0.0164)
					return 0;
				else
				{
					if (dFeatures[6] >= 0.5066)
					{
						if (dFeatures[7] >= 0.0691)
							return 0;
						else
							return 1;
					}
					else
						return 0;
				}
			}
			else
				return 0;
		}
		else
		{
			if (dFeatures[5] >= 17)
				return 0;
			else
				return 1;
		}
	}
}

UInt TTrEngine::xdecide_depth1_QP37_v2(Double *dFeatures)
{
	if (dFeatures[4] >= 1)
	{
		if (dFeatures[3] >= 0.000449)
		{
			if (dFeatures[8] >= 0.0526)
				return 0;
			else
				return 1;
		}
		else
			return 1;
	}
	else
		return 1;
}

//=====================================================================================
// Transcoder_v3 trees (Python Default Decision Trees)
UInt TTrEngine::xdecide_depth0_QP22_v3(Double *dFeatures)
{
if(dFeatures[4] <= 0.66)
{
    if(dFeatures[3] <= 18.50)
    {
        if(dFeatures[0] <= 3.49)
        {
            if(dFeatures[4] <= 0.41)
            {
                if(dFeatures[5] <= 0.16)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[0] <= 1.28)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 0.05)
            {
                if(dFeatures[5] <= 0.59)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[5] <= 0.47)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 0.00)
        {
            if(dFeatures[5] <= 0.28)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[2] <= 58.50)
            {
                return 0;
            }
            else
            {
                return 0;
            }
        }
    }
}
else
{
    if(dFeatures[3] <= 19.50)
    {
        if(dFeatures[4] <= 0.78)
        {
            if(dFeatures[1] <= 0.00)
            {
                if(dFeatures[0] <= 11.03)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[2] <= 12.50)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 0.00)
            {
                return 0;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 0.01)
        {
            if(dFeatures[3] <= 25.50)
            {
                if(dFeatures[0] <= 24.14)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[4] <= 0.72)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            return 0;
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP27_v3(Double *dFeatures)
{
if(dFeatures[4] <= 0.72)
{
    if(dFeatures[3] <= 16.50)
    {
        if(dFeatures[3] <= 3.50)
        {
            if(dFeatures[1] <= 0.00)
            {
                if(dFeatures[0] <= 4.23)
                {
                    if(dFeatures[0] <= 1.87)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 0.16)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 4.78)
                {
                    if(dFeatures[1] <= 0.60)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.41)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 0.00)
            {
                if(dFeatures[5] <= 0.34)
                {
                    if(dFeatures[5] <= 0.09)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.64)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 0.00)
        {
            if(dFeatures[3] <= 28.00)
            {
                if(dFeatures[4] <= 0.59)
                {
                    if(dFeatures[0] <= 216.98)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[0] <= 0.51)
            {
                if(dFeatures[1] <= 0.70)
                {
                    if(dFeatures[3] <= 58.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.36)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[5] <= 0.09)
                {
                    if(dFeatures[3] <= 42.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 32.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[4] <= 0.84)
    {
        if(dFeatures[1] <= 0.00)
        {
            if(dFeatures[5] <= 0.09)
            {
                return 0;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[3] <= 16.50)
            {
                if(dFeatures[1] <= 0.63)
                {
                    if(dFeatures[3] <= 1.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 8.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 0.00)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[1] <= 0.44)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[5] <= 0.03)
        {
            if(dFeatures[1] <= 0.67)
            {
                if(dFeatures[2] <= 45.00)
                {
                    if(dFeatures[0] <= 26.21)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[3] <= 1.00)
            {
                if(dFeatures[1] <= 0.84)
                {
                    if(dFeatures[4] <= 0.91)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[1] <= 0.00)
                {
                    if(dFeatures[2] <= 8.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.36)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP32_v3(Double *dFeatures)
{
if(dFeatures[4] <= 0.78)
{
    if(dFeatures[4] <= 0.47)
    {
        if(dFeatures[0] <= 1.20)
        {
            if(dFeatures[3] <= 3.50)
            {
                if(dFeatures[0] <= 0.25)
                {
                    if(dFeatures[1] <= 0.05)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[5] <= 0.94)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.26)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[1] <= 0.01)
                        {
                            if(dFeatures[4] <= 0.41)
                            {
                                if(dFeatures[2] <= 1.50)
                                {
                                    if(dFeatures[1] <= 0.01)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 0.03)
                            {
                                return 1;
                            }
                            else
                            {
                                if(dFeatures[1] <= 0.14)
                                {
                                    if(dFeatures[1] <= 0.12)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 1.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.15)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[0] <= 1.18)
                    {
                        if(dFeatures[1] <= 1.46)
                        {
                            if(dFeatures[2] <= 51.50)
                            {
                                if(dFeatures[2] <= 17.50)
                                {
                                    if(dFeatures[5] <= 0.66)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.42)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[4] <= 0.34)
            {
                if(dFeatures[5] <= 0.28)
                {
                    if(dFeatures[3] <= 32.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[4] <= 0.16)
                    {
                        if(dFeatures[3] <= 1.50)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.71)
                            {
                                if(dFeatures[0] <= 1.54)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[3] <= 15.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 4.80)
                                {
                                    if(dFeatures[0] <= 4.69)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 0.66)
                        {
                            if(dFeatures[0] <= 2740.37)
                            {
                                if(dFeatures[2] <= 24.50)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 2.29)
                            {
                                if(dFeatures[0] <= 5.26)
                                {
                                    if(dFeatures[1] <= 0.31)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.28)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 2.43)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 0.00)
                {
                    if(dFeatures[0] <= 300.21)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 0.53)
                    {
                        if(dFeatures[2] <= 41.50)
                        {
                            if(dFeatures[5] <= 0.16)
                            {
                                if(dFeatures[4] <= 0.41)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[3] <= 34.50)
                                {
                                    if(dFeatures[1] <= 1.79)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 12.50)
                        {
                            if(dFeatures[1] <= 0.37)
                            {
                                if(dFeatures[1] <= 0.01)
                                {
                                    if(dFeatures[0] <= 5.67)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 11.50)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[3] <= 2.50)
                                {
                                    if(dFeatures[0] <= 6.71)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.57)
                            {
                                if(dFeatures[0] <= 1.34)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.02)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 0.25)
                                {
                                    if(dFeatures[0] <= 2.78)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.26)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 0.00)
        {
            if(dFeatures[5] <= 0.03)
            {
                if(dFeatures[3] <= 24.00)
                {
                    if(dFeatures[3] <= 8.00)
                    {
                        if(dFeatures[4] <= 0.72)
                        {
                            if(dFeatures[4] <= 0.59)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[4] <= 0.66)
                    {
                        if(dFeatures[2] <= 40.00)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[2] <= 56.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 182.20)
                {
                    if(dFeatures[0] <= 173.98)
                    {
                        if(dFeatures[0] <= 133.83)
                        {
                            if(dFeatures[0] <= 90.21)
                            {
                                if(dFeatures[0] <= 88.13)
                                {
                                    if(dFeatures[2] <= 17.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 135.47)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[5] <= 0.22)
                                {
                                    if(dFeatures[4] <= 0.72)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[3] <= 4.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 570.84)
                    {
                        if(dFeatures[4] <= 0.53)
                        {
                            if(dFeatures[0] <= 239.62)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 0.16)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 1.35)
            {
                if(dFeatures[0] <= 0.53)
                {
                    if(dFeatures[1] <= 0.00)
                    {
                        if(dFeatures[1] <= 0.00)
                        {
                            if(dFeatures[0] <= 0.19)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[0] <= 0.44)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 0.00)
                        {
                            if(dFeatures[0] <= 0.18)
                            {
                                if(dFeatures[2] <= 2.00)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 0.01)
                            {
                                if(dFeatures[1] <= 0.00)
                                {
                                    if(dFeatures[0] <= 0.21)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 0.13)
                                {
                                    if(dFeatures[0] <= 0.08)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.16)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 1.50)
                    {
                        if(dFeatures[1] <= 0.02)
                        {
                            if(dFeatures[4] <= 0.72)
                            {
                                if(dFeatures[0] <= 1.00)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 1.16)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 0.80)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 0.02)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[0] <= 0.67)
                                {
                                    if(dFeatures[0] <= 0.66)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.08)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 0.47)
                        {
                            if(dFeatures[5] <= 0.28)
                            {
                                if(dFeatures[1] <= 1.57)
                                {
                                    if(dFeatures[3] <= 5.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 1.13)
                                {
                                    if(dFeatures[1] <= 0.81)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[4] <= 0.66)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.18)
                            {
                                if(dFeatures[0] <= 0.70)
                                {
                                    if(dFeatures[0] <= 0.66)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 0.03)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 1.25)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 3.50)
                {
                    if(dFeatures[1] <= 0.37)
                    {
                        if(dFeatures[1] <= 0.35)
                        {
                            if(dFeatures[3] <= 1.50)
                            {
                                if(dFeatures[4] <= 0.53)
                                {
                                    if(dFeatures[1] <= 0.29)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 243.53)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 0.02)
                                {
                                    if(dFeatures[5] <= 0.28)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 4.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 268.64)
                        {
                            if(dFeatures[0] <= 223.83)
                            {
                                if(dFeatures[4] <= 0.72)
                                {
                                    if(dFeatures[0] <= 2.74)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 37.95)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 262.53)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 266.44)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 2.50)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[4] <= 0.72)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.23)
                    {
                        if(dFeatures[5] <= 0.16)
                        {
                            if(dFeatures[3] <= 7.50)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[5] <= 0.09)
                                {
                                    if(dFeatures[1] <= 0.02)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.02)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 16.50)
                            {
                                if(dFeatures[1] <= 0.16)
                                {
                                    if(dFeatures[3] <= 12.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.17)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 15.65)
                                {
                                    if(dFeatures[0] <= 6.52)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 254.02)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 7.78)
                        {
                            if(dFeatures[5] <= 0.22)
                            {
                                if(dFeatures[4] <= 0.72)
                                {
                                    if(dFeatures[0] <= 7.19)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 1.41)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.31)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1204.01)
                            {
                                if(dFeatures[4] <= 0.66)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[1] <= 0.00)
    {
        if(dFeatures[0] <= 364.93)
        {
            if(dFeatures[0] <= 0.26)
            {
                if(dFeatures[0] <= 0.23)
                {
                    if(dFeatures[5] <= 0.03)
                    {
                        if(dFeatures[0] <= 0.17)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.06)
                        {
                            if(dFeatures[5] <= 0.09)
                            {
                                if(dFeatures[3] <= 4.50)
                                {
                                    if(dFeatures[2] <= 2.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 5.00)
                                {
                                    if(dFeatures[0] <= 0.03)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.13)
                            {
                                if(dFeatures[0] <= 0.12)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[4] <= 0.91)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.24)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 235.69)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[0] <= 240.63)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 587.71)
            {
                return 0;
            }
            else
            {
                if(dFeatures[0] <= 901.80)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[0] <= 1262.39)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[2] <= 0.50)
        {
            if(dFeatures[4] <= 0.97)
            {
                if(dFeatures[0] <= 10.47)
                {
                    if(dFeatures[0] <= 3.77)
                    {
                        if(dFeatures[0] <= 3.56)
                        {
                            if(dFeatures[0] <= 0.36)
                            {
                                if(dFeatures[1] <= 0.04)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[5] <= 0.03)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 0.16)
                        {
                            if(dFeatures[1] <= 0.22)
                            {
                                if(dFeatures[1] <= 0.19)
                                {
                                    if(dFeatures[1] <= 0.03)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 6.47)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.04)
                    {
                        if(dFeatures[0] <= 11.22)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[0] <= 734.59)
                            {
                                if(dFeatures[1] <= 0.00)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 52.37)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 0.09)
                        {
                            if(dFeatures[0] <= 16.88)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[0] <= 42.11)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 48.26)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 0.11)
                            {
                                if(dFeatures[0] <= 49.54)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.08)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 99.08)
                                {
                                    if(dFeatures[0] <= 27.45)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 348.63)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.12)
                {
                    if(dFeatures[0] <= 0.08)
                    {
                        if(dFeatures[1] <= 0.01)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.54)
                    {
                        if(dFeatures[1] <= 0.00)
                        {
                            if(dFeatures[1] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 0.05)
                            {
                                if(dFeatures[1] <= 0.00)
                                {
                                    if(dFeatures[1] <= 0.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 8.19)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 0.08)
                                {
                                    if(dFeatures[1] <= 0.07)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.33)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 0.85)
                        {
                            if(dFeatures[1] <= 0.67)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 0.05)
            {
                if(dFeatures[1] <= 0.04)
                {
                    if(dFeatures[1] <= 0.03)
                    {
                        if(dFeatures[0] <= 0.04)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[1] <= 0.02)
                            {
                                if(dFeatures[1] <= 0.01)
                                {
                                    if(dFeatures[3] <= 15.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 5.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 230.63)
                                {
                                    if(dFeatures[1] <= 0.03)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 15.34)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[2] <= 5.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.37)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[1] <= 0.13)
                    {
                        if(dFeatures[3] <= 8.00)
                        {
                            if(dFeatures[3] <= 2.50)
                            {
                                if(dFeatures[0] <= 0.51)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.75)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 0.09)
                            {
                                if(dFeatures[0] <= 4.87)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.59)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[0] <= 11.33)
                            {
                                if(dFeatures[5] <= 0.09)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 1.14)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 360.12)
                                {
                                    if(dFeatures[2] <= 6.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP37_v3(Double *dFeatures)
{
if(dFeatures[4] <= 0.72)
{
    if(dFeatures[0] <= 2.27)
    {
        if(dFeatures[0] <= 0.62)
        {
            if(dFeatures[5] <= 0.66)
            {
                if(dFeatures[1] <= 0.04)
                {
                    if(dFeatures[3] <= 24.00)
                    {
                        if(dFeatures[3] <= 2.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[4] <= 0.50)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.06)
                    {
                        if(dFeatures[4] <= 0.53)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.56)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 0.08)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 0.20)
            {
                if(dFeatures[5] <= 0.34)
                {
                    if(dFeatures[1] <= 0.18)
                    {
                        if(dFeatures[1] <= 0.13)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 2.08)
                    {
                        if(dFeatures[0] <= 1.85)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[4] <= 0.34)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 9.00)
                {
                    if(dFeatures[0] <= 0.71)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[0] <= 1.20)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 1.70)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 0.41)
        {
            if(dFeatures[5] <= 0.28)
            {
                if(dFeatures[0] <= 206.52)
                {
                    if(dFeatures[0] <= 52.40)
                    {
                        if(dFeatures[3] <= 12.00)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[0] <= 32.71)
                {
                    if(dFeatures[5] <= 0.47)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[1] <= 0.00)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 2.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 0.16)
            {
                if(dFeatures[1] <= 0.01)
                {
                    if(dFeatures[5] <= 0.09)
                    {
                        if(dFeatures[0] <= 62.36)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 29.00)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.01)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[5] <= 0.03)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[4] <= 0.59)
                {
                    if(dFeatures[3] <= 15.50)
                    {
                        if(dFeatures[0] <= 25.59)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 17.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.29)
                    {
                        if(dFeatures[0] <= 67.75)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 11.27)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[5] <= 0.09)
    {
        if(dFeatures[1] <= 0.00)
        {
            if(dFeatures[0] <= 369.62)
            {
                if(dFeatures[0] <= 28.01)
                {
                    if(dFeatures[0] <= 2.57)
                    {
                        if(dFeatures[0] <= 2.19)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 29.79)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[4] <= 0.84)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 8.00)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 26.00)
            {
                if(dFeatures[1] <= 0.00)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[0] <= 1020.46)
                    {
                        if(dFeatures[5] <= 0.03)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 183.52)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 2.24)
        {
            if(dFeatures[1] <= 0.11)
            {
                if(dFeatures[1] <= 0.00)
                {
                    if(dFeatures[0] <= 0.24)
                    {
                        if(dFeatures[0] <= 0.18)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.00)
                    {
                        if(dFeatures[5] <= 0.16)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 0.11)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[5] <= 0.22)
                {
                    if(dFeatures[1] <= 0.13)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[1] <= 0.13)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 1.31)
                    {
                        if(dFeatures[3] <= 4.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 1.49)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 115.95)
            {
                if(dFeatures[4] <= 0.78)
                {
                    if(dFeatures[0] <= 61.63)
                    {
                        if(dFeatures[0] <= 41.26)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 10.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 34.98)
                    {
                        if(dFeatures[1] <= 0.00)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 0.16)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 0.01)
                {
                    if(dFeatures[0] <= 129.80)
                    {
                        if(dFeatures[0] <= 118.16)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 329.62)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 1.50)
                    {
                        if(dFeatures[4] <= 0.84)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 1122.29)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP22_v3(Double *dFeatures)
{
if(dFeatures[0] <= 17.50)
{
    if(dFeatures[1] <= 3.50)
    {
        if(dFeatures[2] <= 0.38)
        {
            if(dFeatures[2] <= 0.12)
            {
                if(dFeatures[1] <= 1.50)
                {
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[0] <= 5.00)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.62)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 8.50)
        {
            if(dFeatures[2] <= 0.88)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[2] <= 0.12)
            {
                if(dFeatures[1] <= 15.50)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.62)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
    }
}
else
{
    if(dFeatures[1] <= 26.50)
    {
        if(dFeatures[2] <= 0.88)
        {
            if(dFeatures[1] <= 17.50)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[1] <= 18.50)
            {
                if(dFeatures[0] <= 27.50)
                {
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[1] <= 24.50)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 37.50)
        {
            if(dFeatures[2] <= 0.38)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[1] <= 51.50)
            {
                if(dFeatures[0] <= 85.50)
                {
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[1] <= 154.50)
                {
                    return 0;
                }
                else
                {
                    return 1;
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP27_v3(Double *dFeatures)
{
if(dFeatures[1] <= 4.50)
{
    return 1;
}
else
{
    return 0;
    }
}

UInt TTrEngine::xdecide_depth1_QP32_v3(Double *dFeatures)
{
if(dFeatures[1] <= 2.50)
{
    if(dFeatures[2] <= 0.12)
    {
        if(dFeatures[1] <= 1.50)
        {
            if(dFeatures[0] <= 0.50)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(dFeatures[2] <= 0.62)
        {
            return 1;
        }
        else
        {
            return 1;
        }
    }
}
else
{
    if(dFeatures[0] <= 20.50)
    {
        if(dFeatures[1] <= 15.50)
        {
            if(dFeatures[0] <= 11.50)
            {
                if(dFeatures[0] <= 5.50)
                {
                    if(dFeatures[2] <= 0.88)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 6.50)
                    {
                        if(dFeatures[2] <= 0.62)
                        {
                            if(dFeatures[2] <= 0.12)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 5.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.62)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 11.50)
                {
                    if(dFeatures[2] <= 0.88)
                    {
                        if(dFeatures[0] <= 12.50)
                        {
                            if(dFeatures[1] <= 4.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 13.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 6.50)
                        {
                            if(dFeatures[0] <= 13.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 16.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.38)
                    {
                        if(dFeatures[1] <= 13.50)
                        {
                            if(dFeatures[1] <= 12.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 19.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 12.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.38)
            {
                if(dFeatures[1] <= 16.50)
                {
                    if(dFeatures[2] <= 0.12)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 19.50)
                    {
                        if(dFeatures[1] <= 18.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 16.50)
                {
                    if(dFeatures[2] <= 0.88)
                    {
                        if(dFeatures[2] <= 0.62)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 16.50)
                    {
                        if(dFeatures[2] <= 0.88)
                        {
                            if(dFeatures[0] <= 18.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 18.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 19.50)
                        {
                            if(dFeatures[2] <= 0.62)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.62)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 19.50)
        {
            if(dFeatures[2] <= 0.12)
            {
                if(dFeatures[0] <= 30.50)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[0] <= 34.50)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[0] <= 37.50)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.62)
                {
                    if(dFeatures[1] <= 16.50)
                    {
                        if(dFeatures[0] <= 32.50)
                        {
                            if(dFeatures[0] <= 31.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 15.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 32.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 17.50)
                    {
                        if(dFeatures[1] <= 11.50)
                        {
                            if(dFeatures[0] <= 21.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 36.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.88)
                        {
                            if(dFeatures[0] <= 42.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 29.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 60.50)
            {
                if(dFeatures[1] <= 24.50)
                {
                    if(dFeatures[2] <= 0.88)
                    {
                        if(dFeatures[0] <= 52.00)
                        {
                            if(dFeatures[0] <= 40.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 20.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 49.00)
                        {
                            if(dFeatures[0] <= 32.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 36.50)
                    {
                        if(dFeatures[0] <= 35.00)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 33.50)
                        {
                            if(dFeatures[2] <= 0.88)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 40.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.88)
                {
                    if(dFeatures[2] <= 0.38)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 37.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP37_v3(Double *dFeatures)
{
if(dFeatures[2] <= 0.12)
{
    return 1;
}
else
{
    return 1;
    }
}
