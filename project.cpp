// Copyright (c) 2015-2023 Vector 35 Inc
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
#include <cstring>
#include <optional>
#include "binaryninjaapi.h"
#include "binaryninjacore.h"

using namespace BinaryNinja;


Project::Project(BNProject* project)
{
	m_object = project;
}


Ref<Project> Project::CreateProject(const std::string& path, const std::string& name)
{
	BNProject* bnproj = BNCreateProject(path.c_str(), name.c_str());
	if (!bnproj)
		return nullptr;
	return new Project(BNNewProjectReference(bnproj));
}


Ref<Project> Project::OpenProject(const std::string& path)
{
	BNProject* bnproj = BNOpenProject(path.c_str());
	if (!bnproj)
		return nullptr;
	return new Project(BNNewProjectReference(bnproj));
}


Ref<Project> Project::GetActiveProject()
{
	BNProject* bnproj = BNGetActiveProject();
	if (!bnproj)
		return nullptr;
	return new Project(BNNewProjectReference(bnproj));
}


bool Project::Close()
{
	return BNCloseProject(m_object);
}


std::string Project::GetId() const
{
	char* id = BNProjectGetId(m_object);
	std::string result = id;
	BNFreeString(id);
	return result;
}


bool Project::IsOpen() const
{
	return BNProjectIsOpen(m_object);
}


std::string Project::GetPath() const
{
	char* path = BNProjectGetPath(m_object);
	std::string result = path;
	BNFreeString(path);
	return result;
}

std::string Project::GetResourcesPath() const
{
	char* path = BNProjectGetResourcesPath(m_object);
	std::string result = path;
	BNFreeString(path);
	return result;
}


std::string Project::GetName() const
{
	char* name = BNProjectGetName(m_object);
	std::string result = name;
	BNFreeString(name);
	return result;
}


void Project::SetName(const std::string& name)
{
	BNProjectSetName(m_object, name.c_str());
}


bool Project::PathExists(Ref<ProjectFolder> folder, const std::string& name) const
{
	LogWarn("Path exists called");
	return false;
}


void Project::PullFolders()
{
	BNProjectPullFolders(m_object);
}


Ref<ProjectFolder> Project::CreateFolder(Ref<ProjectFolder> parent, const std::string& name, const std::string& description)
{
	BNProjectFolder* folder = BNProjectCreateFolder(m_object, parent ? parent->m_object : nullptr, name.c_str(), description.c_str());
	if (folder == nullptr)
		return nullptr;
	return new ProjectFolder(BNNewProjectFolderReference(folder));
}


std::vector<Ref<ProjectFolder>> Project::GetFolders() const
{
	size_t count;
	BNProjectFolder** folders = BNProjectGetFolders(m_object, &count);

	std::vector<Ref<ProjectFolder>> result;
	result.reserve(count);
	for (size_t i = 0; i < count; i++)
	{
		result.push_back(new ProjectFolder(BNNewProjectFolderReference(folders[i])));
	}

	BNFreeProjectFolderList(folders, count);
	return result;
}


Ref<ProjectFolder> Project::GetFolderById(const std::string& id) const
{
	BNProjectFolder* folder = BNProjectGetFolderById(m_object, id.c_str());
	if (folder == nullptr)
		return nullptr;
	return new ProjectFolder(BNNewProjectFolderReference(folder));
}


void Project::PushFolder(Ref<ProjectFolder> folder)
{
	BNProjectPushFolder(m_object, folder->m_object);
}


void Project::DeleteFolder(Ref<ProjectFolder> folder)
{
	BNProjectDeleteFolder(m_object, folder->m_object);
}


void Project::PullFiles()
{
	BNProjectPullFiles(m_object);
}


Ref<ProjectFile> Project::CreateFile(Ref<FileMetadata> metadata, Ref<ProjectFolder> folder, const std::string& name, const std::string& description)
{
	BNProjectFile* file = BNProjectCreateFile(m_object, metadata->m_object, folder ? folder->m_object : nullptr, name.c_str(), description.c_str());
	if (file == nullptr)
		return nullptr;
	return new ProjectFile(BNNewProjectFileReference(file));
}


std::vector<Ref<ProjectFile>> Project::GetFiles() const
{
	size_t count;
	BNProjectFile** files = BNProjectGetFiles(m_object, &count);

	std::vector<Ref<ProjectFile>> result;
	result.reserve(count);
	for (size_t i = 0; i < count; i++)
	{
		result.push_back(new ProjectFile(BNNewProjectFileReference(files[i])));
	}

	BNFreeProjectFileList(files, count);
	return result;
}


Ref<ProjectFile> Project::GetFileById(const std::string& id) const
{
	BNProjectFile* file = BNProjectGetFileById(m_object, id.c_str());
	if (file == nullptr)
		return nullptr;
	return new ProjectFile(BNNewProjectFileReference(file));
}


void Project::PushFile(Ref<ProjectFile> file)
{
	BNProjectPushFile(m_object, file->m_object);
}


void Project::DeleteFile(Ref<ProjectFile> file)
{
	BNProjectDeleteFile(m_object, file->m_object);
}


ProjectFile::ProjectFile(BNProjectFile* file)
{
	m_object = file;
}


Ref<Project> ProjectFile::GetProject() const
{
	return new Project(BNProjectFileGetProject(m_object));
}


std::string ProjectFile::GetPathOnDisk() const
{
	char* path = BNProjectFileGetPathOnDisk(m_object);
	std::string result = path;
	BNFreeString(path);
	return result;
}


std::string ProjectFile::GetName() const
{
	char* name = BNProjectFileGetName(m_object);
	std::string result = name;
	BNFreeString(name);
	return result;
}


void ProjectFile::SetName(const std::string& name)
{
	BNProjectFileSetName(m_object, name.c_str());
}


std::string ProjectFile::GetId() const
{
	char* id = BNProjectFileGetId(m_object);
	std::string result = id;
	BNFreeString(id);
	return result;
}


Ref<ProjectFolder> ProjectFile::GetFolder() const
{
	BNProjectFolder* folder = BNProjectFileGetFolder(m_object);
	if (!folder)
		return nullptr;
	return new ProjectFolder(BNNewProjectFolderReference(folder));
}


void ProjectFile::SetFolder(Ref<ProjectFolder> folder)
{
	BNProjectFileSetFolder(m_object, folder ? folder->m_object : nullptr);
}


ProjectFolder::ProjectFolder(BNProjectFolder* folder)
{
	m_object = folder;
}


Ref<Project> ProjectFolder::GetProject() const
{
	return new Project(BNProjectFolderGetProject(m_object));
}


std::string ProjectFolder::GetId() const
{
	char* id = BNProjectFolderGetId(m_object);
	std::string result = id;
	BNFreeString(id);
	return result;
}


std::string ProjectFolder::GetName() const
{
	char* name = BNProjectFolderGetName(m_object);
	std::string result = name;
	BNFreeString(name);
	return result;
}


void ProjectFolder::SetName(const std::string& name)
{
	BNProjectFolderSetName(m_object, name.c_str());
}


Ref<ProjectFolder> ProjectFolder::GetParent() const
{
	BNProjectFolder* parent = BNProjectFolderGetParent(m_object);
	if (!parent)
		return nullptr;
	return new ProjectFolder(BNNewProjectFolderReference(parent));
}


void ProjectFolder::SetParent(Ref<ProjectFolder> parent)
{
	BNProjectFolderSetParent(m_object, parent ? parent->m_object : nullptr);
}
