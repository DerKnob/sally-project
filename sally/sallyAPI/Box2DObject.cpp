////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Box2DObject.cpp
///
/// \brief	Implements the box 2 d object class. 
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
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Box2DObject.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CBox2DObject::CBox2DObject(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CImageBox(parent, x, y, width, height, controlId),
/// m_pb2Body(NULL), m_pb2World(NULL), m_pb2MouseJoint(NULL), m_pb2Shape(NULL)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CBox2DObject::CBox2DObject(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CImageBox(parent, x, y, width, height, controlId),
	m_pb2Body(NULL), m_pb2World(NULL), m_pb2MouseJoint(NULL), m_pb2Shape(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CBox2DObject::~CBox2DObject()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CBox2DObject::~CBox2DObject()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CBox2DObject::CreateBox2DObject(b2World* world)
///
/// \brief	Creates a box 2 d object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	world	If non-null, the world. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CBox2DObject::CreateBox2DObject(b2World* world)
{
	if (m_pb2Body != NULL)
		return false;

	m_pb2World = world;

	// create body
	b2BodyDef bodyDef;
	float posX = (float) (m_iXAbsolut + (m_iWidth / 2));
	float posY = (float) (m_iYAbsolut + (m_iHeight / 2)) * -1;

	bodyDef.position.Set(posX, posY);
	bodyDef.massData.mass = 50.0f;
	bodyDef.linearDamping = 0.0f;
	bodyDef.angularDamping = 0.0f;
	m_pb2Body = m_pb2World->CreateBody(&bodyDef);

	CreateShape();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBox2DObject::CreateShape()
///
/// \brief	Creates the shape. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CBox2DObject::CreateShape()
{
	if (m_pb2Body != NULL)
	{
		if (m_pb2Shape != NULL)
		{
			m_pb2Body->DestroyShape(m_pb2Shape);
			m_pb2Shape = NULL;
		}

		// create body def
		b2PolygonDef shapeDef;
		float width = (float) (m_iWidth / 2);
		float height = (float) (m_iHeight / 2);

		shapeDef.SetAsBox(width, height);
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.3f;

		// asign shape to body
		m_pb2Shape = m_pb2Body->CreateShape(&shapeDef);
		m_pb2Body->SetMassFromShapes();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CBox2DObject::MoveAndHold(int x, int y)
///
/// \brief	Move and hold. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

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
	}
	return moving;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBox2DObject::ResizeInternal(int width, int height)
///
/// \brief	Resize internal. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CBox2DObject::ResizeInternal(int width, int height)
{
	SallyAPI::GUI::CImageBox::ResizeInternal(width, height);

	CreateShape();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBox2DObject::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CBox2DObject::Timer(float timeDelta)
{
	SallyAPI::GUI::CImageBox::Timer(timeDelta);

	if (m_pb2Body != NULL)
	{
		b2Vec2 position = m_pb2Body->GetPosition();
		float32 angle1 = m_pb2Body->GetAngle();
		int posX = (int) position.x - (m_iWidth / 2);
		int posY = (int) (position.y  + (m_iHeight / 2)) * -1;

		int xParent = 0;
		int yParent = 0;
		m_pParent->GetAbsolutPosition(&xParent, &yParent);

		Move(posX - xParent, posY - yParent);
		SetRotationAngleZ(angle1);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CBox2DObject::ProcessMouseMove(int x, int y)
///
/// \brief	Process a mouse move. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CBox2DObject::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CBox2DObject::ProcessMouseUp(int x, int y)
{
	DeleteMouseJoint();
	return SallyAPI::GUI::CImageBox::ProcessMouseUp(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBox2DObject::DeleteMouseJoint()
///
/// \brief	Deletes a mouse joint. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CBox2DObject::ProcessMouseDown(int x, int y)
///
/// \brief	Process a mouse down. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBox2DObject::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CBox2DObject::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_UPDATE_ABSOLUTE_POSITION:
		SallyAPI::GUI::CImageBox::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);

		if (m_pb2Body != NULL)
		{
			float posX = (float) m_iXAbsolut + (m_iWidth / 2);
			float posY = (float) (m_iYAbsolut + (m_iHeight / 2)) * -1;

			b2Vec2 vec(posX, posY);
			m_pb2Body->SetXForm(vec, 0);
		}
		return;
	case GUI_MESSAGE_MOUSE_UP_INFO:
		DeleteMouseJoint();
		break;
	}
	SallyAPI::GUI::CImageBox::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}