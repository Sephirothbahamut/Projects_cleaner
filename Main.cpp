#include <string>
#include <iostream>
#include <filesystem>

#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include "Windows.h"

inline constexpr const char* command_clean_git = "git clean -xdf";
inline constexpr const char* command_clean_sln = R"q(
::FOR /d /r . %%d IN ("ipch") DO @IF EXIST "%%d" rd /s /q "%%d"
del /s /q *.obj
del /s /q *.o
del /s /q *.pdb
del /s /q *.idb
del /s /q *.ilk
del /s /q *.gch
del /s /q *.suo
del /s /q *.ncb
del /s /q *.sbr
del /s /q *.exe
)q";


void clean_git_repository(const std::filesystem::path& root)
	{
	std::cout << "Found git repository at " << root << "\n";
	std::filesystem::current_path(root);
	system(command_clean_git);
	std::cout << std::endl;
	}
void clean_sln(const std::filesystem::path& root)
	{
	std::cout << "Found Visual Studio Solution at " << root << "\n";
	std::filesystem::current_path(root);
	system(command_clean_sln);
	std::cout << std::endl;
	}

void clean_all(const std::filesystem::path& root)
	{
	std::cout << "Checking " << root << std::endl;
	for (const auto& entry : std::filesystem::directory_iterator{root})
		{
		auto path{entry.path()};

		if (path.filename().string() == ".git")
			{
			clean_git_repository(root);
			return;
			}
		else if (std::filesystem::is_regular_file(path) && path.has_extension() && path.extension().string() == ".sln")
			{
			clean_sln(root);
			return;
			}
		}

	for (const auto& entry : std::filesystem::directory_iterator{root})
		{
		auto path{entry.path()};
		if (std::filesystem::is_directory(path)) { clean_all(path); }
		}
	}

int main(int argc, char** argv)
	{
	std::vector<std::string> arguments(argv + 1, argv + argc);
	if (arguments.size()) 
		{
		for (const auto& argument : arguments)
			{
			clean_all({argument});
			}
		}
	else { clean_all(std::filesystem::current_path()); }
	}
