﻿#include <hgl/File.h>
#include <hgl/LogInfo.h>
#include <hgl/Other.h>
#include <hgl/io/FileInputStream.h>
#include <hgl/io/FileOutputStream.h>

#include <windows.h>

#include <sys/types.h>
#include <string.h>

namespace hgl
{
	const int FILE_PROC_BUF_SIZE=HGL_SIZE_1MB;

	/**
	* 复制一个文件
	* @param sourcename 源文件名
	* @param targetname 目标文件名
	* @return 文件是否复制成功
	*/
	bool FileCopy(const OSString &sourcename,const OSString &targetname)
	{
		return(::CopyFileW(sourcename,targetname,false));
	}

	/**
	* 删除一个文件
	* @param filename 文件名
	* @return 文件是否成功删除
	*/
	bool FileDelete(const OSString &filename)
	{
		return(::DeleteFileW(filename));
	}

	/**
	* 移动一个文件
	* @param sourcename 源文件名
	* @param targetname 目标文件名
	* @return 文件是否移动成功
	*/
	bool FileMove(const OSString &sourcename,const OSString &targetname)
	{
		return(::MoveFileW(sourcename,targetname));
	}

	/**
	* 修改文件名
	* @param oldname 旧的文件名
	* @param newname 新的文件名
	* @return 文件名是否修改成功
	*/
	bool FileRename(const OSString &oldname,const OSString &newname)
	{
		return(::MoveFileW(oldname,newname));
	}

	/**
	* 确认文件是否存在
	* @param filename 要查找的文件名称
	* @return 这个文件是否存在
	*/
	bool FileConfirm(const OSString &filename)
	{
		WIN32_FIND_DATAW wfd;
		HANDLE hFind;

		hFind=FindFirstFileW(filename, &wfd);

		if(hFind==INVALID_HANDLE_VALUE)
			return(false);

		FindClose(hFind);
		return(true);
	}

	/**
	* 检测文件是否可读
	* @param filename 文件名
	*/
	bool FileCanRead(const OSString &filename)
	{
		const DWORD attr=GetFileAttributesW(filename);

		if(attr==INVALID_FILE_ATTRIBUTES)
			return(false);

		if(attr&(FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_DIRECTORY))	//不是个文件
			return(false);

		if(attr&FILE_ATTRIBUTE_OFFLINE)
			return(false);

		return(true);
	}

	/**
	* 检测文件是否可写
	* @param filename 文件名
	*/
	bool FileCanWrite(const OSString &filename)
	{
		const DWORD attr=GetFileAttributesW(filename);

		if(attr==INVALID_FILE_ATTRIBUTES)
			return(false);

		if(attr&(FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_DIRECTORY))	//不是个文件
			return(false);

		if(attr&FILE_ATTRIBUTE_OFFLINE)
			return(false);

		return (attr&FILE_ATTRIBUTE_READONLY)?false:true;
	}

	/**
	 * 检测文件是否可执行
	* @param filename 文件名
	 */
	bool FileCanExec(const OSString &filename)
	{
		int index = filename.FindRightChar('.');

		if (index == -1)return(false);

		if (index > filename.Length() - 4)
			return(false);

		const os_char *ext = filename.c_str() + index + 1;

		if (!ext)return(false);

		if (stricmp(ext, "exe") == 0)return(true);
		if (stricmp(ext, "com") == 0)return(true);
		if (stricmp(ext, "bat") == 0)return(true);
		if (stricmp(ext, "msi") == 0)return(true);
		if (stricmp(ext, "msp") == 0)return(true);

		return(false);
	}

	/**
	* 判断当前名称是否是一个目录
	* @param name 名称
	*/
	bool IsDirectory(const os_char *name)
	{
		DWORD attrib = GetFileAttributesW(name);

		if (attrib == INVALID_FILE_ATTRIBUTES)
			return(false);

		return attrib&FILE_ATTRIBUTE_DIRECTORY;
	}

	inline bool MakeDirectory(const OSString &name)
	{
		if(::CreateDirectoryW(name,nullptr))return(true);

		LOG_PROBLEM(OS_TEXT("创建目录<")+name+OS_TEXT(">失败！"));
		return(false);
	}

	/**
	* 创建一个子目录,该函数可自动创建多级目录。
	* @param dirname 目录名称
	* @return 目录是否创建成功
	*/
	bool MakePath(const OSString &dirname)
	{
		const os_char directory_separator=HGL_DIRECTORY_SEPARATOR;
		os_char *p;

		os_char str[HGL_MAX_PATH];
		os_char *sp;

		strcpy(str,HGL_MAX_PATH,dirname.c_str());

		sp=str;

		while(1)
		{
			p=hgl::strchr(sp+1,directory_separator);				// unix下有可能第一个字符就是分隔符，所以必须用sp+1开始查找

			if(p)
				*p=0;

            if(*sp==0)
                return(true);

			if(!IsDirectory(str))//没有找到
				if(!MakeDirectory(str))
					return(false);

            if(p)
                *p++=directory_separator;
            else
                return(true);

            sp=p;
		}
	}

	/**
	* 删除一个子目录
	* @param name 目录名称
	* @return 目录是否删除成功
	*/
	bool DeletePath(const OSString &name)
	{
		return(RemoveDirectoryW(name));
	}

	/**
	* 取得当前所在目录<br>
	*/
	bool GetCurrentPath(OSString &path)
	{
		int len;
		u16char *dir;

		len=GetCurrentDirectoryW(0,nullptr);

		if(len==0)
			return(nullptr);

		dir=new u16char[len+1];

		if(GetCurrentDirectoryW(len,dir))
		{
			if(len==3&&dir[1]==OS_TEXT(':'))
				len=2;        //如果是"C:\"这种情况，去掉"\"

			dir[len]=0;

			path.Set(dir,len,true);
			return(true);
		}

		delete[] dir;
		return(false);
	}

	/**
	 * 取得当前程序完整路径名称
	 */
	bool GetCurrentProgram(OSString &result)
	{
		os_char *path=new os_char[HGL_MAX_PATH];

		GetModuleFileNameW(nullptr,path,HGL_MAX_PATH);

		result = path;
		delete[] path;

        return(true);
	}

	/**
	* 枚举一个目录内的所有文件
	* @param folder_name 目录名称
	* @param find_name 查找用名称
	* @param data 自定义回传数据
	* @param proc_folder 处理目录
	* @param proc_file 处理文件
	* @param sub_folder 是否处理子目录
	* @param func 回调函数
	* @return 查找到文件数据,-1表示失败
	*/
	int EnumFile(const OSString &folder_name,const OSString &find_name,void *data,bool proc_folder,bool proc_file,bool sub_folder,EnumFileFunc func)
	{
		OSString full_name;
		int count=0;

		if(!func)return(-1);

		if(!find_name||!(*find_name))
			return(-1);

		if(!folder_name||!(*folder_name))
		{
			full_name=find_name;
		}
		else
		{
			full_name=folder_name;

			if(folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
                full_name+=HGL_DIRECTORY_SEPARATOR;

			full_name+=find_name;
		}

		WIN32_FIND_DATAW FindFileData;
		HANDLE hFind;

		hFind = FindFirstFileW(full_name, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
			return(-1);

		do
		{
			if(strcmp(FindFileData.cFileName,OS_TEXT("."))==0
			|| strcmp(FindFileData.cFileName,OS_TEXT("..")) == 0)
			{
				continue;
			}

			if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if(proc_folder)
				{
					if(sub_folder)
					{
						OSString child_name=folder_name;

						if(folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
							child_name+=HGL_DIRECTORY_SEPARATOR;

						child_name+=FindFileData.cFileName;
						count+=EnumFile(child_name,find_name,data,proc_folder,proc_file,true,func);
					}
				}
				else
					continue;
			}
			else
			{
				if(!proc_file)continue;
			}

			count++;

			FileInfo fi;
			memset(&fi,0,sizeof(FileInfo));

			strcpy(fi.name,HGL_MAX_PATH,FindFileData.cFileName);

			if(!folder_name||!(*folder_name))
			{
            	strcpy(fi.fullname, HGL_MAX_PATH, fi.name);
			}
			else
			{
				strcpy(fi.fullname, HGL_MAX_PATH, folder_name);

				if(folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
					strcat(fi.fullname, HGL_MAX_PATH, HGL_DIRECTORY_SEPARATOR);

				const int rp = find_name.FindChar(HGL_DIRECTORY_SEPARATOR);//防止查询名称内仍有路径

				if(rp!=-1)
					strcat(fi.fullname, HGL_MAX_PATH, find_name.c_str(),rp);

				strcat(fi.fullname, HGL_MAX_PATH, fi.name, HGL_MAX_PATH);
			}

			fi.size =	FindFileData.nFileSizeHigh;
			fi.size <<= 32;
			fi.size |=	FindFileData.nFileSizeLow;

			if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				fi.is_file=false;
				fi.is_directory=true;
			}
			else
			{
				fi.is_file=true;
				fi.is_directory=false;
			}

			fi.can_read=true;
			fi.can_write=!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_READONLY);

			func(data,fi);
		}
		while(FindNextFileW(hFind, &FindFileData));

		FindClose(hFind);

		return(count);
	}

	/**
	* 枚举一个目录内的所有文件
	* @param folder_name 目录名称
	* @param find_name 查找用名称
	* @param data 自定义回传数据
	* @param func 回调函数
	* @return 查找到文件数据,-1表示失败
	*/
	int EnumFile(const u16char *folder_name,const u16char *find_name,void *data,EnumFileFunc func)
	{
		return EnumFile(folder_name,find_name,data,true,true,false,func);
	}

	/**
	* 枚举当前计算机所有卷
	* @param data 用户自定义回传信息
	* @param func 回调函数
	* @param check_cd 检测光盘
	* @return 查找到的卷数量，-1表示失败
	*/
	int EnumVolume(void *data,void (__cdecl *func)(void *,hgl::VolumeInfo &),bool check_removable,bool check_remote,bool check_cd)
	{
		HANDLE handle;
		u16char volume_name[HGL_MAX_PATH+1];
		u16char path_name[HGL_MAX_PATH];
		int count=0;

		handle=FindFirstVolumeW(volume_name,HGL_MAX_PATH);

		if(handle==INVALID_HANDLE_VALUE)return(-1);

		do
		{
			hgl::VolumeInfo vi;

			memset(&vi,0,sizeof(hgl::VolumeInfo));

			DWORD length;

			GetVolumePathNamesForVolumeNameW(volume_name,path_name,HGL_MAX_PATH,&length);				//这个函数要win xp/2003才能用

			path_name[length]=0;

			strcpy(vi.name, HGL_MAX_PATH,volume_name);
			strcpy(vi.path, HGL_MAX_PATH,path_name);

			UINT type=GetDriveTypeW(path_name);

			if(type==DRIVE_REMOVABLE){if(!check_removable)continue;	vi.driver_type=hgl::VolumeInfo::dtRemovable;}else
			if(type==DRIVE_FIXED	){								vi.driver_type=hgl::VolumeInfo::dtFixed;	}else
			if(type==DRIVE_REMOTE	){if(!check_remote	 )continue;	vi.driver_type=hgl::VolumeInfo::dtRemote;	}else
			if(type==DRIVE_RAMDISK	){								vi.driver_type=hgl::VolumeInfo::dtRamDisk;	}else
			if(type==DRIVE_CDROM	){if(!check_cd		 )continue;	vi.driver_type=hgl::VolumeInfo::dtCDROM;	}else
				vi.driver_type=hgl::VolumeInfo::dtNone;

			uint32 file_system_flags;

			if(GetVolumeInformationW(path_name,
									vi.volume_label,
									255,
									(unsigned long *)&vi.serial,
									(unsigned long *)&vi.filename_max_length,
									(unsigned long *)&file_system_flags,
									vi.file_system,
									255))
			{
				vi.unicode=file_system_flags&FILE_UNICODE_ON_DISK;
			}
			else
				LOG_PROBLEM(U16_TEXT("取得卷<") + UTF16String(path_name) + U16_TEXT(">信息失败！Windows错误编号: ") + UTF16String((uint)GetLastError()));

			if(GetDiskFreeSpaceExW(	path_name,
									(ULARGE_INTEGER *)&vi.available_space,
									(ULARGE_INTEGER *)&vi.total_space,
									(ULARGE_INTEGER *)&vi.free_space))
			{
				func(data,vi);
			}
			else
				LOG_PROBLEM(U16_TEXT("取得驱动器<") + UTF16String(path_name) + U16_TEXT(">容量数据失败！"));

			count++;

		}while(FindNextVolumeW(handle,volume_name,HGL_MAX_PATH));

		FindVolumeClose(handle);

		return(count);
	}
}//namespace hgl
