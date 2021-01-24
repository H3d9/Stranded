// Stranded Engine
// 脚本系统 接口定义

#include "EngineBase.h"
#include "CommandScript.h"
#include <fstream>

using namespace std;


CommandScript::CommandScript() : totalScriptLines(0), currentLine(0),
                                   currentLineChar(0), m_script(0)
{
}


CommandScript::~CommandScript()
{
    ShutDown();
}


bool CommandScript::LoadScriptFile(const char *filename)
{
   ifstream input, input2;
   char tempLine[MAX_LINE_SIZE];
   
   input.open(filename);
   if(!input.is_open()) return false;
   
   ShutDown();

   // Open and get number of lines.
   while(!input.eof())
      {
         input.getline(tempLine, MAX_LINE_SIZE, '\n');
         totalScriptLines++;
      }
   
   input.close();
   
   input2.open(filename);
   if(!input2.is_open()) return false;
   
   // Load in every line of text.   
   m_script = new char*[totalScriptLines];
   
   for(int i = 0; i < totalScriptLines; i++)
      {
         m_script[i] = new char[MAX_LINE_SIZE + 1];
         input2.getline(m_script[i], MAX_LINE_SIZE, '\n');
      }
   
   input2.close();
   return true;
}


void CommandScript::ParseCommand(char *destCommand)
{
   // This function will take the first word of the current
   // line and save it in destCommand.

   int commandSize = 0;

   // If destcommand is NULL, or if we run out of lines, or at the end
   // of the current line then we return.
   if(!destCommand) return;
   if(currentLine >= totalScriptLines) return;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) return;

   // Init string.
   destCommand[0] = '\0';
   
   // Since commands start each line we can say that if there is
   // a # at the start of the line, then this is a comment.
   if(IsLineComment())
      {
         destCommand[0] = '#';
         destCommand[1] = '\0';
         return;
      }

   // Loop through every character until you find a space or newline.
   // That means we are at the end of a command.
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
         if(m_script[currentLine][currentLineChar] == ' ' ||
            m_script[currentLine][currentLineChar] == '\n')
            break;

         // Save the text in the array.
         destCommand[commandSize] = m_script[currentLine][currentLineChar];
         commandSize++;
         currentLineChar++;
      }

   // Skip next space or newline.
   currentLineChar++;
   destCommand[commandSize] = '\0';
}


void CommandScript::ParseStringParam(char *destString)
{
   // This function will take a text inside " and " and save
   // it to destString.

   int paramSize = 0;
   bool endQuoteFound = false;

   // If destcommand is NULL, or if we run out of lines, or at the end
   // of the current line then we return.
   if(!destString) return;
   if(currentLine >= totalScriptLines) return;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) return;

   // Initialize string.
   destString[0] = '\0';

   // Skip beginning quote.
   currentLineChar++;

   // Loop through every character until you find a end quote or newline.
   // That means we are at the end of a string.
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
         if(m_script[currentLine][currentLineChar] == '"')
            {
               endQuoteFound = true;
               break;
            }

         if(m_script[currentLine][currentLineChar] == '\n')
            break;

         // Save the text in the array.
         destString[paramSize] = m_script[currentLine][currentLineChar];
         paramSize++;
         currentLineChar++;
      }

   // Skip end quotes and next space or newline.  In this system we don't
   // allow strings to take up multiple lines.  You can simple have
   // multiple print string commands instead.
   if(endQuoteFound) currentLineChar += 2;
   else currentLineChar++;

   destString[paramSize] = '\0';
}


bool CommandScript::ParseBoolParam()
{
   // This function will get the next text and will return
   // true if this text is "true" or false if it is "false".

   char string[MAX_PARAM_SIZE];
   int paramSize = 0;

   // If we run out of lines, or at the end
   // of the current line then we return.
   if(currentLine >= totalScriptLines) return 0;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) return 0;

   // Loop through every character until you find a space or newline.
   // That means we are at the end of a variable.
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
         if(m_script[currentLine][currentLineChar] == ' ' ||
            m_script[currentLine][currentLineChar] == '\n')
            break;

         // Save the text in the array.
         string[paramSize] = m_script[currentLine][currentLineChar];
         paramSize++;
         currentLineChar++;
      }

   // Skip next space or newline.
   currentLineChar++;
   string[paramSize] = '\0';

   if(_stricmp(string, "true") == 0) return true;
   return false;
}


int CommandScript::ParseIntParam()
{
   // This funciton will take the next text and convert
   // it to a int that is returned.

   char string[MAX_PARAM_SIZE];
   int paramSize = 0;

   // If we run out of lines, or at the end
   // of the current line then we return.
   if(currentLine >= totalScriptLines) return 0;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) return 0;

   // Loop through every character until you find a space or newline.
   // That means we are at the end of a variable.
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
         if(m_script[currentLine][currentLineChar] == ' ' ||
            m_script[currentLine][currentLineChar] == '\n')
            break;

         // Save the text in the array.
         string[paramSize] = m_script[currentLine][currentLineChar];
         paramSize++;
         currentLineChar++;
      }

   // Skip next space or newline.
   currentLineChar++;
   string[paramSize] = '\0';
   return atoi(string);
}


float CommandScript::ParseFloatParam()
{
   // This funciton will take the next text and convert
   // it to a float that is returned.

   char string[MAX_PARAM_SIZE];
   int paramSize = 0;

   // If we run out of lines, or at the end
   // of the current line then we return.
   if(currentLine >= totalScriptLines) return 0;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) return 0;

   // Loop through every character until you find a space or newline.
   // That means we are at the end of a variable.
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
         if(m_script[currentLine][currentLineChar] == ' ' ||
            m_script[currentLine][currentLineChar] == '\n')
            break;

         // Save the text in the array.
         string[paramSize] = m_script[currentLine][currentLineChar];
         paramSize++;
         currentLineChar++;
      }

   // Skip next space or newline.
   currentLineChar++;
   string[paramSize] = '\0';
   return (float)atof(string);
}

void CommandScript::ShutDown()
{
    if (m_script)
    {
        // Delete every line in the script file.
        for (int i = 0; i < totalScriptLines; i++)
        {
            if (m_script[i])
            {
                delete[] m_script[i];
                m_script[i] = 0;
            }
        }

        delete m_script;
        m_script = 0;
    }

    // Reset variables.  
    totalScriptLines = 0;
    currentLineChar = 0;
    currentLine = 0;
}


void CommandScript::MoveToStart()
{
   currentLine = 0;
   currentLineChar = 0;
}


void CommandScript::MoveToNextLine()
{
   currentLine++;
   currentLineChar = 0;
}


int CommandScript::GetCurrentLineNum()
{
   return currentLine;
}


int CommandScript::GetTotalLines()
{
   return totalScriptLines;
}


bool CommandScript::IsLineComment()
{
   // If the first character of a line a # then
   // the line must be commented out.
   if(m_script[currentLine][0] == '#') return true;
   return false;
}

void CommandScript::Release()
{
    delete this;
}
