#include"GameLayer.h"



GameLayer::~GameLayer()
{

}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto layer = LayerColor::create(Color4B(49, 49, 49, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	this->addChild(layer, -1);

	_map = Node::create();
	_map->setContentSize(Size(MAP_WIDTH, MAP_HEIGHT));
	this->addChild(_map, 0);


	initPlayer();
	initBean();
	//initThorn();
	addThorn(1);
	updateView();
	return true;
}


void GameLayer::initData()
{
	initBean();
	initPlayer();
	initThorn();
}

void GameLayer::initBean()
{
	for (int i = 0; i < MAP_DIVISION_X; i++)
	{
		for (int j = 0; j < MAP_DIVISION_Y; j++)
		{
			int m = 0;
			while (m < MAX_DIVISION_BEAN_NUM)
			{
				int shape = rand() % 4 + 3;
				int color = rand() % 6 + 1;
				float x = rand_0_1();
				float y = rand_0_1();

				std::string path = StringUtils::format("bean_polygon%d_%d.png", shape, color);
				auto bean = Bean::create(path.c_str());

				bean->setPosition(Vec2(SCREEN_WIDTH*(i + x), SCREEN_HEIGHT*(j + y)));
				bean->setLocalZOrder(bean->getScore());
				_map->addChild(bean);
				_beanlist.pushBack(bean);
				m++;
			}
		}
	}
}

void GameLayer::initPlayer()  //+++++++++
{
	float x = rand() % MAP_WIDTH;
	float y = rand() % MAP_HEIGHT;
	_player = Player::create("TEST", Vec2(x, y), _map, 1);
	_player->setLocalZOrder(_player->getAllScore());
	_map->addChild(_player);
}

void GameLayer::initThorn()
{
	this->schedule(schedule_selector(GameLayer::addThorn), 5);
}

void GameLayer::addThorn(float t)
{
	static int ThornID = 0;
	auto thorn = Thorn::create("BEAN/thorn.png");
	int x = rand() % MAP_WIDTH;
	int y = rand() % MAP_HEIGHT;
	thorn->setPosition(Vec2(x, y));
	_map->addChild(thorn,thorn->getScore());
	_thornMap.insert(ThornID, thorn);
	ThornID++;
}

void GameLayer::collideBean(Player* player)
{
	Rect rect = player->getPlayerRect();
	Vec2 position = player->getPosition();

	Vec2 leftdown = Vec2(rect.origin.x - rect.size.width / 2, rect.origin.y - rect.size.height / 2);
	Vec2 rightup = Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2);

	int leftdownX = leftdown.x / MAP_DIVISION_X;
	int leftdownY = leftdown.y / MAP_DIVISION_Y;
	int rightupX = (rightup.x / MAP_DIVISION_X);
	int rightupY = (rightup.y / MAP_DIVISION_Y);

	if (leftdownX < 0) { leftdownX = 0; }
	if (leftdownY < 0) { leftdownY = 0; }
	if (rightupX > (MAP_DIVISION_X - 1)) { rightupX = MAP_DIVISION_X - 1; }
	if (rightupY > (MAP_DIVISION_Y - 1)) { rightupY = MAP_DIVISION_Y - 1; }


	for (int i = leftdownX; i <= rightupX; i++)
	{
		for (int j = leftdownY; j <= rightupY; j++)
		{
			int m = i * MAP_DIVISION_Y + j;
			for (int n = m * 100; n < (m + 1) * 100; n++)
			{
				auto bean = _beanlist.at(m);
				if (bean->isVisible())
				{
					if (player->collideBean(bean))
					{
						bean->setVisible(false);
						float t = rand() % 10 + 10;

						auto seq = Sequence::create(DelayTime::create(t), CallFunc::create(CC_CALLBACK_0(GameLayer::resetBean, this, bean)));
						bean->runAction(seq);
					}
				}
			}
		}
	}
}

void GameLayer::resetBean(Node* node)
{
	node->setVisible(true);
}

void GameLayer::collidePlayer()
{
	for (auto i : _rivalMap)
	{
		auto player = i.second;
		if (player != NULL)
		{
			if (_player->collidePlayer(player))
			{
				if (_player->getDivisionNum() == 0)
				{
					_player->resetPlayer();
					return;
				}
			}
		}
	}
}

void GameLayer::updateBean()
{
	collideBean(_player);

	for (auto i : _rivalMap)
	{
		auto player = i.second;
		if (player != NULL)
		{
			collideBean(player);
		}
	}
}

void GameLayer::updateThorn()
{
	std::vector<int> eraseVec;
	for (auto i : _thornMap)
	{
		auto thorn = i.second;
		if (thorn != NULL)
		{
			if (_player->collideThorn(thorn))
			{
				eraseVec.push_back(i.first);
			}
		}
	}
	for (auto i : eraseVec)
	{
		auto thorn = _thornMap.at(i);
		_thornMap.erase(i);
		thorn->removeFromParentAndCleanup(true);

		/*






		*/
	}
	eraseVec.clear();
}

void GameLayer::updateRival()
{
	for (auto i : _rivalMap)
	{
		auto player = i.second;
		if (player != NULL)
		{
			player->updateDivision();
		}
	}
}

void GameLayer::updatePlayer()
{
	_player->updateDivision();
}

void GameLayer::updateSpore()
{
	std::vector<int> eraseVec;
	for (auto i : _sporeMap)
	{
		auto spore = i.second;
		if (spore != NULL)
		{
			Vec2 position = spore->getPosition();
			if (position.x - SPORE_RADIUS < 0) { position.x = SPORE_RADIUS; }
			if (position.y - SPORE_RADIUS < 0) { position.y = SPORE_RADIUS; }
			if (position.x + SPORE_RADIUS > MAP_WIDTH) { position.x = MAP_WIDTH - SPORE_RADIUS; }
			if (position.y + SPORE_RADIUS > MAP_HEIGHT) { position.y = MAP_HEIGHT - SPORE_RADIUS; }
			
			spore->setPosition(position);

			if (_player->collideSpore(spore))
			{
				eraseVec.push_back(i.first);
			}
			else
			{
				for (auto j : _rivalMap)
				{
					auto player = j.second;
					if (player != NULL)
					{
						if (player->collideSpore(spore))
						{
							eraseVec.push_back(i.first);
							break;
						}
					}
				}
			}
		}
	}

	for (auto i : eraseVec)
	{
		auto spore = _sporeMap.at(i);
		_sporeMap.erase(i);
		spore->removeFromParentAndCleanup(true);

	}
	eraseVec.clear();
}

void GameLayer::updateView()//              +++++++++++++++++++++++++++++++++++
{
	auto rect = _player->getPlayerRect();

	float scaleX = SCREEN_WIDTH / (SCREEN_WIDTH + rect.size.width);
	float scaleY = SCREEN_HEIGHT / (SCREEN_HEIGHT + rect.size.height);
	_mapscale = (scaleX < scaleY) ? scaleX : scaleY;

	float dx = rect.origin.x*_mapscale - SCREEN_WIDTH / 2;
	float dy = rect.origin.y*_mapscale - SCREEN_HEIGHT / 2;

	Vec2 newPosition = Vec2(-dx, -dy);

	_map->setScale(_mapscale);
	_map->setPosition(newPosition);
}

void GameLayer::spitSpore()
{

}

void GameLayer::divide()
{

}

void GameLayer::update(float t)
{

}
