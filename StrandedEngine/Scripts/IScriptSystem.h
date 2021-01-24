// Stranded Engine
// �ű�ϵͳ ����ӿ�

// ˵����
// �ṩ�ű�ϵͳ�ĳ��������Ϊ�ӿڡ�
// ���ģʽ��
// factory������������Engine/Engine.h|cpp��


#pragma once

class IScriptSystem {
public:
	virtual bool LoadScriptFile(const char* filename) = 0;
	virtual void ParseCommand(char* destCommand) = 0;
	virtual void ParseStringParam(char* destString) = 0;
	virtual bool ParseBoolParam() = 0;
	virtual int ParseIntParam() = 0;
	virtual float ParseFloatParam() = 0;
	virtual void ShutDown() = 0;

	virtual void MoveToStart() = 0;
	virtual void MoveToNextLine() = 0;
	virtual int GetCurrentLineNum() = 0;
	virtual int GetTotalLines() = 0;
	virtual bool IsLineComment() = 0;

	virtual void Release() = 0;
};