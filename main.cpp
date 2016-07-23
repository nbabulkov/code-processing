// project: Code Processing
// date: 13.06.2015
//		Nikolai Babulkov FN-45088
//		Informatics, FMI

#include <iostream>

#include "Command.h"
#include "SourceCode.h"
#include "File.h"
#include "DynCode.h"
#include "CommentRemoval.h"
#include "Indentation.h"
#include "Normalise.h"
#include "Format.h" 


const size_t AmountOfTypes = 5;
const char commandTypes[AmountOfTypes][12] = 
{ 
	"comments", 
	"newlines", 
	"indentation", 
	"format", 
	"html"
};

enum CommandType
{
	NONE,
	comments, 
	newlines, 
	indentation, 
	format, 
	html
};
	
bool isLetter(const char symb)
{
	return (symb >= 'a' && symb <= 'z') || (symb >= 'A' && symb <= 'Z');
}

bool isCommand(const char* word)
{
	return word[0] == '-' && word[1] == '-';
}

bool isPath(char* word)
{
	int end = strlen(word);
	char* ending = word + end - 4;
	if (!strcmp(ending, ".cpp"))
		return true;
	else if (!strcmp(ending + 2, ".h"))
		return true;
	else
		return false;
}

void adjustSizes(char** argv, int argc,
				 size_t& commandsSize, size_t& filesSizes)
{
	for (int word = 1; word < argc; ++word)
	{
		if (isPath(argv[word]))
			filesSizes++;
		else if (isCommand(argv[word]))
			commandsSize++;
	}
}

CommandType matchCommand(const char* str)
{
	for (size_t pos = 0; pos < AmountOfTypes; ++pos)
	{
		size_t size = strlen(commandTypes[pos]);
		if (!strncmp(commandTypes[pos], str + 2, size))
			return CommandType (pos + 1);
	}
	return NONE;
}	

SourceCode extractSource(File& file)
{
	char* rawCode = file.readFromFile();
	SourceCode code(rawCode);
	return code;
}

Command* createCommand(char* command) 
{
	CommandType type = matchCommand(command);

	switch (type)
	{
	case comments:
		return new Comments;
	case newlines:
		return new Normalise(command);
	case indentation:
		return new Indentation(command);
	case format:
		return new Format;
	case html:
	
	default:
		throw std::invalid_argument("No such command!");
	}
}

void executeCommands(File& file, Command** commands, size_t commandsSize)
{
	SourceCode& code = extractSource(file);
	bool isWindowsStyle = true;
	for (size_t comPos = 0; comPos < commandsSize; ++comPos)
	{
		Command* curr = commands[comPos];
		curr->setCode(code);
		try
		{
			curr->transformCode(isWindowsStyle);
		}
		catch (std::invalid_argument& i)
		{
			throw;
		}
	}
	char* rawCode = code.genRawCode(isWindowsStyle);
	file.writeToFile(rawCode);
}

void clear(Command** commands, size_t commandsSize, File* files)
{
	for (size_t i = 0; i < commandsSize; ++i)
		delete commands[i];
	delete[] commands;
	delete[] files;
}

int main(/*int argc, char** argv*/)
{
	int argc = 4;
	char** argv = new char*[argc];
	argv[1] = "--comments";
	argv[2] = "Practice.cpp";
	argv[3] = "--format";


	size_t commandsSize = 0, 
		   filesSizes = 0;
	adjustSizes(argv, argc, commandsSize, filesSizes);

	Command** commands = new Command*[commandsSize];
	File* files = new File [filesSizes];

	size_t filePos = 0,
		   commandPos = 0;

	for (int pos = 1; pos < argc; ++pos)
	{
		if (isPath(argv[pos]))
		{
			try
			{
				File currFile(argv[pos]);
				files[filePos++] = currFile;
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
			
		}
		else if (isCommand(argv[pos]))
		{
			try
			{
				commands[commandPos++] = createCommand(argv[pos]);
			}
			catch (std::invalid_argument& i)
			{
				std::cerr << i.what() << std::endl;
			}
		}
		else
			std::cerr << "\n The " << pos << " command is invalid! \n";
	}

	try
	{
		for (filePos = 0; filePos < filesSizes; ++filePos)
			executeCommands(files[filePos], commands, commandsSize);
	}
	catch (std::invalid_argument& i)
	{
		std::cerr << i.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	clear(commands, commandsSize, files);
	delete[] argv;
	return 0;
}