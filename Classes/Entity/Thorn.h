#ifndef _Thorn_H_
#define _Thorn_H_

#include "Entity.h"

class Thorn : public Entity
{
public:
	Thorn() {};
	~Thorn() {};

	static Thorn* create(const std::string&);
	bool init(const std::string&);
};



#endif // !_Thorn_H_

