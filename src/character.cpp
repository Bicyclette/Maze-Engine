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

void Character::draw(Shader & shader, DRAWING_MODE mode)
{
	character->draw(shader, character->getAnimator()->getFinalJointTransform(), mode);
}

std::unique_ptr<AnimatedObject> & Character::get()
{
	return character;
}
