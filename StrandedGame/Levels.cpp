
#include <stdio.h>
#include "Levels.h"

// Level Params
GameWorld			g_gameWorld;
Camera				g_camera;

GameObject::GameObject(): m_type(), m_id(0),
                          m_x(0), m_y(0), m_z(0),
                          m_rotX(0), m_rotY(0), m_rotZ(0), m_visible(true)
{
}

GameWorld::GameWorld():
m_terrainID(-1),
m_skyboxID(-1),
m_staticModels(nullptr),
m_totalStaticModels(0),
m_artifacts(nullptr),
m_totalArtifacts(0),
m_collectedArtifacts(nullptr),
m_totalCollectedArtifacts(0),
m_characters(nullptr),
m_totalCharacters(0),
m_characterSpot(nullptr), 
m_spottedCount(0),
m_guns(nullptr),
m_totalGuns(0),
m_enemyKilled(0),
m_totalTime(0),
m_missionStatus(MISSION_ONGOING)
{
	m_timer =
	m_startTime =
	m_currentTime = (float)timeGetTime() * 0.001f;
}

GameWorld::~GameWorld()
{
	Shutdown();
}

void GameWorld::Shutdown()
{
	delete[] m_staticModels;
	m_staticModels = nullptr;
	delete[] m_artifacts;
	m_artifacts = nullptr;
	delete[] m_collectedArtifacts;
	m_collectedArtifacts = nullptr;
	delete[] m_characters;
	m_characters = nullptr;
	delete[] m_characterSpot;
	m_characterSpot = nullptr;
	m_missionStatus = MISSION_ONGOING;

	m_terrainID = -1;
	m_skyboxID = -1;
	m_totalStaticModels = 0;
	m_totalArtifacts = 0;
	m_totalCollectedArtifacts = 0;
	m_totalCharacters = 0;
	m_spottedCount = 0;
	m_totalTime = 0;
	m_enemyKilled = 0;
}

bool LoadLevel(const char* fileName)
{
	if (!g_Renderer || !fileName) return false;

	/* 读取脚本 */
	char command[MAX_COMMAND_SIZE];
	char param[MAX_PARAM_SIZE];
	int totalStaticModels = 0;
	int currentStaticModelNum = 0;
	int totalArtifacts = 0;
	int currentArtifactNum = 0;
	int totalCharacters = 0;
	int currentCharacterNum = 0;
	int totalGuns = 0;
	int currentGun = 0;

	if (!g_ScriptSystem->LoadScriptFile(fileName)) return false;

		/* 读取脚本，记录模型数，检测错误 */
	for (auto i = 0; i < g_ScriptSystem->GetTotalLines(); i++)
	{
		g_ScriptSystem->ParseCommand(command);

		if (_stricmp(command, "LoadSkyBoxAsX") == 0) {}
		else if (_stricmp(command, "LoadTerrainAsX") == 0) {}
		else if (_stricmp(command, "LoadStaticModelAsX") == 0)
		{
			totalStaticModels++;
		}
		else if (_stricmp(command, "LoadAnimatedModelAsX") == 0){}
		else if (_stricmp(command, "LoadArtifactModelAsX") == 0)
		{
			totalArtifacts++;
		}
		else if (_stricmp(command, "LoadCharacterModelAsX") == 0)
		{
			totalCharacters++;
		}
		else if (_stricmp(command, "LoadGunsAsX") == 0)
		{
			totalGuns++;
		}
		else if (_stricmp(command,"SetCamera") == 0) {}
		else if (_stricmp(command, "#") == 0) {}
		else
		{
			g_ScriptSystem->Release();
			return false;
		}
		g_ScriptSystem->MoveToNextLine();
	}

		/* 为关卡全局变量的成员分配内存 */
	delete[] g_gameWorld.m_staticModels;
	delete[] g_gameWorld.m_artifacts;
	delete[] g_gameWorld.m_collectedArtifacts;
	delete[] g_gameWorld.m_characters;
	delete[] g_gameWorld.m_characterSpot;
	delete[] g_gameWorld.m_guns;
	g_gameWorld.m_staticModels = new GameObject[totalStaticModels];
	g_gameWorld.m_artifacts = new GameObject[totalArtifacts];
	g_gameWorld.m_collectedArtifacts = new int[totalArtifacts]();
	g_gameWorld.m_characters = new GameObject[totalCharacters];
	g_gameWorld.m_characterSpot = new int[totalCharacters]();
	g_gameWorld.m_guns = new GameObject[totalGuns];
	g_gameWorld.m_totalStaticModels = totalStaticModels;
	g_gameWorld.m_totalArtifacts = totalArtifacts;
	g_gameWorld.m_totalCharacters = totalCharacters;
	g_gameWorld.m_totalGuns = totalGuns;

		/* 再次读取脚本 */
	g_ScriptSystem->MoveToStart();
	for (auto i = 0; i < g_ScriptSystem->GetTotalLines(); i++)
	{
		g_ScriptSystem->ParseCommand(command);
		if (_stricmp(command, "LoadSkyBoxAsX") == 0)
		{
			g_ScriptSystem->ParseStringParam(param);
			if (g_gameWorld.m_skyboxID == -1)
			{
				if (!g_Renderer->LoadXModel(param, &g_gameWorld.m_skyboxID)) return false;
			}
		}
		else if (_stricmp(command, "LoadTerrainAsX") == 0)
		{
			g_ScriptSystem->ParseStringParam(param);
			if (g_gameWorld.m_terrainID == -1)
			{
				if (!g_Renderer->LoadXModel(param, &g_gameWorld.m_terrainID)) return false;
			}
		}
		else if (_stricmp(command, "LoadStaticModelAsX") == 0)
		{
			auto obj =  &g_gameWorld.m_staticModels[currentStaticModelNum++];

			if(obj)
			{
				g_ScriptSystem->ParseStringParam(param);

				if(!g_Renderer->LoadXModel(param, &obj->m_id)) return false;
				obj->m_type = MT_STATICMESH;
				obj->m_x = g_ScriptSystem->ParseFloatParam();
				obj->m_y = g_ScriptSystem->ParseFloatParam();
				obj->m_z = g_ScriptSystem->ParseFloatParam();
				obj->m_rotX = g_ScriptSystem->ParseFloatParam();
				obj->m_rotY = g_ScriptSystem->ParseFloatParam();
				obj->m_rotZ = g_ScriptSystem->ParseFloatParam();
			}
		}
		else if (_stricmp(command, "LoadAnimatedModelAsX") == 0) {}
		else if (_stricmp(command, "LoadArtifactModelAsX") == 0)
		{
			auto obj = &g_gameWorld.m_artifacts[currentArtifactNum++];

			if (obj)
			{
				g_ScriptSystem->ParseStringParam(param);

				if (!g_Renderer->LoadXModel(param, &obj->m_id)) return false;
				obj->m_type = MT_ANIMATEDMESH;
				obj->m_x = g_ScriptSystem->ParseFloatParam();
				obj->m_y = g_ScriptSystem->ParseFloatParam();
				obj->m_z = g_ScriptSystem->ParseFloatParam();
				obj->m_rotX = g_ScriptSystem->ParseFloatParam();
				obj->m_rotY = g_ScriptSystem->ParseFloatParam();
				obj->m_rotZ = g_ScriptSystem->ParseFloatParam();
			}
		}
		else if (_stricmp(command, "LoadCharacterModelAsX") == 0)
		{
			auto obj = &g_gameWorld.m_characters[currentCharacterNum++];

			if (obj)
			{
				g_ScriptSystem->ParseStringParam(param);

				if (!g_Renderer->LoadXModel(param, &obj->m_id)) return false;
				obj->m_type = MT_ANIMATEDMESH;
				obj->m_x = g_ScriptSystem->ParseFloatParam();
				obj->m_y = g_ScriptSystem->ParseFloatParam();
				obj->m_z = g_ScriptSystem->ParseFloatParam();
				obj->m_rotX = g_ScriptSystem->ParseFloatParam();
				obj->m_rotY = g_ScriptSystem->ParseFloatParam();
				obj->m_rotZ = g_ScriptSystem->ParseFloatParam();
			}
		}
		else if (_stricmp(command, "LoadGunsAsX") == 0)
		{
			auto obj = &g_gameWorld.m_guns[currentGun++];

			if (obj)
			{
				g_ScriptSystem->ParseStringParam(param);

				if (!g_Renderer->LoadXModel(param, &obj->m_id)) return false;
				obj->m_type = MT_ANIMATEDMESH;
				obj->m_x = g_ScriptSystem->ParseFloatParam();
				obj->m_y = g_ScriptSystem->ParseFloatParam();
				obj->m_z = g_ScriptSystem->ParseFloatParam();
				obj->m_rotX = g_ScriptSystem->ParseFloatParam();
				obj->m_rotY = g_ScriptSystem->ParseFloatParam();
				obj->m_rotZ = g_ScriptSystem->ParseFloatParam();
			}
		}
		else if (_stricmp(command, "SetCamera") == 0)
		{
			float x = g_ScriptSystem->ParseFloatParam();
			float y = g_ScriptSystem->ParseFloatParam();
			float z = g_ScriptSystem->ParseFloatParam();
			float xv = g_ScriptSystem->ParseFloatParam();
			float yv = g_ScriptSystem->ParseFloatParam();
			float zv = g_ScriptSystem->ParseFloatParam();
			float xu = g_ScriptSystem->ParseFloatParam();
			float yu = g_ScriptSystem->ParseFloatParam();
			float zu = g_ScriptSystem->ParseFloatParam();

			STECameraSet(&g_camera, x, y, z, xv, yv, zv, xu, yu, zu);
		}
		else if (_stricmp(command, "#") == 0) {}
		g_ScriptSystem->MoveToNextLine();
	}

	g_ScriptSystem->ShutDown();

	/* 根据脚本计算模型的世界变换 */
	for (int i = 0; i < g_gameWorld.m_totalStaticModels; i++) {
		Matrix trans, rotX, rotY, rotZ;
		STEMatrixTranslate(
			&trans,
			g_gameWorld.m_staticModels[i].m_x,
			g_gameWorld.m_staticModels[i].m_y,
			g_gameWorld.m_staticModels[i].m_z
		);
		STEMatrixRotateByAxis(&rotX, g_gameWorld.m_staticModels[i].m_rotX, 1, 0, 0);
		STEMatrixRotateByAxis(&rotY, g_gameWorld.m_staticModels[i].m_rotY, 0, 1, 0);
		STEMatrixRotateByAxis(&rotZ, g_gameWorld.m_staticModels[i].m_rotZ, 0, 0, 1);

		g_gameWorld.m_staticModels[i].m_worldMat = trans * (rotX * rotY * rotZ);
	}

	for (int i = 0; i < g_gameWorld.m_totalArtifacts; i++) {
		Matrix trans, rotX, rotY, rotZ;
		STEMatrixTranslate(
			&trans,
			g_gameWorld.m_artifacts[i].m_x,
			g_gameWorld.m_artifacts[i].m_y,
			g_gameWorld.m_artifacts[i].m_z
		);
		STEMatrixRotateByAxis(&rotX, g_gameWorld.m_artifacts[i].m_rotX, 1, 0, 0);
		STEMatrixRotateByAxis(&rotY, g_gameWorld.m_artifacts[i].m_rotY, 0, 1, 0);
		STEMatrixRotateByAxis(&rotZ, g_gameWorld.m_artifacts[i].m_rotZ, 0, 0, 1);

		g_gameWorld.m_artifacts[i].m_worldMat = trans * (rotX * rotY * rotZ);
	}

	for (int i = 0; i < g_gameWorld.m_totalCharacters; i++) {
		Matrix trans, rotX, rotY, rotZ;
		STEMatrixTranslate(
			&trans,
			g_gameWorld.m_characters[i].m_x,
			g_gameWorld.m_characters[i].m_y,
			g_gameWorld.m_characters[i].m_z
		);
		STEMatrixRotateByAxis(&rotX, g_gameWorld.m_characters[i].m_rotX, 1, 0, 0);
		STEMatrixRotateByAxis(&rotY, g_gameWorld.m_characters[i].m_rotY, 0, 1, 0);
		STEMatrixRotateByAxis(&rotZ, g_gameWorld.m_characters[i].m_rotZ, 0, 0, 1);

		g_gameWorld.m_characters[i].m_worldMat = trans * (rotX * rotY * rotZ);
	}

	for (int i = 0; i < g_gameWorld.m_totalGuns; i++) {
		Matrix trans, rotX, rotY, rotZ;
		STEMatrixTranslate(
			&trans,
			g_gameWorld.m_guns[i].m_x,
			g_gameWorld.m_guns[i].m_y,
			g_gameWorld.m_guns[i].m_z
		);
		STEMatrixRotateByAxis(&rotX, g_gameWorld.m_guns[i].m_rotX, 1, 0, 0);
		STEMatrixRotateByAxis(&rotY, g_gameWorld.m_guns[i].m_rotY, 0, 1, 0);
		STEMatrixRotateByAxis(&rotZ, g_gameWorld.m_guns[i].m_rotZ, 0, 0, 1);

		g_gameWorld.m_guns[i].m_worldMat = trans * (rotX * rotY * rotZ);
	}

	/* 设置游戏时间限制 */
	g_gameWorld.m_totalTime = 120.0f;
	g_gameWorld.m_startTime = (float)timeGetTime() * 0.001f;

	return true;
}

void RenderLevel() {
	float currentTime = (float)timeGetTime() * 0.001f;
	float timeDelta = (float)timeGetTime() * 0.001f - g_gameWorld.m_timer;

	/* 更新关卡的当前时间 */
	if(g_gameWorld.m_missionStatus == MISSION_ONGOING)
		g_gameWorld.m_currentTime = currentTime;

	g_Renderer->StartRendering();

	g_Renderer->SetCamera(g_camera.m_pos, g_camera.m_view, g_camera.m_up);

	/* 渲染地形 */
	Matrix world;
	g_Renderer->SetWorldMatrix(&world);	
	g_Renderer->RenderXModel(g_gameWorld.m_terrainID);

	/* 渲染天空 */
	STEMatrixTranslate(&world, g_camera.m_pos.x, g_camera.m_pos.y, g_camera.m_pos.z);
	g_Renderer->SetWorldMatrix(&world);
	g_Renderer->RenderXModel(g_gameWorld.m_skyboxID);

	/* 渲染普通网格物体 */
	for (auto i = 0; i < g_gameWorld.m_totalStaticModels; i++)
	{
		if (g_gameWorld.m_staticModels[i].m_type == MT_ANIMATEDMESH)
			g_Renderer->UpdateXAnimation(g_gameWorld.m_staticModels[i].m_id,
				timeDelta, &g_gameWorld.m_staticModels[i].m_worldMat);
		else
			g_Renderer->UpdateXAnimation(g_gameWorld.m_staticModels[i].m_id, 
				0, &g_gameWorld.m_staticModels[i].m_worldMat);

		// Render object.
		g_Renderer->RenderXModel(g_gameWorld.m_staticModels[i].m_id);
	}

	/* 渲染Artifacts网格 */
	for (auto i = 0; i < g_gameWorld.m_totalArtifacts; i++)
	{
		/* 检测现在是否与宝物发生碰撞 */
		if(CollisionTestArtifact(i) && !g_gameWorld.m_collectedArtifacts[i])
		{
			// PlaySound("sound/shot.wav", NULL, SND_FILENAME | SND_ASYNC);
			g_gameWorld.m_collectedArtifacts[i] = 1;
			g_gameWorld.m_totalCollectedArtifacts++;
		}

		if(!g_gameWorld.m_collectedArtifacts[i])
		{
			g_Renderer->UpdateXAnimation(g_gameWorld.m_artifacts[i].m_id,
				timeDelta, &g_gameWorld.m_artifacts[i].m_worldMat);
			g_Renderer->RenderXModel(g_gameWorld.m_artifacts[i].m_id);
		}
	}

	/* 渲染角色网格 */
	for (auto i = 0; i < g_gameWorld.m_totalCharacters; i++)
	{
		if(g_gameWorld.m_characters[i].m_visible)
		{
			// 发现角色的敌人会移动，否则不移动
			if (g_gameWorld.m_characterSpot[i])
				g_Renderer->UpdateXAnimation(g_gameWorld.m_characters[i].m_id,
					timeDelta, &g_gameWorld.m_characters[i].m_worldMat);
			else
				g_Renderer->UpdateXAnimation(g_gameWorld.m_characters[i].m_id,
					0, &g_gameWorld.m_characters[i].m_worldMat);

			g_Renderer->RenderXModel(g_gameWorld.m_characters[i].m_id);
		}
	}

	/* 渲染枪支 */
	for (auto i = 0; i < g_gameWorld.m_totalGuns; i++)
	{
		// 枪(hud模型)世界变换矩阵：g_gameWorld.m_guns[i].m_worldMat
		// 求解基于camera的hud世界变换矩阵
		g_Renderer->UpdateHudPositionFromCamera(&g_gameWorld.m_guns[i].m_worldMat, &g_camera, &angleH, &angleV);

		g_Renderer->UpdateXAnimation(g_gameWorld.m_guns[i].m_id,
				timeDelta, &g_gameWorld.m_guns[i].m_worldMat);

		g_Renderer->RenderXModel(g_gameWorld.m_guns[i].m_id);
	}

	/* 显示文字 */
	auto color = COLOR_ARGB(255, 255, 0, 255);
	g_Renderer->DisplayText(g_arialID, 1000, 10, color, g_fpsBuf);
	
	wchar_t displayBuf[128];
	if(g_gameWorld.m_missionStatus == MISSION_ONGOING)
	{
		auto timeLeft = g_gameWorld.m_totalTime - (g_gameWorld.m_currentTime - g_gameWorld.m_startTime);
		if (timeLeft < 0) timeLeft = 0;

		swprintf_s(displayBuf, 128,
			L"MISSION_ONGOING - 剩余时间: %d s  收集的宝物: %d / %d 水平角度：%fpi 垂直角度：%fpi", //
			(int)timeLeft, g_gameWorld.m_totalCollectedArtifacts, g_gameWorld.m_totalArtifacts, 
			angleH / PI, angleV / PI);

		g_Renderer->DisplayText(g_arial24ID, 10, 10, color, displayBuf);
	}
	else if(g_gameWorld.m_missionStatus == MISSION_COMPLETE)
	{
		g_Renderer->DisplayText(g_timesBigID, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 - 100, color, L"You Win! ");

		swprintf_s(displayBuf, 128, L"用时：%d s, 杀了 %d 个敌人",
			(int)(g_gameWorld.m_currentTime - g_gameWorld.m_startTime), g_gameWorld.m_enemyKilled);

		g_Renderer->DisplayText(g_timesBigID, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 + 100, color, displayBuf);
	}
	else if(g_gameWorld.m_missionStatus == MISSION_TIMEUP)
	{
		g_Renderer->DisplayText(g_timesBigID, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 - 100, color, L"Mission Failed!");
		g_Renderer->DisplayText(g_timesBigID, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 + 100, color, L"TIME  UP");
	}
	else if(g_gameWorld.m_missionStatus == MISSION_FAILED)
	{
		g_Renderer->DisplayText(g_timesBigID, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 - 100, color, L"Mission Failed!");
		g_Renderer->DisplayText(g_timesBigID, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 + 100, color, L"你呗发现了");
	}

	/* 绘制准星 */
	g_Renderer->DisplayText(g_arial36ID, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 30, color, L"+");

	/* 结束渲染 */
	g_gameWorld.m_timer = currentTime;
	g_Renderer->EndRendering();
}


bool CollisionTestStatic(int i)
{
	BoundingSphere sphere;
	int id = g_gameWorld.m_staticModels[i].m_id;

	g_Renderer->GetBoundingSphere(id, &sphere.m_center, &sphere.m_radius);

	Matrix* worldMat = &g_gameWorld.m_staticModels[i].m_worldMat;
	STEMatrixMultiplyVector3(&sphere.m_center, worldMat, &sphere.m_center);

	sphere.m_radius -= 1.0f;
	if (STEBoundingSpherePointIsInside(&sphere, &g_camera.m_pos)) {
		return true;
	}
	return false;
}

bool CollisionTestArtifact(int i)
{
	BoundingSphere sphere;
	int id = g_gameWorld.m_artifacts[i].m_id;

	g_Renderer->GetBoundingSphere(id, &sphere.m_center, &sphere.m_radius);

	Matrix* worldMat = &g_gameWorld.m_artifacts[i].m_worldMat;
	STEMatrixMultiplyVector3(&sphere.m_center, worldMat, &sphere.m_center);

	sphere.m_radius += 1.0f;
	if (STEBoundingSpherePointIsInside(&sphere, &g_camera.m_pos))
		return true;

	return false;
}

bool CollisionTestCharacter(int i)
{
	if (!g_gameWorld.m_characters[i].m_visible)
		return false;

	BoundingSphere sphere;
	int id = g_gameWorld.m_characters[i].m_id;

	g_Renderer->GetBoundingSphere(id, &sphere.m_center, &sphere.m_radius);

	Matrix* worldMat = &g_gameWorld.m_characters[i].m_worldMat;
	STEMatrixMultiplyVector3(&sphere.m_center, worldMat, &sphere.m_center);
	
	if (STEBoundingSpherePointIsInside(&sphere, &g_camera.m_pos)) {
		return true;
	}

	return false;
}

bool CollisionTestCharacter(int i, Ray ray)
{
	if (!g_gameWorld.m_characters[i].m_visible)
		return false;

	BoundingSphere sphere;
	int id = g_gameWorld.m_characters[i].m_id;

	g_Renderer->GetBoundingSphere(id, &sphere.m_center, &sphere.m_radius);

	Matrix* worldMat = &g_gameWorld.m_characters[i].m_worldMat;
	STEMatrixMultiplyVector3(&sphere.m_center, worldMat, &sphere.m_center);

	if (STERayIntersectSphere(&ray, &sphere.m_center, sphere.m_radius, 0))
		return true;

	return false;
}

bool CharacterVisCheck(int i)
{
	if (!g_gameWorld.m_characters[i].m_visible)
		return false;

	BoundingSphere sphere;
	int id = g_gameWorld.m_characters[i].m_id;

	g_Renderer->GetBoundingSphere(id, &sphere.m_center, &sphere.m_radius);

	Matrix* worldMat = &g_gameWorld.m_characters[i].m_worldMat;
	STEMatrixMultiplyVector3(&sphere.m_center, worldMat, &sphere.m_center);

	sphere.m_radius *= 10.0f;
	if (STEBoundingSpherePointIsInside(&sphere, &g_camera.m_pos))
		return true;

	return false;
}
