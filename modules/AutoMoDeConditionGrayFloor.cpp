/*
 * File: AutoMoDeConditionGrayFloor.cpp
 *
 */

 #include "AutoMoDeConditionGrayFloor.h"

 namespace argos {

	AutoMoDeConditionGrayFloor::AutoMoDeConditionGrayFloor() {
		m_strLabel = "GrayFloor";
	}

	AutoMoDeConditionGrayFloor::~AutoMoDeConditionGrayFloor() {}

	AutoMoDeConditionGrayFloor::AutoMoDeConditionGrayFloor(AutoMoDeConditionGrayFloor* pc_condition) {
		m_strLabel = pc_condition->GetLabel();
		m_unIndex = pc_condition->GetIndex();
		m_unIdentifier = pc_condition->GetIndex();
		m_unFromBehaviourIndex = pc_condition->GetOrigin();
		m_unToBehaviourIndex = pc_condition->GetExtremity();
		m_mapParameters = pc_condition->GetParameters();
	}

	AutoMoDeConditionGrayFloor* AutoMoDeConditionGrayFloor::Clone() {
		return new AutoMoDeConditionGrayFloor(*this);
	}

	bool AutoMoDeConditionGrayFloor::Verify() {
		return true;
	}
	
	void AutoMoDeConditionGrayFloor::Init() {
		std::map<std::string, Real>::iterator it = m_mapParameters.find("p");
		if (it != m_mapParameters.end()) {
			m_fProbability = it->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}


	void AutoMoDeConditionGrayFloor::Reset() {

	}

 }
