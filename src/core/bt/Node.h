/*
 * @file <src/core/bt/Node.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe-BT
 *
 * @license MIT License
 */

#ifndef NODE_H
#define NODE_H

#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/configuration/argos_exception.h>

#include <map>

#include "../../modules/AutoMoDeBehaviour.h"
#include "../../modules/AutoMoDeBehaviourAttraction.h"
#include "../../modules/AutoMoDeBehaviourAntiPhototaxis.h"
#include "../../modules/AutoMoDeBehaviourExploration.h"
#include "../../modules/AutoMoDeBehaviourPhototaxis.h"
#include "../../modules/AutoMoDeBehaviourRepulsion.h"
#include "../../modules/AutoMoDeBehaviourStop.h"
#include "../../modules/AutoMoDeCondition.h"
#include "../../modules/AutoMoDeConditionBlackFloor.h"
#include "../../modules/AutoMoDeConditionGrayFloor.h"
#include "../../modules/AutoMoDeConditionWhiteFloor.h"
#include "../../modules/AutoMoDeConditionFixedProbability.h"
#include "../../modules/AutoMoDeConditionNeighborsCount.h"
#include "../../modules/AutoMoDeConditionInvertedNeighborsCount.h"

namespace argos {
	class Node {
		protected:
			/*
			 * The type of the node.
			 */
			std::string m_strLabel;

      /*
       * List of child nodes.
       */
      std::vector<Node*> m_vecChilds;

			std::vector<AutoMoDeCondition*> m_vecConditions;
			std::vector<AutoMoDeBehaviour*> m_vecActions;

			enum ReturnState {
				SUCCESS,
				FAILURE,
        RUNNING
			};


		public:
		  virtual ~Node();

      /*
       * Cloning function.
       */
      virtual Node* Clone() = 0;

      /*
			 */
			virtual ReturnState Tick() = 0;

			/*
			 * Returns a string containing the DOT description of the node.
			 */
			virtual const std::string GetDOTDescription() = 0;

			/*
			 * Adds condition to list of conditions.
			 * If particular node should not have a condition leaf as child, throw an error!
			 */
			virtual void AddCondition(AutoMoDeCondition* pc_condition) = 0;

			/*
			 * Adds action to list of actions.
			 * If particular node should not have a action leaf as child, throw an error!
			 */
			virtual void AddAction(AutoMoDeBehaviour* pc_action) = 0;
	};
}

#endif