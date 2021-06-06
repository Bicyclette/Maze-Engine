#ifndef JOINT_HPP
#define JOINT_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <string>
#include <memory>
#include <utility>

class Joint
{
	public:
		Joint(int p_id, std::string p_name);
		int getId() const;
		std::string & getName();
		glm::mat4 & getOffsetMatrix();
		void setOffsetMatrix(glm::mat4 matrix);
		void addChild(std::shared_ptr<Joint> j);
		void printHierarchy(int level);
		std::vector<std::shared_ptr<Joint>> & getChildren();

	private:
		const int id;
		std::string name;
		glm::mat4 offsetMatrix; // from mesh space to bone space in bind pose
		std::vector<std::shared_ptr<Joint>> children;
};

#endif
