#include "AStarNode.h"

#include <float.h>

AStarNode::AStarNode()
{
	m_fScore = FLT_MAX;
	m_gScore = FLT_MAX/2;
	m_hScore = FLT_MAX/2;
	m_bias = 0;
}

const float AStarNode::getFScore()
{
	m_fScore = m_gScore + m_hScore + m_bias;

	return m_fScore;
}

/*!
 * Returns the cost of reaching this node from the start position.
 */
const float AStarNode::getGScore()
{
	return m_gScore;
}

/*!
 * Sets the cost of reaching this node from the start position.
 */
void AStarNode::setGScore(float gScore)
{
	m_gScore = gScore;
}

/*!
 * Sets the heuristic value (usually absolute distance) from this node to the goal node.
*/
void AStarNode::setHScore(float hScore)
{
	m_hScore = hScore;
}

/*!
 * Sets bias on the node. A positive bias makes it less attractive to the A* algorithm, a negative bias makes 
 * it more attractive.
 *
 * This is useful for specifying positions you don't want the robot to travel through - for example, the 
 * location of obstacles.
*/
void AStarNode::setBias(float bias)
{
	m_bias = bias;
}

/*!
 * Returns the Vector2 position which led to this node.
*/
const Vector2 AStarNode::getPreviousNode()
{
	return m_previousNode;
}

/*!
 * Sets the Vector2 position which led to this node.
*/
void AStarNode::setPreviousNode(Vector2 previousNode)
{
	m_previousNode = previousNode;
}