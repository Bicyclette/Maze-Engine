#include "character.hpp"

Character::Character(std::string aPath, glm::mat4 aModel, std::string aName) :
	character(std::make_unique<AnimatedObject>(aPath, aModel)),
	name(aName),
	sceneID(-1),
	walkDirection(glm::vec3(0.0f, 0.0f, 1.0f)),
	turnAngle(0.0f),
	turnSpeed(0.01f),
	walkSpeed(2.5f),
	runSpeed(7.0f)
{}

void Character::walk()
{
	if(character->getAnimator()->getCurrentAnimation() != character->getAnimations()[2])
	{
		character->getAnimator()->playAnimation(character->getAnimations()[4]);
	}
}

void Character::run()
{
	if(character->getAnimator()->getCurrentAnimation() != character->getAnimations()[2])
	{
		character->getAnimator()->playAnimation(character->getAnimations()[3]);
	}
}

void Character::jump()
{
	if(character->getAnimator()->getCurrentAnimation() != character->getAnimations()[2])
	{
		character->getAnimator()->playAnimation(character->getAnimations()[2]);
	}
}

void Character::idle()
{
	if(character->getAnimator()->getCurrentAnimation() != character->getAnimations()[2])
	{
		character->getAnimator()->playAnimation(character->getAnimations()[0]);
	}
}

void Character::setModel(glm::mat4 & m)
{
	character->setModel(m);
}

glm::mat4 Character::getModel()
{
	return character->getModel();
}

void Character::draw(Shader & shader, struct IBL_DATA * iblData, DRAWING_MODE mode)
{
	character->draw(shader, character->getAnimator()->getFinalJointTransform(), iblData, mode);
}

std::unique_ptr<AnimatedObject> & Character::get()
{
	return character;
}

std::shared_ptr<Camera> & Character::getCamera()
{
	return camera;
}

glm::vec3 Character::getPosition()
{
	glm::mat4 model = character->getModel();
	glm::vec3 scale;
	glm::quat rot;
	glm::vec3 tr;
	glm::vec3 skew;
	glm::vec4 persp;
	glm::decompose(model, scale, rot, tr, skew, persp);
	return tr;
}

glm::vec3 Character::getDirection()
{
	return walkDirection;
}
