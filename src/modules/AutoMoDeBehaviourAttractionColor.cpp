/**
  * @file <src/modules/AutoMoDeBehaviourAttractionColor.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourAttractionColor.h"


namespace argos {

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourAttractionColor::AutoMoDeBehaviourAttractionColor() {
        m_strLabel = "AttractionColor";
	}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourAttractionColor::AutoMoDeBehaviourAttractionColor(AutoMoDeBehaviourAttractionColor* pc_behaviour) {
		m_strLabel = pc_behaviour->GetLabel();
		m_bLocked = pc_behaviour->IsLocked();
		m_bOperational = pc_behaviour->IsOperational();
		m_unIndex = pc_behaviour->GetIndex();
		m_unIdentifier = pc_behaviour->GetIdentifier();
		m_mapParameters = pc_behaviour->GetParameters();
		Init();
	}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourAttractionColor::~AutoMoDeBehaviourAttractionColor() {}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourAttractionColor* AutoMoDeBehaviourAttractionColor::Clone() {
        return new AutoMoDeBehaviourAttractionColor(this);   // todo: check without *
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourAttractionColor::ControlStep() {
        CCI_EPuckOmnidirectionalCameraSensor::SReadings sReadings = m_pcRobotDAO->GetCameraInput();
        CCI_EPuckOmnidirectionalCameraSensor::TBlobList::iterator it;
        CVector2 sColVectorSum(0,CRadians::ZERO);
		CVector2 sProxVectorSum(0,CRadians::ZERO);
		CVector2 sResultVector(0,CRadians::ZERO);

        for (it = sReadings.BlobList.begin(); it != sReadings.BlobList.end(); it++) {
            if ((*it)->Color == m_cColorReceiverParameter) {
                sColVectorSum += CVector2(m_unAttractionParameter / (((*it)->Distance)+1),
                                          (*it)->Angle); // Longest distance in the arena
            }
            // TODO Check sColVectorSum function
        }

		sProxVectorSum = SumProximityReadings(m_pcRobotDAO->GetProximityInput());
        sResultVector = sColVectorSum - 6*sProxVectorSum;

		if (sResultVector.Length() < 0.1) {
			sResultVector = CVector2(1, CRadians::ZERO);
		}

		m_pcRobotDAO->SetWheelsVelocity(ComputeWheelsVelocityFromVector(sResultVector));
        m_pcRobotDAO->SetLEDsColor(m_cColorEmiterParameter);

		m_bLocked = false;
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourAttractionColor::Init() {
		std::map<std::string, Real>::iterator it = m_mapParameters.find("att");
		if (it != m_mapParameters.end()) {
			m_unAttractionParameter = it->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
        it = m_mapParameters.find("cle");
        if (it != m_mapParameters.end()) {
            m_cColorEmiterParameter = GetColorParameter(it->second);
        } else {
            LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
            THROW_ARGOSEXCEPTION("Missing Parameter");
        }
        it = m_mapParameters.find("clr");
        if (it != m_mapParameters.end()) {
            m_cColorReceiverParameter = GetColorParameter(it->second);
        } else {
            LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
            THROW_ARGOSEXCEPTION("Missing Parameter");
        }
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourAttractionColor::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourAttractionColor::ResumeStep() {
		m_bOperational = true;
	}
}