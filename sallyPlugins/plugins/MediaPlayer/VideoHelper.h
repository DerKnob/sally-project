////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\VideoHelper.h
///
/// \brief	Declares the video helper class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Define.h"
#include <sallyAPI\sallyAPI.h>
#include "VideoFile.h"
#include "MediaDatabase.h"

class CVideoHelper :
	public SallyAPI::System::CThread
{
private:
	SallyAPI::GUI::CAppBase*	m_pMainWindow;
	CVideoFile*						m_pVideo;

	virtual void	RunEx();
public:
	CVideoHelper();
	virtual ~CVideoHelper();

	void SetStaticValues(SallyAPI::GUI::CAppBase* control);
	void SetValues(CVideoFile* video);
};
