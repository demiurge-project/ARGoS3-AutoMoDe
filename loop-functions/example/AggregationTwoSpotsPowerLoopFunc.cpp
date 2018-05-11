/**
  * @file <loop-functions/AggregationTwoSpotsLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @license MIT License
  */

#include "AggregationTwoSpotsPowerLoopFunc.h"

/****************************************/
/****************************************/

AggregationTwoSpotsPowerLoopFunction::AggregationTwoSpotsPowerLoopFunction() {
  m_fRadius = 0.3;
  m_cCoordSpot1 = CVector2(0.5,0);
  m_cCoordSpot2 = CVector2(-0.5,0);
  m_unScoreSpot1 = 0;
  m_unScoreSpot2 = 0;
  m_fObjectiveFunction = 0;
  m_unExpSteps = 0; // Experiment Steps
  m_bExpFinished = false;
}

/****************************************/
/****************************************/

AggregationTwoSpotsPowerLoopFunction::AggregationTwoSpotsPowerLoopFunction(const AggregationTwoSpotsPowerLoopFunction& orig) {}

/****************************************/
/****************************************/

AggregationTwoSpotsPowerLoopFunction::~AggregationTwoSpotsPowerLoopFunction() {}

/****************************************/
/****************************************/

void AggregationTwoSpotsPowerLoopFunction::Destroy() {}

/****************************************/
/****************************************/

argos::CColor AggregationTwoSpotsPowerLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
  CVector2 vCurrentPoint(c_position_on_plane.GetX(), c_position_on_plane.GetY());
  Real d = (m_cCoordSpot1 - vCurrentPoint).Length();
  if (d <= m_fRadius) {
    return CColor::BLACK;
  }
  d = (m_cCoordSpot2 - vCurrentPoint).Length();
  if (d <= m_fRadius) {
    return CColor::BLACK;
  }

  return CColor::GRAY50;
}


/****************************************/
/****************************************/

void AggregationTwoSpotsPowerLoopFunction::Reset() {
  m_fObjectiveFunction = 0;
  m_unScoreSpot1 = 0;
  m_unScoreSpot2 = 0;
  AutoMoDeLoopFunctions::Reset();
}

/****************************************/
/****************************************/

void AggregationTwoSpotsPowerLoopFunction::PostStep(){
  m_unExpSteps += 1;
  if (m_unExpSteps == (m_unMaxExpTime - m_unBatteryLife)){
    //LOG << "Max Experiment time Check" << m_unMaxExpTime << std::endl;
    m_bExpFinished = true;
  }
}
/****************************************/
/****************************************/

bool AggregationTwoSpotsPowerLoopFunction::IsExperimentFinished(){
  return m_bExpFinished;
}
/****************************************/
/****************************************/
void AggregationTwoSpotsPowerLoopFunction::PostExperiment() {

  CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
  CVector2 cEpuckPosition(0,0);
  for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
    CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
    cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                       pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

    Real fDistanceSpot1 = (m_cCoordSpot1 - cEpuckPosition).Length();
    Real fDistanceSpot2 = (m_cCoordSpot2 - cEpuckPosition).Length();
    if (fDistanceSpot1 <= m_fRadius) {
      m_unScoreSpot1 += 1;
    } else if (fDistanceSpot2 <= m_fRadius){
      m_unScoreSpot2 += 1;
    }
  }
  m_fObjectiveFunction = Max(m_unScoreSpot1, m_unScoreSpot2)/(Real) m_unNumberRobots;

  LOG << "Score = " << m_fObjectiveFunction << std::endl;
}

/****************************************/
/****************************************/

Real AggregationTwoSpotsPowerLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

/****************************************/
/****************************************/

CVector3 AggregationTwoSpotsPowerLoopFunction::GetRandomPosition() {
  Real temp;
  Real a = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
  Real  b = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
  // If b < a, swap them
  if (b < a) {
    temp = a;
    a = b;
    b = temp;
  }
  Real fPosX = b * m_fDistributionRadius * cos(2 * CRadians::PI.GetValue() * (a/b));
  Real fPosY = b * m_fDistributionRadius * sin(2 * CRadians::PI.GetValue() * (a/b));

  return CVector3(fPosX, fPosY, 0);
}

REGISTER_LOOP_FUNCTIONS(AggregationTwoSpotsPowerLoopFunction, "aggregation_two_spots_power_lf");