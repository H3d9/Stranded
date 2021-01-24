#pragma once
#include "GameDef.h"


// Level Params.
extern GameWorld	g_gameWorld;
extern Camera		g_camera;

enum MissionStatus
{
	MISSION_COMPLETE = 1,
	MISSION_TIMEUP,
	MISSION_ONGOING,
	MISSION_FAILED,
};

enum MeshType
{
	MT_STATICMESH,
	MT_ANIMATEDMESH,
};

struct GameObject
{
	GameObject();

	MeshType	m_type;
	int			m_id;		//在模型列表里的索引
	float		m_x, m_y, m_z;
	float		m_rotX, m_rotY, m_rotZ;
	Matrix		m_worldMat;
	bool		m_visible;
};

struct GameWorld
{
	GameWorld();
	~GameWorld();
	void Shutdown();

	int				m_terrainID;
	int				m_skyboxID;

	GameObject*		m_staticModels;
	int				m_totalStaticModels;

	GameObject*		m_artifacts;
	int				m_totalArtifacts;
	int*			m_collectedArtifacts;
	int				m_totalCollectedArtifacts;

	GameObject*		m_characters;
	int				m_totalCharacters;
	int*			m_characterSpot; // 注意到玩家的敌人
	int				m_spottedCount;

	GameObject*		m_guns;
	int				m_totalGuns;

	int				m_enemyKilled;

	float			m_timer;         // 用于计时器的变量

	float			m_startTime;     // 开始时的系统时间
	float			m_totalTime;     // 游戏的限制时间
	float			m_currentTime;   // 当前的系统时间

	MissionStatus	m_missionStatus; // 游戏状态(进行中，胜利，失败)
};


/* GameLoop() SubRoutines */

bool LoadLevel(const char* fileName);  //使用脚本文件加载关卡
void RenderLevel();

/* Collision Tests */


bool CollisionTestStatic(int i);
bool CollisionTestArtifact(int i);
bool CollisionTestCharacter(int i);
bool CollisionTestCharacter(int i, Ray ray);
bool CharacterVisCheck(int i);
