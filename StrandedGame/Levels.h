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
	int			m_id;		//��ģ���б��������
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
	int*			m_characterSpot; // ע�⵽��ҵĵ���
	int				m_spottedCount;

	GameObject*		m_guns;
	int				m_totalGuns;

	int				m_enemyKilled;

	float			m_timer;         // ���ڼ�ʱ���ı���

	float			m_startTime;     // ��ʼʱ��ϵͳʱ��
	float			m_totalTime;     // ��Ϸ������ʱ��
	float			m_currentTime;   // ��ǰ��ϵͳʱ��

	MissionStatus	m_missionStatus; // ��Ϸ״̬(�����У�ʤ����ʧ��)
};


/* GameLoop() SubRoutines */

bool LoadLevel(const char* fileName);  //ʹ�ýű��ļ����عؿ�
void RenderLevel();

/* Collision Tests */


bool CollisionTestStatic(int i);
bool CollisionTestArtifact(int i);
bool CollisionTestCharacter(int i);
bool CollisionTestCharacter(int i, Ray ray);
bool CharacterVisCheck(int i);
