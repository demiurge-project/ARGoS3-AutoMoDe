#include "GiandujaStopLoopFunc.h"

/****************************************/
/****************************************/

GiandujaStopLoopFunction::GiandujaStopLoopFunction() {
  m_fRadius = 0.07;
  m_cCoordSpot1 = CVector2(0.6,0.8);
  m_CCoordRect1Pos = CVector2(0.8,-0.5);
  //m_CCoordRect1 = CVector2(1.25,-0.5);

  m_CCoordRect2Pos = CVector2(-0.8,-1);
  //m_CCoordRect2 = CVector2(-1.25,-1.25);


  m_unCostI = 0;
  m_unCostO = 0;
  m_unState = 0;
  m_unTbar = 0;
  m_fObjectiveFunction = 0;

}

/****************************************/
/****************************************/

GiandujaStopLoopFunction::GiandujaStopLoopFunction(const GiandujaStopLoopFunction& orig) {}

/****************************************/
/****************************************/

GiandujaStopLoopFunction::~GiandujaStopLoopFunction() {}

/****************************************/
/****************************************/

void GiandujaStopLoopFunction::Destroy() {}

/****************************************/
/****************************************/

void GiandujaStopLoopFunction::Init(TConfigurationNode& t_tree) {
    AutoMoDeLoopFunctions::Init(t_tree);
    Real a =m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
    Real b =m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
    m_cCoordSpot1 = CVector2(-0.7+a*1.4,0.6+b*0.4);
}

argos::CColor GiandujaStopLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
  CVector2 vCurrentPoint(c_position_on_plane.GetX(), c_position_on_plane.GetY());

  Real d = (m_cCoordSpot1 - vCurrentPoint).Length();

  if (d <= m_fRadius) {
    return CColor::BLACK;
  }
  if (d <= m_fRadius+0.015) {
    return CColor::GREEN;
  }

  // if ( (vCurrentPoint.GetX()<=m_CCoordRect1Pos.GetX()) && (vCurrentPoint.GetX()>=m_CCoordRect2Pos.GetX()) && (vCurrentPoint.GetY()>=m_CCoordRect2Pos.GetY()) && (vCurrentPoint.GetY()<=m_CCoordRect1Pos.GetY()) ) {
  //   return CColor::GREEN;
  // }

  // if ( (vCurrentPoint.GetX()<=m_CCoordRect1.GetX()) && (vCurrentPoint.GetX()>=m_CCoordRect2.GetX()) && (vCurrentPoint.GetY()>=m_CCoordRect2.GetY()) && (vCurrentPoint.GetY()<=m_CCoordRect1.GetY()) ) {
  //   return CColor::WHITE;
  // }

  return CColor::GRAY50;
}


void GiandujaStopLoopFunction::PositionRobots() {
  Real a;
  Real b;

  CEPuckEntity* pcEpuck;
  UInt32 unTrials;
  bool bPlaced = false;

  for(UInt32 i = 1; i < m_unNumberRobots + 1; ++i) {
    std::ostringstream id;
    id << "epuck" << i;
    pcEpuck = new CEPuckEntity(id.str().c_str(),
                               "automode",
                               CVector3(0,0,0),
                               CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));
    AddEntity(*pcEpuck);
    // Choose position at random
    unTrials = 0;
    do {
       ++unTrials;
       a = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
       b = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));

       Real fPosX = m_CCoordRect2Pos.GetX() + a*fabs(m_CCoordRect2Pos.GetX() - m_CCoordRect1Pos.GetX());
       Real fPosY = m_CCoordRect2Pos.GetY() + b*fabs(m_CCoordRect2Pos.GetY() - m_CCoordRect1Pos.GetY());

       bPlaced = MoveEntity((*pcEpuck).GetEmbodiedEntity(),
                            CVector3(fPosX, fPosY, 0),
                            CQuaternion().FromEulerAngles(m_pcRng->Uniform(CRange<CRadians>(CRadians::ZERO,CRadians::TWO_PI)),
                            CRadians::ZERO,CRadians::ZERO),false);

    } while(!bPlaced && unTrials < 100);
    if(!bPlaced) {
       THROW_ARGOSEXCEPTION("Can't place robot #" << i);
    }
  }
//   CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
// /* Go through them */
// for(CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
//   CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
//   m_tOldPosPoints[pcEpuck] = CVector2(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(), pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());
//   }
}

/****************************************/
/****************************************/

void GiandujaStopLoopFunction::PostStep() {
    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    UInt8 un_trigger = 0;
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                         pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        Real fDistanceSpot1 = (m_cCoordSpot1 - cEpuckPosition).Length();

        if (m_unState == 0) {
            if (fDistanceSpot1 <= m_fRadius+0.02) {
                un_trigger = 1;
            }
            else if ( m_tOldPosPoints[pcEpuck] == cEpuckPosition ) { //must move
                m_unCostI+=1;
            }
            m_unTbar +=1;
        }
        else if (m_unState == 1) {
            if ( m_tOldPosPoints[pcEpuck] != cEpuckPosition ) { //must stop
                m_unCostO+=1;
            }
        }
        m_tOldPosPoints[pcEpuck] = cEpuckPosition;
    }

    if (m_unState == 0 && un_trigger == 1) {
        m_unState = 1;
        LOG << "Found Spot, triggering." << std::endl;
    }

}

/****************************************/
/****************************************/

void GiandujaStopLoopFunction::PostExperiment() {
    LOG<< "CostI :" << m_unCostI << " / CostO :" << m_unCostO << " / Tbar:" << m_unTbar << std::endl;
    LOG<< m_unCostI + m_unCostO + m_unTbar << std::endl;
    m_fObjectiveFunction = (Real) m_unCostI + m_unCostO + m_unTbar;

}

Real GiandujaStopLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

REGISTER_LOOP_FUNCTIONS(GiandujaStopLoopFunction, "gianduja_stop_loop_functions");