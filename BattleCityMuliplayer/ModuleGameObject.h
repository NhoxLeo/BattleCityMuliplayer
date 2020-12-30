#pragma once

struct GameObject
{
	void newReplicationState(D3DXVECTOR3 position, D3DXVECTOR3 rotation);
	void Interpolate();

	//Transform component
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 rotation = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 speed = D3DXVECTOR3(0, 0, 0);

	// Render component
	D3DXVECTOR3 pivot = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 size = D3DXVECTOR3(0, 0, 0); // NOTE(jesus): If equals 0, it takes the size of the texture
	float angle = 0.0f;
	D3DXVECTOR4 color = D3DXVECTOR4(0, 0, 0, 0); // NOTE(jesus): The texture will tinted with this color
	//Texture * texture = nullptr;
	int  order = 0;          // NOTE(jesus): determines the drawing order

	// For entity interpolation
	D3DXVECTOR3 initial_position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 initial_rotation = D3DXVECTOR3(0, 0, 0);
	float initial_angle = 0.0f;

	D3DXVECTOR3 final_position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 final_rotation = D3DXVECTOR3(0, 0, 0);
	float final_angle = 0.0f;

	float interpolationSecondsElapsed = 0.0f;

	float syncWaitTime = 0;
	int lateFrames = 0;

	// Animation component
	//Animation* animation = nullptr;

	// Collider component
	//Collider *collider = nullptr;

	// "Script" component
	Behaviour *behaviour = nullptr;

	// Network identity component
	uint32 networkId = 0; // NOTE(jesus): Only for network game objects

	uint32 tickCount = 0;

	// NOTE(jesus): Don't use in gameplay systems (use Instantiate, Destroy instead)
	enum State { NON_EXISTING, CREATING, UPDATING, DESTROYING };
	State state = NON_EXISTING;

	// Tag for custom usage
	uint32 tag = 0;

	// Name for players
	std::string name = "";

	// Detect the player inside the client
	bool isPlayer = false;

	uint32 clientInstanceNID = 0; //To define and identify that this will be an instance executed by the client code

	// Interpolate object
	bool doInterpolation = true;

	bool isAI = false;

private:

	void* operator new(size_t size) = delete;
	void operator delete (void* obj) = delete;

	void releaseComponents();
	friend class ModuleGameObject;
};

class ModuleGameObject : public Module
{
public:

	// Virtual functions

	bool init() override;

	bool preUpdate() override;

	bool update() override;

	bool postUpdate() override;

	bool cleanUp() override;

	static GameObject* Instantiate();

	static void Destroy(GameObject* gameObject);

	GameObject gameObjects[MAX_GAME_OBJECTS] = {};

	bool interpolateEntities = true;
};


// NOTE(jesus): These functions are named after Unity functions

GameObject* Instantiate();

void Destroy(GameObject* gameObject);