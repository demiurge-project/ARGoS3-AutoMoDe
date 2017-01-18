/*
 * File: AutoMoDeConditionMessageCount.h
 *
 */

#ifndef AUTOMODE_CONDITION_MESSAGE_COUNT_H
#define AUTOMODE_CONDITION_MESSAGE_COUNT_H

#include "AutoMoDeCondition.h"

namespace argos {
	class AutoMoDeConditionMessageCount: public AutoMoDeCondition {
		public:
			AutoMoDeConditionMessageCount();
			virtual ~AutoMoDeConditionMessageCount();

			AutoMoDeConditionMessageCount(AutoMoDeConditionMessageCount* pc_condition);
			virtual AutoMoDeConditionMessageCount* Clone();

			virtual bool Verify();
			virtual void Reset();
			virtual void Init();

		private:
			Real m_fParameterEta;
			UInt8 m_unParameterXi;
	};
}

#endif