#include "joint.hpp"

Joint::Joint(int p_id, std::string p_name) :
	id(p_id),
	name(p_name)
{}

int Joint::getId() const
{
	return id;
}

std::string & Joint::getName()
{
	return name;
}

glm::mat4 & Joint::getOffsetMatrix()
{
	return offsetMatrix;
}

void Joint::setOffsetMatrix(glm::mat4 matrix)
{
	offsetMatrix = matrix;
}

void Joint::addChild(std::shared_ptr<Joint> j)
{
	children.push_back(j);
}

void Joint::printHierarchy(int level)
{
	if(level == 0)
	{
		std::cout << std::string(level, ' ') << name << " (" << id << ')' << std::endl;
	}

	int childrenCount = children.size();
	for(int i{0}; i < childrenCount; ++i)
	{
		for(int j{0}; j < level; ++j)
			std::cout << "|    ";
		std::cout << "|____" << children.at(i)->getName();
		std::cout << " (" << children.at(i)->getId() << ')' << std::endl;
		children.at(i)->printHierarchy(level + 1);
	}
}

std::vector<std::shared_ptr<Joint>> & Joint::getChildren()
{
	return children;
}
