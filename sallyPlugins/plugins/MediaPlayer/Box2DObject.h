////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\Box2DObject.h
///
/// \brief	Declares the box 2 d object class. 
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
#include <sallyAPI\sallyAPI.h>
#include "Define.h"
#include <Box2D\Box2D.h>

#pragma comment(lib, "box2d.lib")

class CBox2DObject:
	public SallyAPI::GUI::CImageBox
{
private:
	b2World*		m_pb2World;
	b2Body*			m_pb2Body;
	b2MouseJoint*	m_pb2MouseJoint;
	
	void DeleteMouseJoint();

	virtual bool ProcessMouseMove(int x, int y);
	virtual bool ProcessMouseUp(int x, int y);
	virtual bool ProcessMouseDown(int x, int y);
public:
	CBox2DObject(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId = 0);
	~CBox2DObject();

	virtual void SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

	bool	CreateBox2DObject(b2World* world);
	b2Body*	GetBox2DObject();

	virtual void	Timer(float timeDelta);
	bool			MoveAndHold(int x, int y);
};