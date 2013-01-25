#include "AStarNode.h"

#include <float.h>

AStarNode::AStarNode()
{
	m_fScore = FLT_MAX;
	m_gScore = FLT_MAX/2;
	m_hScore = FLT_MAX/2;
}

const float AStarNode::getFScore()
{
	m_fScore = m_gScore + m_hScore;

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