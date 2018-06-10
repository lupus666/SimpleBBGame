#ifndef _Bean_H_
#define _Bean_H_

#include"Classes\Entity\Entity.h"

class Bean : public Entity
{
public:
	Bean() {};
	~Bean() {};

	static Bean * create(const std::string&);
	bool init(const std::string&);
};


#endif // !_Bean_H_

