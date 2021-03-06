#include"Player.h"
#include"PlayerDivision.h"
#include"Bean.h"
#include"Spore.h"
#include"Thorn.h"
#include"Macro.h"


PlayerL* PlayerL::create(const std::string& name, Vec2 position, Node* map, int skinID)
{
	PlayerL* player = new PlayerL();
	if (player&&player->init(name, position, map, skinID))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool PlayerL::init(const std::string& name, Vec2 position, Node* map, int skinID)
{
	if (!Node::init())
	{
		return false;
	}

	_map = map;
	_name = name;
	_skinID = skinID;
	_state = State::NORMAL;
	_divideTimes = 0;
	_spitTimes = 0;
	auto division = this->createDivision(position,Vec2::ZERO ,PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);
	return true;
}

PlayerL* PlayerL::create(const std::string& name, Vec2 position, Vec2 vector, Node* map, int skinID, int dTimes, int sTimes)
{
	PlayerL* player = new PlayerL();
	if (player&&player->init(name, position, vector, map, skinID, dTimes, sTimes))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool PlayerL::init(const std::string& name, Vec2 position, Vec2 vector, Node* map, int skinID,int dTimes, int sTimes)
{
	if (!Node::init())
	{
		return false;
	}

	_map = map;
	_name = name;
	_skinID = skinID;
	_state = State::NORMAL;
	_vector = vector;
	_divideTimes = dTimes;
	_spitTimes = sTimes;

	return true;
}

PlayerDivision* PlayerL::createDivision(Vec2 position, Vec2 vector, int score)
{
	auto division = PlayerDivision::create(_name, _skinID, score);
	division->setPosition(position);
	division->setPlayerName(_name);
	division->setvector(vector);

	_divisionlist.pushBack(division);
	_divisionNum++;

	return division;
}

void PlayerL::dividePlayer()
{
	_divideTimes += 1;
	bool flag = false;
	int size = _divisionlist.size();
	for (int i = 0; i < size; i++)
	{
		auto division = _divisionlist.at(i);
		if (division->getScore() >= PLAYER_MIN_DIVIDE_SCORE)
		{
			if (_divisionNum + 1 > PLAYER_MAX_DIVISION)
			{
				break;
			}


			this->unscheduleAllCallbacks();
			_state = State::DIVIDE;
			_combineEnable = false;
			flag = true;

			division->scorehalve();
			Vec2 position = division->getPosition();
			int score = division->getScore();
			auto subdivision = this->createDivision(position, _vector, score);
			_map->addChild(subdivision);

			float angle = _vector.getAngle();
			float distance = division->getRadius() + PLAYER_MIN_DIVISION_DISTANCE;
			Vec2 newposition = Vec2(distance*cosf(angle), distance*sinf(angle));

			auto seq = Sequence::create(
				EaseOut::create(MoveBy::create(0.5f, newposition), 1.8f),
				CallFunc::create(CC_CALLBACK_0(PlayerL::divideFinish, this)),
				NULL);

			subdivision->runAction(seq);
		}
	}
	if (flag)
	{
		this->scheduleOnce(schedule_selector(PlayerL::setcombine), 15);
	}
}

bool PlayerL::collideBean(Bean* bean)
{
	for (auto division : _divisionlist)
	{
		if (division != NULL)
		{
			if (division->collideBean(bean))
			{
				division->setLocalZOrder(division->getScore());
				return true;
			}
			
		}
	}
	return false;
}

bool PlayerL::collideSpore(Spore* spore)
{	
	if (_state != State::SPIT) 
	{
		for (auto division : _divisionlist)
		{
			if (division != NULL)
			{
				if (division->collideSpore(spore))
				{
					division->setLocalZOrder(division->getScore());
					return true;
				}
			}
		}
	}
	return false;
}

bool PlayerL::collideThorn(Thorn* thorn)
{
	for (auto division : _divisionlist)
	{
		if (division != NULL)
		{
			if (division->collideThorn(thorn))
			{
				int tscore = thorn->getScore();
				int dscore = division->getScore();
				int averagescore = (tscore + dscore) / (PLAYER_MAX_THORN_DIVISION + 1);
				if (averagescore >= THORN_DIVISION_SCORE)
				{
					averagescore = THORN_DIVISION_SCORE;
					dscore = dscore + tscore - THORN_DIVISION_SCORE * PLAYER_MAX_THORN_DIVISION;
					division->setScore(dscore);
					division->change();
				}
				else
				{
					division->setScore(averagescore);
					division->change();
				}

				Vec2 position = division->getPosition();
				for (int i = 0; i < PLAYER_MAX_THORN_DIVISION; i++)
				{
					if (_divisionNum + 1 > PLAYER_MAX_DIVISION)
					{
						break;
					}

					_state = State::DIVIDE;
					_combineEnable = false;

					Vec2 vector = Vec2(cosf(6.28*i / PLAYER_MAX_DIVISION), sinf(6.28*i / PLAYER_MAX_DIVISION));
					auto division1 = PlayerL::createDivision(position, vector, averagescore);
					Vec2 newposition = vector * (PLAYER_MIN_DIVISION_DISTANCE + division->getRadius());
					_map->addChild(division1, averagescore);

					auto seq = Sequence::create(
						EaseOut::create(MoveBy::create(0.5f, newposition), 1.8f),
						CallFunc::create(CC_CALLBACK_0(PlayerL::thornFinish, this)),
						NULL
					);

					division1->runAction(seq);
					
				}
				this->scheduleOnce(schedule_selector(PlayerL::setcombine), 15);
				return true;
				
			}
		}
	}
	return false;

}

bool PlayerL::collidePlayer(PlayerL* player)
{
	bool flag = false;
	for (int i = 0; i < _divisionlist.size(); i++)
	{
		auto division = _divisionlist.at(i);
		Vec2 position = division->getPosition();
		for (int j = 0; j < player->getDivisionlist().size(); j++)
		{
			float radius = division->getRadius();
			auto division1 = player->getDivisionlist().at(j);
			Vec2 position1 = division1->getPosition();
			float radius1 = division1->getRadius();
			float distance = position1.distance(position);
			if (distance <= abs(radius - radius1))
			{
				int score = division->getScore();
				int score1 = division1->getScore();
				if (score >= score1 * MIN_EAT_MULTIPLE)
				{
					division->addScore(score1);
					flag = true;
					break;
				}
				else if (score1 >= score * MIN_EAT_MULTIPLE)
				{
					_divisionlist.eraseObject(division);
					division->removeFromParentAndCleanup(true);
					_divisionNum--;
					i--;
					flag = true;
					break;
				}
			}
		}
	}
	return flag;
}

void PlayerL::updateDivision()//更新分身位置
{
	auto rect = this->getPlayerRect();
	if (_state == State::STATIC)
	{
		for (auto division : _divisionlist)
		{
			if (division != NULL) {
				Vec2 position = division->getPosition();
				Vec2 vector = rect.origin - position;
				vector.normalize();
				division->setvector(vector);
				float speed = PLAYER_STATIC_SPEED;
				float dx = vector.x*speed;
				float dy = vector.y*speed;
				Vec2 newposition = Vec2(position.x + dx, position.y + dy);
				float radius = division->getRadius();

				if (newposition.x <= radius)newposition.x = radius;
				if (newposition.x >= (MAP_WIDTH - radius))newposition.x = MAP_WIDTH - radius;
				if (newposition.y <= radius)newposition.y = radius;
				if (newposition.y >= MAP_HEIGHT - radius)newposition.y = MAP_HEIGHT - radius;

				division->setPreposition(position);
				division->setPosition(newposition);
			}
		}
	}
	if (_state == State::NORMAL || _state == State::SPIT)//
	{
		for (auto division : _divisionlist)
		{
			if (division != NULL)
			{
				Vec2 position = division->getPosition();
				Vec2 vector = division->getvector();
				float speed = division->getspeed();
				Vec2 tvector = Vec2(vector.x*speed, vector.y*speed);
				Vec2 nvector = rect.origin - position;
				nvector.normalize();
				nvector.x = nvector.x * PLAYER_MOVING_COMBINE_SPEED;
				nvector.y = nvector.y * PLAYER_MOVING_COMBINE_SPEED;
				float cosangle = cosf(Vec2::angle(tvector, nvector));
				Vec2 newposition = Vec2::ZERO;
				if (cosangle > 0 && cosangle < 1)
				{
					newposition = Vec2(position.x + tvector.x + nvector.x, position.y + tvector.y + nvector.y);
				}
				else newposition = Vec2(position.x + tvector.x, position.y + tvector.y);

				float radius = division->getRadius();

				if (newposition.x <= radius)newposition.x = radius;
				if (newposition.x >= (MAP_WIDTH - radius))newposition.x = MAP_WIDTH - radius;
				if (newposition.y <= radius)newposition.y = radius;
				if (newposition.y >= MAP_HEIGHT - radius)newposition.y = MAP_HEIGHT - radius;

				division->setPreposition(position);
				division->setPosition(newposition);
			}
		}
	}
	if (_combineEnable)
	{
		for (auto division1 : _divisionlist)
		{
			if (division1 != NULL)
			{
				Vec2 position1 = division1->getPosition();
				float radius1 = division1->getRadius();
				for (auto division2 : _divisionlist)
				{
					if (division2 != NULL && division1 != division2)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						if (position2.distance(position1) <= abs(radius2 - radius1))
						{
							_divisionNum--;
							
							_combineEnable = false;
							this->scheduleOnce(schedule_selector(PlayerL::setcombine), 8);
							int score = division1->getScore() + division2->getScore();
							if (radius1 >= radius2)
							{
								division1->addScore(score);
								_divisionlist.eraseObject(division2);
								division2->removeFromParentAndCleanup(true);
								return;
							}
							else
							{
								division2->addScore(score);
								_divisionlist.eraseObject(division1);
								division1->removeFromParentAndCleanup(true);
								return;
							}
						}
					}
				}
			}
		}
	}
	/*if (!_combineEnable)
	{
		for (auto division1 : _divisionlist)
		{
			if (division1 != NULL)
			{
				Vec2 position1 = division1->getPosition();
				float radius1 = division1->getRadius();
				for (auto division2 : _divisionlist)
				{
					if (division2 != NULL && division1 != division2)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float distance = position2.distance(position1);
						if (distance <= (radius2 + radius1))
						{
							Vec2 oldposition1 = division1->getPreposition();
							Vec2 oldposition2 = division2->getPreposition();
							float olddistance = oldposition2.distance(oldposition1);

						}
					}
				}
			}
		}
	}*/
	if (!_combineEnable)//分身会进行碰撞检测，移动后如果产生碰撞则取消移动
	{
		for (auto division1 : _divisionlist)
		{
			if (division1 != NULL)
			{
				for (auto division2 : _divisionlist)
				{
					if (division2 != NULL && division2 != division1)
					{
						Vec2 position1 = division1->getPosition();
						Vec2 position2 = division2->getPosition();
						float radius1 = division1->getRadius();
						float radius2 = division2->getRadius();
						float distance = position1.distance(position2);
						if (distance < radius1 + radius2)
						{
							Vec2 oldPosition1 = division1->getPreposition();
							Vec2 oldPosition2 = division2->getPreposition();
							float oldDistance = oldPosition1.distance(oldPosition2);
							float fixDistance = 0;
							if (oldDistance <= radius1 + radius2)
							{
								fixDistance = 2;
							}
							else
							{
								fixDistance = (radius1 + radius2 - distance);
							}

							Vec2 v = position2 - position1;
							float angle = v.getAngle();
							Vec2 newPosition1 = Vec2(position1.x - cosf(angle)*fixDistance, position1.y - sinf(angle)*fixDistance);
							Vec2 newPosition2 = Vec2(position2.x + sinf(angle)*fixDistance, position2.y + sinf(angle)*fixDistance);
							division1->setPosition(newPosition1);
						}
					}
				}
			}
		}
	}
}

void PlayerL::resetPlayer()
{
	int x = rand() % MAP_WIDTH;
	int y = rand() % MAP_HEIGHT;
	
	auto division = PlayerL::createDivision(Vec2(x, y), Vec2::ZERO, PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);
}

int PlayerL::getAllScore() 
{
	int score = 0;
	for (auto division : _divisionlist)
	{
		if (division != NULL)
		{
			score += division->getScore();
		}
	}
	return score;
}

Rect PlayerL::getPlayerRect()
{
	float left, right, up, down;

	auto division = _divisionlist.at(0);
	if (division != NULL)
	{
		auto position = division->getPosition();
		auto radius = division->getRadius();
		left = position.x - radius;
		right = position.x + radius;
		up = position.y + radius;
		down = position.y - radius;
	}
	
	for (auto division1 : _divisionlist)
	{
		if (division1 != NULL)
		{
			auto position1 = division1->getPosition();
			float radius1 = division1->getRadius();
			if (left > (position1.x - radius1))left = (position1.x - radius1);
			if (right < (position1.x + radius1))right = (position1.x + radius1);
			if (up < (position1.y + radius1)) up = position1.y + radius1;
			if (down > (position1.y - radius1))down = position1.y - radius1;
		}
	}
	
	Rect rect;
	rect.size = Size(right - left, up - down);
	rect.origin = Vec2((right + left) / 2, (up + down) / 2);
	return rect;
}

void PlayerL::spitSpore(Map<int,Spore*>& sporelist,int& sporeID)
{
	_spitTimes += 1;
	for (auto division : _divisionlist)
	{
		if (division != NULL)
		{
			
			int score = division->getScore();
			if (score >= PLAYER_MIN_SPIT_SCORE)
			{
				_state = State::SPIT;
				division->spitspore();
				Vec2 position = division->getPosition();
				Vec2 vector = division->getvector();
				float angle = vector.getAngle();
				float radius = division->getRadius();
				Vec2 position1 = Vec2(position.x + cosf(angle)*(radius+1), position.y + sinf(angle)*(radius+1));
				Spore* spore = Spore::create("SKIN/spore_1.png");
				spore->setPosition(position1);

				Vec2 dposition1 = Vec2(position1.x + SPORE_MIN_SPIT_DISTANCE * cosf(angle), position1.y + SPORE_MIN_SPIT_DISTANCE * sinf(angle));
				auto action = EaseOut::create(MoveTo::create(0.5, dposition1), 1.8f);
				auto seq1 = Sequence::create(action,CallFunc::create(CC_CALLBACK_0(PlayerL::divideFinish, this)),NULL);
				_map->addChild(spore, spore->getScore());
				float time = 0.1;
				auto seq2 = Sequence::create(
					DelayTime::create(time),
					CallFunc::create(CC_CALLBACK_0(PlayerL::divideFinish, this)),
					NULL);
				spore->runAction(seq1);
				spore->runAction(seq2);
				sporelist.insert(sporeID, spore);
				sporeID++;
			}
		}
	}
}

int PlayerL::countSpitSpore()
{
	int count = 0;
	for (auto division : _divisionlist)
	{
		if (division != NULL)
		{
			if (division->getScore() > PLAYER_MIN_SPIT_SCORE)
			{
				count += 1;
			}
		}
	}
	return count;
}

void PlayerL::setVector(Vec2 v)
{
	_state = State::NORMAL;

	for (auto division : _divisionlist)
	{
		if (division != NULL)
		{
			division->setvector(v);
		}
	}
	_vector = v;
}