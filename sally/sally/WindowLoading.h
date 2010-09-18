////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\WindowLoading.h
///
/// \brief	Declares the window loading class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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

using namespace SallyAPI::GUI;

class CWindowLoading
	: public CWindow
{
protected:
	SallyAPI::GUI::CLabel*		m_pStatus;
	SallyAPI::GUI::CProcessbar* m_pProcessbar;

	virtual void	RenderControl();
public:
	CWindowLoading();
	virtual ~CWindowLoading();

	void	SetStatus(const std::string& status);
	void	SetProcessbar(int i);
	void	UpdateProcessbar();
};