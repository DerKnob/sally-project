////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\Box2DObject.cpp
///
/// \brief	Implements the box 2 d object class. 
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

#include "Box2DObject.h"

CBox2DObject::CBox2DObject(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CImageBox(parent, x, y, width, height, controlId),
	m_pb2Body(NULL), m_pb2World(NULL), m_pb2MouseJoint(NULL)
{
}

CBox2DObject::~CBox2DObject(void)
{
}

bool CBox2DObject::CreateBox2DObject(b2World* world)
{
	if (m_pb2Body != NULL)
		return false;

	m_pb2World = world;

	// create body
	b2BodyDef bodyDef;
	float posX = (float) (m_iX + (m_iWidth / 2));
	float posY = (float) (m_iY + (m_iHeight / 2)) * -1;

	bodyDef.position.Set(posX, posY);
	bodyDef.massData.mass = 50.0f;
	bodyDef.linearDamping = 0.0f;
	bodyDef.angularDamping = 0.0f;
	m_pb2Body = m_pb2World->CreateBody(&bodyDef);

	// create body def
	b2PolygonDef shapeDef;
	float width = (float) (m_iWidth / 2);
	float height = (float) (m_iHeight / 2);

	shapeDef.SetAsBox(width, height);
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.3f;

	// asign shape to body
	m_pb2Body->CreateShape(&shapeDef);
	m_pb2Body->SetMassFromShapes();

	return true;
}

bool CBox2DObject::MoveAndHold(int x, int y)
{
	bool moving = false;

	if (m_pb2Body != NULL)
	{
		if (!m_pb2Body->IsSleeping())
			moving = true;
	}

	Move(x, y);

	if (m_pb2Body != NULL)
	{
		float posX = (float) m_iXAbsolut + (m_iWidth / 2);
		float posY = (float) (m_iYAbsolut + (m_iHeight / 2)) * -1;

		m_pb2Body->PutToSleep();

		b2Vec2 vec(posX, posY);
		m_pb2Body->SetXForm(vec, 0);

		m_pb2Body->WakeUp();
	}
	return moving;
}

void CBox2DObject::Timer(float timeDelta)
{
	SallyAPI::GUI::CImageBox::Timer(timeDelta);

	if (m_pb2Body != NULL)
	{
		/*m_pb2Body->WakeUp();*/

		b2Vec2 position1 = m_pb2Body->GetPosition();
		float32 angle1 = m_pb2Body->GetAngle();
		int posX = (int) position1.x - (m_iWidth / 2);
		int posY = (int) (position1.y  + (m_iHeight / 2)) * -1;
		Move(posX, posY);
		SetRotationAngleZ(angle1);
	}
}

bool CBox2DObject::ProcessMouseMove(int x, int y)
{
	if (m_pb2Body != NULL)
	{
		if (m_pb2MouseJoint != NULL)
		{
			float moveX = (float) x;
			float moveY = (float) y * -1;
			b2Vec2 p(moveX, moveY);
			m_pb2MouseJoint->SetTarget(p);
		}
	}
	return SallyAPI::GUI::CImageBox::ProcessMouseMove(x, y);
}

bool CBox2DObject::ProcessMouseUp(int x, int y)
{
	DeleteMouseJoint();
	return SallyAPI::GUI::CImageBox::ProcessMouseUp(x, y);
}

void CBox2DObject::DeleteMouseJoint()
{
	if (m_pb2Body != NULL)
	{
		if (m_pb2MouseJoint != NULL)
		{
			m_pb2World->DestroyJoint(m_pb2MouseJoint);
			m_pb2MouseJoint = NULL;
		}
	}
}

bool CBox2DObject::ProcessMouseDown(int x, int y)
{
	if (m_pb2Body != NULL)
	{
		if (m_pb2MouseJoint == NULL)
		{
			float posX = (float) x;
			float posY = (float) y * -1;
			b2Vec2 p(posX, posY);

			b2MouseJointDef jointDef;
			jointDef.body1 = m_pb2World->GetGroundBody();
			jointDef.body2 = m_pb2Body;
			jointDef.target = p;
			jointDef.timeStep = 30;
			jointDef.dampingRatio = 1;
			jointDef.maxForce = 30000.0f * m_pb2Body->GetMass();

			m_pb2MouseJoint = (b2MouseJoint*) m_pb2World->CreateJoint(&jointDef);
		}
	}
	return SallyAPI::GUI::CImageBox::ProcessMouseDown(x, y);
}

void CBox2DObject::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_MOUSE_UP_INFO:
		DeleteMouseJoint();
		break;
	}
	SallyAPI::GUI::CImageBox::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}