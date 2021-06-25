#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "animatedObject.hpp"

class Character
{
	public:
		Character(std::string aPath, std::string aName);
		void walk();
		void run();
		void jump();

	private:
		std::string name;
		std::unique_ptr<AnimatedObject> character;
};

#endif
