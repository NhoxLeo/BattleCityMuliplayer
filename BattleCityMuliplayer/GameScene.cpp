#include "GameScene.h"
#include "GameManager.h"
#include "Tank.h"
#include "Map.h"
#include "Bullet.h"
#include <ctime>
#include "StaticSprite.h"
#include "ActiveSprite.h"
#include "StartScene.h"
#include "OverScene.h"
#include "Award.h"
//#include "Sound.h"

bool GameScene::init()
{
	//StartSound = Sound::create(_T("../sound/StartGame.mp3"));
	win = true;
	counter = 0;
	EnemyNumber = 3;
	nowEnemyNumber = 3;
	EnemyTimer = 0;
	Scene::init();
	background2 = ActiveSprite::create();
	background2->LoadTexture(_T("../Sprite/background2.jpg"), 416, 416, 1);
	background2->setPosition(D3DXVECTOR3(320, 252, 0));
	background2->setRet(Ret(Size(416, 416), background2->getPosition()));
	addActiveChild(background2);
	Tank* Enemy1 = Tank::create();
	Enemy1->setPosition(EnemyPosition1);
	Enemy1->changeAwardAble();
	addActiveChild(Enemy1);
	Tank* Enemy2 = Tank::create();
	Enemy2->setPosition(EnemyPosition2);
	addActiveChild(Enemy2);
	Tank* Enemy3 = Tank::create();
	Enemy3->setPosition(EnemyPosition3);
	addActiveChild(Enemy3);

	MyTank1 = Tank::create();
	MyTank1->setPlayer(1);
	MyTank1->setCamp(true);
	MyTank1->setPosition(D3DXVECTOR3(272, 444, 0));
	addActiveChild(MyTank1);
	if (GameManager::getInstance()->getPlayer() == 2)
	{
		MyTank2 = Tank::create();
		MyTank2->setPlayer(2);
		MyTank2->setCamp(true);
		MyTank2->setPosition(D3DXVECTOR3(368, 444, 0));
		addActiveChild(MyTank2);
	}

	setScene();

	if (GameManager::getInstance()->getMapMake())
	{
		MapManager::getInstance()->LoadMap(this, MapNumber + 1);
		LoadMap();
	}
	else
	{
		MapManager::getInstance()->LoadMap(this, GameManager::getInstance()->getLevel());
		LoadMap();
	}
	//StartSound->Play();
	return true;
}

void GameScene::setPlayerTank()
{
	bool x1 = true;
	bool x2 = true;
	if (GameManager::getInstance()->getMyLife(1)<1)
	{
		x1 = false;
	}
	if (GameManager::getInstance()->getMyLife(2)<1)
	{
		x2 = false;
	}
	for (int t = 0; t < TankArray::getInstance()->getNumber();t++)
	{
		if (TankArray::getInstance()->getTankArray()[t]->getPlayer() == 1)
		{
			x1 = false;
		}
		if (TankArray::getInstance()->getTankArray()[t]->getPlayer() == 2)
		{
			x2 = false;
		}
	}
	if (x1)
	{
		MyTank1 = Tank::create();
		MyTank1->setPlayer(1);
		MyTank1->setCamp(true);
		MyTank1->setPosition(D3DXVECTOR3(272, 444, 0));
		addActiveChild(MyTank1);
		GameManager::getInstance()->LostLife(1);
	}
	if (x2&&(GameManager::getInstance()->getPlayer() == 2))
	{
		MyTank2 = Tank::create();
		MyTank2->setPlayer(2);
		MyTank2->setCamp(true);
		MyTank2->setPosition(D3DXVECTOR3(368, 444, 0));
		addActiveChild(MyTank2);
		GameManager::getInstance()->LostLife(2);
	}
}

void GameScene::setScene()
{
	playerSign_R_1 = ActiveSprite::create();
	playerSign_R_1->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
	playerSign_R_1->setRenderRet(14, 28, 28, 0);
	playerSign_R_1->setPosition(D3DXVECTOR3(564, 337, 0));
	playerSign_R_1->setRet(Ret(Size(14, 28), playerSign_R_1->getPosition()));
	addActiveChild(playerSign_R_1);

	playerSign_T_1 = ActiveSprite::create();
	playerSign_T_1->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
	playerSign_T_1->setRenderRet(14, 28, 28, 0);
	playerSign_T_1->setPosition(D3DXVECTOR3(30, 17, 0));
	playerSign_T_1->setRet(Ret(Size(14, 28), playerSign_T_1->getPosition()));
	addActiveChild(playerSign_T_1);

	playerIco_1 = ActiveSprite::create();
	playerIco_1->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
	playerIco_1->setRenderRet(14, 14, 14, 0);
	playerIco_1->setPosition(D3DXVECTOR3(557, 353, 0));
	playerIco_1->setRet(Ret(Size(14, 14), playerIco_1->getPosition()));
	addActiveChild(playerIco_1);

	for (int i = 0; i < 6; i++)
	{
		ActiveSprite* op = ActiveSprite::create();
		player_1_grade.push_back(op);
		player_1_grade[i]->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
		player_1_grade[i]->setRenderRet(14, 14, 0, 0);
		player_1_grade[i]->setPosition(D3DXVECTOR3(60 + 14 * i, 17, 0));
		player_1_grade[i]->setRet(Ret(Size(14, 14), player_1_grade[i]->getPosition()));
		addActiveChild(player_1_grade[i]);
	}

	player_1_blood = ActiveSprite::create();
	player_1_blood->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
	player_1_blood->setPosition(D3DXVECTOR3(571, 353, 0));
	player_1_blood->setRet(Ret(Size(14, 14), player_1_blood->getPosition()));
	addActiveChild(player_1_blood);

	if (GameManager::getInstance()->getPlayer() == 2)
	{
		playerSign_R_2 = ActiveSprite::create();
		playerSign_R_2->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		playerSign_R_2->setRenderRet(14, 28, 56, 0);
		playerSign_R_2->setPosition(D3DXVECTOR3(564, 375, 0));
		playerSign_R_2->setRet(Ret(Size(14, 28), playerSign_R_2->getPosition()));
		addActiveChild(playerSign_R_2);

		playerSign_T_2 = ActiveSprite::create();
		playerSign_T_2->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		playerSign_T_2->setRenderRet(14, 28, 56, 0);
		playerSign_T_2->setPosition(D3DXVECTOR3(500, 17, 0));
		playerSign_T_2->setRet(Ret(Size(14, 28), playerSign_T_2->getPosition()));
		addActiveChild(playerSign_T_2);

		playerIco_2 = ActiveSprite::create();
		playerIco_2->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
		playerIco_2->setRenderRet(14, 14, 14, 0);
		playerIco_2->setPosition(D3DXVECTOR3(557, 391, 0));
		playerIco_2->setRet(Ret(Size(14, 14), playerIco_2->getPosition()));
		addActiveChild(playerIco_2);

		for (int i = 0; i < 6; i++)
		{
			ActiveSprite* op = ActiveSprite::create();
			player_2_grade.push_back(op);
			player_2_grade[i]->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
			player_2_grade[i]->setRenderRet(14, 14, 0, 0);
			player_2_grade[i]->setPosition(D3DXVECTOR3(530 + 14 * i, 17, 0));
			player_2_grade[i]->setRet(Ret(Size(14, 14), player_2_grade[i]->getPosition()));
			addActiveChild(player_2_grade[i]);
		}

		player_2_blood = ActiveSprite::create();
		player_2_blood->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
		player_2_blood->setPosition(D3DXVECTOR3(571, 391, 0));
		player_2_blood->setRet(Ret(Size(14, 14),player_2_blood->getPosition()));
		addActiveChild(player_2_blood);
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ActiveSprite* op;
			op = ActiveSprite::create();
			op->LoadTexture(_T("../Sprite/misc.bmp"), 84, 14, 1);
			op->setRenderRet(14, 14, 0, 0);
			op->setPosition(D3DXVECTOR3(557 + j * 14, 60 + i * 14, 0));
			op->setRet(Ret(Size(14, 14), op->getPosition()));
			NumberOfEnemy.push_back(op);
			addActiveChild(op);
		}
	}

	flag = ActiveSprite::create();
	flag->LoadTexture(_T("../Sprite/flag.bmp"), 32, 32, 1);
	flag->setPosition(D3DXVECTOR3(566, 424, 0));
	flag->setRet(Ret(Size(14, 28), flag->getPosition()));
	addActiveChild(flag);

	for (int i = 0; i < 2; i++)
	{
		ActiveSprite* op = ActiveSprite::create();
		levelArray.push_back(op);
		levelArray[i]->LoadTexture(_T("../Sprite/num.bmp"), 140, 14, 1);
		levelArray[i]->setRenderRet(14, 14, 0 + 14 * i, 0);
		levelArray[i]->setPosition(D3DXVECTOR3(590 + 14 * i, 443, 0));
		levelArray[i]->setRet(Ret(Size(14, 14), levelArray[i]->getPosition()));
		addActiveChild(levelArray[i]);
	}
	for (int i = 0; i < 17; i++)
	{
		NumberOfEnemy[i]->OnShow();
	}
	NumberOfEnemy[17]->StopShow();
	NumberOfEnemy[18]->StopShow();
	NumberOfEnemy[19]->StopShow();
}

void GameScene::Update()
{
	if (nowAward != NULL)
	{
		nowAward->Update();
	}
	if (checkEnemy() == 0 && EnemyNumber == 20)
	{
		if (GameManager::getInstance()->getMapMake())
		{
			MapManager::getInstance()->removeLast();
			GameManager::getInstance()->changeMapMake();
		}
		resetScene();
	}
	setPlayerTank();
	if (win == false || (MyTank1 == NULL&&MyTank2 == NULL))
	{
		if (GameManager::getInstance()->getMapMake())
		{
			MapManager::getInstance()->removeLast();
			GameManager::getInstance()->changeMapMake();
		}
		OverScene* scene = OverScene::create();
 		GameManager::getInstance()->setScene(scene);
		release();
		return;
	}
	EnemyControl();
	MyTank1->setSpeed(Speed(0, 0));
	if (GameManager::getInstance()->getClick3() == ESCBUTTON_UP)
	{
		PostQuitMessage(0);
		/*StartScene* scene = StartScene::create();
		GameManager::getInstance()->setScene(scene);
		release();
		return;*/
	}
	if (GameManager::getInstance()->getClick1() == ABUTTON_ON)
	{
		MyTank1->setSpeed(Speed(-1, 0));
		MyTank1->setDirection(D3DXVECTOR3(-1, 0, 0));
	}
	if (GameManager::getInstance()->getClick1() == SBUTTON_ON)
	{
		MyTank1->setSpeed(Speed(0, 1));
		MyTank1->setDirection(D3DXVECTOR3(0, 1, 0));
	}
	if (GameManager::getInstance()->getClick1() == WBUTTON_ON)
	{
		MyTank1->setSpeed(Speed(0, -1));
		MyTank1->setDirection(D3DXVECTOR3(0, -1, 0));
	}
	if (GameManager::getInstance()->getClick1() == DBUTTON_ON)
	{
		MyTank1->setSpeed(Speed(1, 0));
		MyTank1->setDirection(D3DXVECTOR3(1, 0, 0));
	}
	if (GameManager::getInstance()->getClick2() == SPACEBUTTON_ON)
	{
		MyTank1->fire();
	}
	if (GameManager::getInstance()->getPlayer() == 2)
	{
		MyTank2->setSpeed(Speed(0, 0));
		if (GameManager::getInstance()->getClick3() == LEFTBUTTON_ON)
		{
			MyTank2->setSpeed(Speed(-1, 0));
			MyTank2->setDirection(D3DXVECTOR3(-1, 0, 0));
		}
		if (GameManager::getInstance()->getClick3() == DOWNBUTTON_ON)
		{
			MyTank2->setSpeed(Speed(0, 1));
			MyTank2->setDirection(D3DXVECTOR3(0, 1, 0));
		}
		if (GameManager::getInstance()->getClick3() == UPBUTTON_ON)
		{
			MyTank2->setSpeed(Speed(0, -1));
			MyTank2->setDirection(D3DXVECTOR3(0, -1, 0));
		}
		if (GameManager::getInstance()->getClick3() == RIGHTBUTTON_ON)
		{
			MyTank2->setSpeed(Speed(1, 0));
			MyTank2->setDirection(D3DXVECTOR3(1, 0, 0));
		}
		if (GameManager::getInstance()->getClick4() == NUMBERZEROBUTTON_ON||GameManager::getInstance()->getClick4() == ENTERBUTTON_ON)
		{
			MyTank2->fire();
		}
	}

	TankArray::getInstance()->VisitAll();
	BulletArray::getInstance()->VisitAll();
	nowEnemyNumber = checkEnemy();
	if (GameManager::getInstance()->getPlayer() == 1)
	{
		if (EnemyTimer > 60*EnemyTime)
		{
			if (nowEnemyNumber<3)
			{
				if ((3 - nowEnemyNumber)>(20-EnemyNumber))
				{
					setEnemy(20 - EnemyNumber);
				}
				else
				{
					setEnemy(3 - nowEnemyNumber);
				}
			}
			EnemyTimer = 0;
		}
		else
		{
			EnemyTimer++;
		}
	}
	if (GameManager::getInstance()->getPlayer() == 2)
	{
		if (EnemyTimer > 60 * EnemyTime)
		{
			if (nowEnemyNumber < 5)
			{
				if ((5 - nowEnemyNumber) > (20 - EnemyNumber))
				{
					setEnemy(20 - EnemyNumber);
				}
				else
				{
					setEnemy(5 - nowEnemyNumber);
				}
			}
			EnemyTimer = 0;
		}
		else
		{
			EnemyTimer++;
		}
	}

	player_1_blood->setRenderRet(14, 14, 14 * GameManager::getInstance()->getMyLife(1), 0);
	if (GameManager::getInstance()->getPlayer() == 2)
	{
		player_2_blood->setRenderRet(14, 14, 14 * GameManager::getInstance()->getMyLife(2), 0);
	}

	{
		int level_decade = GameManager::getInstance()->getLevel() / 10;
		int level_unit = GameManager::getInstance()->getLevel() % 10;
		levelArray[0]->setRenderRet(14, 14, 14 * level_decade, 0);
		levelArray[1]->setRenderRet(14, 14, 14 * level_unit, 0);
	}

	{
		n = 100000;
		Grade_1 = GameManager::getInstance()->getGrade(1);
		if (GameManager::getInstance()->getPlayer() == 2)
		{
			Grade_2 = GameManager::getInstance()->getGrade(2);
		}
		for (int i = 0; i < 6; i++)
		{
			j_1 = Grade_1 / n;
			Grade_1 = Grade_1%n;
			player_1_grade[i]->setRenderRet(14, 14, 14 * j_1, 0);

			if (GameManager::getInstance()->getPlayer() == 2)
			{
				j_2 = Grade_2 / n;
				Grade_2 = Grade_2%n;
				player_2_grade[i]->setRenderRet(14, 14, 14 * j_2, 0);
			}
			n = n / 10;
		}
	}
}

void GameScene::setEnemy(int number)
{
	Tank* enemy;
	srand(time(0));
	for (int t = 0; t < number;t++)
	{
		switch (rand() % 5)
		{
		case 0:
			if (!checkTank(EnemyPosition1))
			{
				enemy = Tank::create();
				int x = rand() % AwardNumber;
				if (x == 0)
				{
					enemy->changeAwardAble();
				}
				enemy->setLevel(rand() % 3 + 1);
				enemy->setPosition(EnemyPosition1);
				addActiveChild(enemy);
				t++;
				EnemyNumber++;
				NumberOfEnemy[20 - EnemyNumber]->StopShow();
			}
			break;
		case 1:
			if (!checkTank(EnemyPosition2))
			{
				enemy = Tank::create();
				int x = rand() % AwardNumber;
				if (x == 0)
				{
					enemy->changeAwardAble();
				}
				enemy->setLevel(rand() % 3 + 1);
				enemy->setPosition(EnemyPosition2);
				addActiveChild(enemy);
				t++;
				EnemyNumber++;
				NumberOfEnemy[20 - EnemyNumber]->StopShow();
			}
			break;
		case 2:
			if (!checkTank(EnemyPosition3))
			{
				enemy = Tank::create();
				int x = rand() % AwardNumber;
				if (x == 0)
				{
					enemy->changeAwardAble();
				}
				enemy->setLevel(rand() % 3 + 1);
				enemy->setPosition(EnemyPosition3);
				addActiveChild(enemy);
				t++;
				EnemyNumber++;
				NumberOfEnemy[20 - EnemyNumber]->StopShow();
			}
			break;
		case 3:
			if (!checkTank(EnemyPosition4))
			{
				enemy = Tank::create();
				int x = rand() % AwardNumber;
				if (x == 0)
				{
					enemy->changeAwardAble();
				}
				enemy->setLevel(rand() % 3 + 1);
				enemy->setPosition(EnemyPosition4);
				addActiveChild(enemy);
				t++;
				EnemyNumber++;
				NumberOfEnemy[20 - EnemyNumber]->StopShow();
			}
			break;
		case 4:
			if (!checkTank(EnemyPosition5))
			{
				enemy = Tank::create();
				int x = rand() % AwardNumber;
				if (x == 0)
				{
					enemy->changeAwardAble();
				}
				enemy->setLevel(rand() % 3 + 1);
				enemy->setPosition(EnemyPosition5);
				addActiveChild(enemy);
				t++;
				EnemyNumber++;
				NumberOfEnemy[20 - EnemyNumber]->StopShow();
			}
			break;
		}
	}
}

int GameScene::checkEnemy()
{
	int x = 0;
	for (int t = 0; t < TankArray::getInstance()->getNumber();t++)
	{
		if (!TankArray::getInstance()->getTankArray()[t]->getCamp())
		{
			x++;
		}
	}
	return x;
}

bool GameScene::checkTank(D3DXVECTOR3 po)
{
	for (int i = 0; i < TankArray::getInstance()->getNumber(); i++)
	{
		if (Ret(Size(28, 28), po).Collision(TankArray::getInstance()->getTankArray()[i]->getRet()))
		{
			return true;
		}
	}
	return false;
}

void GameScene::EnemyControl()
{
	for (int i = 0; i < TankArray::getInstance()->getNumber(); i++)
	{
		if (!(TankArray::getInstance()->getTankArray()[i]->getCamp()))
		{
			if (YESFIRE)
			{
				TankArray::getInstance()->getTankArray()[i]->fire();
			}
			if ((TankArray::getInstance()->getTankArray()[i]->getTimer() % 180) == 0 ||
				(TankArray::getInstance()->getTankArray()[i]->getSpeed().x == 0 &&
				TankArray::getInstance()->getTankArray()[i]->getSpeed().y == 0))
			{
				int weight = rand() % 100 + 1;


				if (weight >= 0 && weight <= 20)//setDirection(Up)
					TankArray::getInstance()->getTankArray()[i]->setDirection(D3DXVECTOR3(0, -1, 0));
				if (weight >20 && weight <= 45)//setDirection(Left)
					TankArray::getInstance()->getTankArray()[i]->setDirection(D3DXVECTOR3(-1, 0, 0));
				if (weight > 45 && weight <= 75)//setDirection(Right)
					TankArray::getInstance()->getTankArray()[i]->setDirection(D3DXVECTOR3(1, 0, 0));
				if (weight > 75 && weight <= 100)//setDirection(Down)
					TankArray::getInstance()->getTankArray()[i]->setDirection(D3DXVECTOR3(0, 1, 0));
				if (TankArray::getInstance()->getTankArray()[i]->getLevel() == 2)
				{
					TankArray::getInstance()->getTankArray()[i]->setSpeed(Speed(2*TankArray::getInstance()->getTankArray()[i]->getDirection().x,
						2*TankArray::getInstance()->getTankArray()[i]->getDirection().y));
				}
				else
				{
					TankArray::getInstance()->getTankArray()[i]->setSpeed(Speed(TankArray::getInstance()->getTankArray()[i]->getDirection().x,
						TankArray::getInstance()->getTankArray()[i]->getDirection().y));
				}
			}
		}
	}
}

float GameScene::getWeight(D3DXVECTOR3 op, D3DXVECTOR3 dir)
{
	D3DXVECTOR3 NewPosition((op.x + 16*dir.x), (op.y + 16*dir.y),0);
	if (NewPosition.x < 128 ||
		NewPosition.x > 512 ||
		NewPosition.y < 60 ||
		NewPosition.y > 446 ||
		nowMap->getMap()[(int)((26*NewPosition.x-128)/16+(NewPosition.y-44)/16)] == 2 ||
		nowMap->getMap()[(int)((26 * NewPosition.x - 128) / 16 + (NewPosition.y - 44) / 16)] == 4)
	{
		return 0;
	}
	else if (nowMap->getMap()[(int)((26 * NewPosition.x - 128) / 16 + (NewPosition.y - 44) / 16)] == 1)
	{
		return getWeight(NewPosition, dir) + 0.8;
	}
	else
	{
		return getWeight(NewPosition, dir) + 1;
	}
}

void GameScene::setMap(Map* map)
{
	nowMap = map;
}

void GameScene::LoadMap()
{
	D3DXVECTOR3 SpritePosition = D3DXVECTOR3(120, 52, 0);
	for (int i = 0; i < 26; i++)
	{
		SpritePosition = D3DXVECTOR3(120, 16 * i + 52, 0);
		for (int k = 0; k < 26; k++)
		{
			if (nowMap->getMap()[i * 26 + k])
			{
				StaticSprite* sp = StaticSprite::Create(nowMap->getMap()[i * 26 + k]);
				addStaticChild(sp);
				sp->setPosition(SpritePosition);
				sp->setRet(Ret(Size(16, 16), sp->getPosition()));
				if (nowMap->getMap()[i * 26 + k] == 6||
					nowMap->getMap()[i * 26 + k] == 7||
					nowMap->getMap()[i * 26 + k] == 8||
					nowMap->getMap()[i * 26 + k] == 9)
				{
					home.push_back(sp);
				}
			}
			SpritePosition += D3DXVECTOR3(16, 0, 0);
		}
	}
}

void GameScene::clear()
{
	for (int i = 0; i < StaticNumber;i++)
	{
		Static_children[i]->release();
	}
}

void GameScene::resetScene()
{
	clear();
	GameManager::getInstance()->setLevel(GameManager::getInstance()->getLevel() + 1);
	MapManager::getInstance()->LoadMap(this, GameManager::getInstance()->getLevel());
	LoadMap();
	if (MyTank1!=NULL)
	{
		MyTank1->setPosition(D3DXVECTOR3(272, 444, 0));
		MyTank1->setDirection(D3DXVECTOR3(0, -1, 0));
	}
	if (MyTank2!=NULL)
	{
		MyTank2->setPosition(D3DXVECTOR3(368, 444, 0));
		MyTank2->setDirection(D3DXVECTOR3(0, -1, 0));
	}
	counter = 0;
	EnemyNumber = 3;
	nowEnemyNumber = 3;
	EnemyTimer = 0;
}

GameScene::~GameScene()
{
//	levelArray.~vector();
//	player_1_grade.~vector();
//	player_2_grade.clear();
//	NumberOfEnemy.clear();
}