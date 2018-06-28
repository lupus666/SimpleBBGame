#include"GameLayer.h"
#include"MainScene.h"


GameLayer::~GameLayer()
{
	_rivalMap.clear();
	_beanlist.clear();
	_sporeMap.clear();
	_thornMap.clear();
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


	initData();

	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	touchlistener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);

	auto keyboardlistener = EventListenerKeyboard::create();

	keyboardlistener->onKeyPressed = CC_CALLBACK_2(GameLayer::onKeyPressed, this);
	keyboardlistener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);



	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);

	this->scheduleUpdate();
	return true;
}


void GameLayer::initData()
{
	initBean();
	initPlayer();
	initThorn();
	sporeID = 0;
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

	_player = PlayerL::create(UserDefault::getInstance()->getStringForKey("playername"), Vec2(x, y), _map, rand()%8 + 1);
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

void GameLayer::collideBean(PlayerL* player)
{
	for (auto bean : _beanlist)
	{
		if (bean->isVisible())
		{
			if (player->collideBean(bean))
			{
				bean->setVisible(false);
				float time = rand() % 10 + 10;

				auto seq = Sequence::create(
					DelayTime::create(time),
					CallFunc::create(CC_CALLBACK_0(GameLayer::resetBean, this, bean)),
					NULL);
				bean->runAction(seq);
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
	_player->spitSpore(_sporeMap,sporeID);
}

void GameLayer::divide()
{
	_player->dividePlayer();
}

void GameLayer::update(float t)
{
	Client client(grpc::CreateChannel(
		"localhost:50051", grpc::InsecureChannelCredentials()));

	std::string name = _player->getName();
	playerinfo.set_name(name);
	playerinfo.set_skin(_player->getSkin());
	playerinfo.set_divisionnum(_player->getDivisionNum());
	playerinfo.set_dividetimes(_player->getdivideTimes());
	playerinfo.set_spittimes(_player->getspitTimes());
	playerinfo.set_vector_x(_player->getVector().x);
	playerinfo.set_vector_y(_player->getVector().y);

	int size = _player->getDivisionNum();
	for (int i = 0; i < size; i++) 
	{
		auto division1 = _player->getDivisionlist().at(i);
		player::PlayerDivision* division = playerinfo.add_division();
		division->set_position_x(division1->getPosition().x);
		division->set_position_y(division1->getPosition().y);
		division->set_score(division1->getScore());
		division->set_vector_x(division1->getvector().x);
		division->set_vector_y(division1->getvector().x);
	}
	MapInfo mapinfo;
	mapinfo = client.sendPlayerInfo(playerinfo);
	player::OtherPlayerInfo allrival;
	allrival = mapinfo.allrival();
	
	updateBean();
	updateRival(allrival);
	updatePlayer();
	updateSpore();
	updateThorn();
	updateView();
	collidePlayer();
}

void GameLayer::onTouchEnded(Touch* touch,Event *event)
{
	auto position = touch->getLocation();
	auto position1 = _player->getPosition();
	auto vector = _player->convertToWorldSpace(position);//!!!
	//auto vector = Vec2(position.x - position1.x, position.y - position1.y);
	//auto vector = Vec2(-1, -1);
	vector.normalize();
	_player->setVector(vector);
}

bool GameLayer::onTouchBegan(Touch* touch,Event * event)
{
	return true;
}
void GameLayer::onTouchMove(Touch*touch,Event *event)
{

}

void GameLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_D)
	{
		divide();
	}
	if (keycode == EventKeyboard::KeyCode::KEY_S)
	{
		spitSpore();
	}
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, Main::createScene()));
	}
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{

}

void GameLayer::initBean(int seed)
{
	scand(seed);
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

void GameLayer::initDataOnline()
{
	Client client(grpc::CreateChannel(
		"localhost:50051", grpc::InsecureChannelCredentials()));

	std::string name = _player->getName();
	playerinfo.set_name(name);
	playerinfo.set_skin(_player->getSkin());
	playerinfo.set_divisionnum(_player->getDivisionNum());
	playerinfo.set_dividetimes(0);
	playerinfo.set_spittimes(0);
	playerinfo.set_vector_x(0);
	playerinfo.set_vector_y(0);


	player::PlayerDivision* division = playerinfo.add_division();
	division->set_position_x(_player->getDivisionlist().at(0)->getPosition().x);
	division->set_position_y(_player->getDivisionlist().at(0)->getPosition().y);
	division->set_score(_player->getDivisionlist().at(0)->getScore());
	MapInfo mapinfo;
	mapinfo = client.sendPlayerInfo(playerinfo);



	int seed = mapinfo.beanseed();
	initBean(seed);           //豆子初始化


	player::OtherPlayerInfo allplayer;
	allplayer = mapinfo.allrival();
	initRival(allplayer);		//敌人初始化
	
}

void GameLayer::initRival(player::OtherPlayerInfo& allplayer)
{
	player::OtherPlayerInfo _allplayer;
	int size = allplayer.rival_size();
	for (int i = 0; i < size; i++)
	{
		SingePlayerInfo rivalinfo;
		rivalinfo = allplayer.rival(i);
		std::string name = rivalinfo.name();
		if (name == _player->getName())
		{
			continue;
		}
		_rivalTag.insert(rivalinfo.name(), &rivalinfo);


		float v_x = rivalinfo.vector_x();
		float v_y = rivalinfo.vector_y();
		int skinID = rivalinfo.skin();

		auto rival = PlayerL::create(name, Vec2(0, 0), Vec2(v_x, v_y), _map, skinID);
		_map->addChild(rival);
		_rivalMap.insert(name, rival);

		int size1 = rivalinfo.division_size();
		for (int j = 0; j < size1; j++)
		{
			player::PlayerDivision division;
			division = rivalinfo.division(j);
			float p_x = division.position_x();
			float p_y = division.position_y();
			int score = division.score();
			float V_x = division.vector_x();
			float V_y = division.vector_y();

			auto division1 = rival->createDivision(Vec2(p_x, p_y), Vec2(V_x, V_y), score);
			_map->addChild(division1, score);
		}

	}
}

void GameLayer::updateRival(player::OtherPlayerInfo& allplayer)
{

}