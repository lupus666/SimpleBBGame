#include"Player.h"
#include"PlayerDivision.h"
#include"Bean.h"
#include"Spore.h"
#include"Thorn.h"
#include"Macro.h"


Player* Player::create(const std::string& name, Vec2 position, Node* map, int skinID)
{
	Player* player = new Player();
	if (player&&player->init(name, position, map, skinID))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool Player::init(const std::string& name, Vec2 position, Node* map, int skinID)
{
	if (!Node::init())
	{
		return false;
	}

	_map = map;
	_name = name;
	_skinID = skinID;
	_state = State::NORMAL;
	auto division = this->createDivision(position,Vec2::ZERO ,PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);
	return true;
}

PlayerDivision* Player::createDivision(Vec2 position, Vec2 vector, int score)
{
	auto division = PlayerDivision::create(_name, _skinID, score);
	division->setPosition(position);
	division->setPlayerName(_name);
	division->setvector(vector);

	_divisionlist.pushBack(division);
	_divisionNum++;

	return division;
}

void Player::dividePlayer()
{
	bool flag = false;
	for (int i = 0; i < _divisionlist.size(); i++)
	{
		auto division = _divisionlist.at(i);
		if (division->getScore() >= PLAYER_MIN_DIVIDE_SCORE)
		{
			if (_divisionlist.size() + 1 > PLAYER_MAX_DIVISION)
			{
				break;
			}
		}

		//this->unscheduleAllCallbacks();
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
			CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this)),
			NULL);

		subdivision->runAction(seq);
	}
	if (flag)
	{
		this->scheduleOnce(schedule_selector(Player::setcombine), 15);
	}
}

bool Player::collideBean(Bean* bean)
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

bool Player::collideSpore(Spore* spore)
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
	return false;
}

bool Player::collideThorn(Thorn* thorn)
{
	bool flag = false;
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
				for (int i = 0; i < _divisionNum; i++)
				{
					if (_divisionNum + 1 > PLAYER_MAX_DIVISION)
					{
						break;
					}

					_state = State::DIVIDE;
					_combineEnable = false;
					flag = true;

					Vec2 vector = Vec2(cosf(6.28*i / PLAYER_MAX_DIVISION), sinf(6.28*i / PLAYER_MAX_DIVISION));
					auto division1 = Player::createDivision(position, vector, averagescore);
					Vec2 newposition = vector * (PLAYER_MIN_DIVISION_DISTANCE + division->getRadius());
					_map->addChild(division1, averagescore);

					auto seq = Sequence::create(
						EaseOut::create(MoveBy::create(0.5f, newposition), 1.8f),
						CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this)),
						NULL
					);

					division1->runAction(seq);
				}
				
			}
		}
	}
	if (flag)
	{
		this->scheduleOnce(schedule_selector(Player::setcombine), 15);
	}
}

bool Player::collidePlayer(Player* player)
{
	for (auto division : _divisionlist)
	{

	}
}