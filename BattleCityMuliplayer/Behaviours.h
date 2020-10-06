#pragma once

struct Behaviour
{
	GameObject* gameObject = nullptr;
	bool isServer = true;

	virtual void start() { }

	virtual void update() { }

	virtual void onInput(const InputController& input) { }

	virtual void onMouse(const MouseController& mouse) { }

	//virtual void onCollisionTriggered(Collider &c1, Collider &c2) { }

	enum networkMessageType
	{
		UPDATE_POSITION,
		UPDATE_TEXTURE,
		UPDATE_ALPHA,
		UPDATE_ANIMATION,
		DESTROY
	};

	void NetworkCommunication(networkMessageType type, GameObject* object)
	{
		if (isServer)
		{
			switch (type)
			{
			case Behaviour::UPDATE_POSITION:
				NetworkUpdate(object, ReplicationAction::Update_Position);
				break;
			case Behaviour::UPDATE_TEXTURE:
				NetworkUpdate(object, ReplicationAction::Update_Texture);
				break;
			case Behaviour::UPDATE_ALPHA:
				NetworkUpdate(object, ReplicationAction::Update_Alpha);
				break;
			case Behaviour::UPDATE_ANIMATION:
				NetworkUpdate(object, ReplicationAction::Update_Animation);
				break;
			case Behaviour::DESTROY:
				NetworkDestroy(object);
				break;
			}
		}
	}
};



struct Player : public Behaviour
{
	float shotingDelay = 0.5f;
	float lastShotTime = 0.0f;

	D3DXVECTOR3 bullet_offset = D3DXVECTOR3(10.0f, 30.0f, 0);

	bool isDown = false;
	float rezTime = 3.0f;
	float rezDuration = 0.0f;

	int detectedPlayers = 0;

	float spawnTime = 0.0f;
	float immunityDuration = 5.0f;
	float blinkTime = 0.5f;
	bool spawning = true;

	GameObject* rez = nullptr;

	D3DXVECTOR3 laser_offset = D3DXVECTOR3(10.0f, 500.0f, 0);
	GameObject* laser = nullptr;

	void start() override
	{
		//gameObject->tag = (uint32)(Random.next() * UINT_MAX);
		spawnTime = Time.time;
		spawning = true;
	}

	void update() override
	{
		float reviveSpeed = 1.0f;
		if (detectedPlayers > 0)
			reviveSpeed = 0.5f + detectedPlayers * 0.5f;
		/*if (rez != nullptr)
		{
			rez->animation->spriteDuration = (rezTime / (rez->animation->sprites.size() - 1)) / reviveSpeed;
			NetworkCommunication(UPDATE_ANIMATION, rez);
		}*/
		//if (spawning)
		//{
		//	if (Time.time - spawnTime >= immunityDuration)
		//	{
		//		spawning = false;
		//		gameObject->color.a = 1.0f;
		//		NetworkCommunication(UPDATE_ALPHA, gameObject);
		//	}
		//	else
		//	{
		//		gameObject->color.a = abs(sin((Time.time - spawnTime) / blinkTime * PI));
		//	}
		//}
		if (isDown && detectedPlayers == 0)
		{
			rezDuration = 0.0f;
			if (rez != nullptr)
			{
				NetworkCommunication(DESTROY, rez);
				rez = nullptr;
			}
		}
		if (detectedPlayers > 0)
		{
			rezDuration += Time.deltaTime * reviveSpeed;
		}
		detectedPlayers = 0;
		if (isDown && rezDuration > rezTime)
		{
			isDown = false;
			rezDuration = 0.0f;
			//gameObject->texture = App->modResources->robot;
			gameObject->size = D3DXVECTOR3(43, 49, 0);
			gameObject->order = 3;
			//NetworkCommunication(UPDATE_TEXTURE, gameObject);
			NetworkCommunication(DESTROY, rez);
			rez = nullptr;
		}
		if (laser != nullptr)
		{
			updateLaser();
		}
	}

	void updateLaser()
	{
		//laser->angle = gameObject->angle;
		//vec2 forward = vec2FromDegrees(gameObject->angle);
		//vec2 right = { -forward.y, forward.x };
		//laser->position = gameObject->position + laser_offset.x * right + laser_offset.y * forward;
	}

	void onInput(const InputController& input) override
	{
		if (!isDown)
		{
			if (input.horizontalAxis != 0.0f || input.verticalAxis != 0.0f)
			{
				const float advanceSpeed = 200.0f;
				if (gameObject->position.x < 905 && input.horizontalAxis > 0.0f) /*Limit on the east*/
				{
					gameObject->position += D3DXVECTOR3(1, 0, 0) * input.horizontalAxis * advanceSpeed * Time.deltaTime;
				}
				else if (gameObject->position.x > -900 && input.horizontalAxis < 0.0f)/*Limit on the west*/
				{
					gameObject->position += D3DXVECTOR3(1, 0, 0) * input.horizontalAxis * advanceSpeed * Time.deltaTime;
				}

				if (gameObject->position.y > -1015 && input.verticalAxis > 0.0f) /*Limit on the north*/
				{
					gameObject->position += D3DXVECTOR3(0, -1, 0) * input.verticalAxis * advanceSpeed * Time.deltaTime;
				}
				else if (gameObject->position.y < 1005 && input.verticalAxis < 0.0f) /*Limit on the south*/
				{
					gameObject->position += D3DXVECTOR3(0, -1, 0) * input.verticalAxis * advanceSpeed * Time.deltaTime;
				}



				NetworkCommunication(UPDATE_POSITION, gameObject);
			}
		}
	}

	void onMouse(const MouseController& mouse) override
	{
		if (!isDown)
		{
			/*gameObject->angle = degreesFromRadians(atan2(mouse.y, mouse.x)) + 90;
			NetworkCommunication(UPDATE_POSITION, gameObject);

			if (!spawning && isServer && mouse.buttons[0] == ButtonState::Pressed && Time.time - lastShotTime > shotingDelay)
			{
				lastShotTime = Time.time;

				GameObject* bullet = App->modNetServer->spawnBullet(gameObject, bullet_offset);
				bullet->clientInstanceNID = gameObject->networkId;
				bullet->tag = gameObject->tag;
			}*/
		}
	}

	/*void onCollisionTriggered(Collider& c1, Collider& c2) override
	{
		if (c2.type == ColliderType::Zombie && c2.gameObject->tag != gameObject->tag)
		{
			if (!isDown && !spawning)
			{
				isDown = true;
				gameObject->texture = App->modResources->dead;
				gameObject->size = { 66,85 };
				gameObject->order = 1;
				NetworkCommunication(UPDATE_TEXTURE, gameObject);
			}
		}
		if (c2.type == ColliderType::Player && c2.gameObject->tag != gameObject->tag)
		{
			if (isDown)
			{
				Player* ret = (Player*)c2.gameObject->behaviour;
				if (!ret->isDown && !ret->spawning)
				{
					detectedPlayers++;
					if (rez == nullptr)
					{
						rez = App->modNetServer->spawnRezUI(gameObject->position);
					}
				}
			}
		}
	}*/
};

//struct Bullet : public Behaviour
//{
//	float secondsSinceCreation = 0.0f;
//
//	void update() override
//	{
//		const float pixelsPerSecond = 1000.0f;
//		//gameObject->position += vec2FromDegrees(gameObject->angle) * pixelsPerSecond * Time.deltaTime;
//
//		secondsSinceCreation += Time.deltaTime;
//
//		NetworkCommunication(UPDATE_POSITION, gameObject);
//
//		const float lifetimeSeconds = 2.0f;
//		if (secondsSinceCreation > lifetimeSeconds) NetworkCommunication(DESTROY, gameObject);
//	}
//};