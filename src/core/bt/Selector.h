/**
  * @file <src/core/bt/Selector.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe-BT
  *
  * @license MIT License
  */

#ifndef SELECTOR_H
#define SELECTOR_H

#include "Node.h"

namespace argos {
	class Selector: public Node {
		public:
			Selector();
			Selector(Selector* pc_behaviour);
			virtual ~Selector();

			virtual ReturnState Tick();

			virtual Selector* Clone();

      virtual const std::string GetDOTDescription();

			virtual void AddCondition(AutoMoDeCondition* pc_condition);

			virtual void AddAction(AutoMoDeBehaviour* pc_action);

	};
}

#endif