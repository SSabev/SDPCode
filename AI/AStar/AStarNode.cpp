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

const float AStarNode::getGScore()
{
	return m_gScore;
}

void AStarNode::setGScore(float gScore)
{
	m_gScore = gScore;
}

void AStarNode::setHScore(float hScore)
{
	m_hScore = hScore;
}

void AStarNode::setBias(float bias)
{
	m_bias = bias;
}

const Vector2 AStarNode::getPreviousNode()
{
	return m_previousNode;
}

void AStarNode::setPreviousNode(Vector2 previousNode)
{
	m_previousNode = previousNode;
}