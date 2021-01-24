// Stranded Engine
// 脚本系统 接口声明

#pragma once

#include "IScriptSystem.h"


class CommandScript : public IScriptSystem {
public:
	CommandScript();
	~CommandScript();

	virtual bool LoadScriptFile(const char* filename) override;
	virtual void ParseCommand(char* destCommand) override;
	virtual void ParseStringParam(char* destString) override;
	virtual bool ParseBoolParam() override;
	virtual int ParseIntParam() override;
	virtual float ParseFloatParam() override;
	virtual void ShutDown() override;

	virtual void MoveToStart() override;
	virtual void MoveToNextLine() override;
	virtual int GetCurrentLineNum() override;
	virtual int GetTotalLines() override;
	virtual bool IsLineComment() override;

	virtual void Release() override;

private:
	int totalScriptLines;
	int currentLine;
	int currentLineChar;
	char** m_script;
};
