#ifndef _Spore_H_
#define _Spore_H_

#include "Entity.h"

class Spore :public Entity
{
public:
	Spore() {};
	~Spore(){};

	static Spore* create(const std::string&);
	bool init(const std::string&);
};


#endif // !_Spore_H_

