#include <Windows.h>
#include <list>

#include <gl/GL.h>
#pragma comment(lib, "opengl32")

#include "minhook/include/minhook.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// Pointers to original functions
decltype(&glOrtho) fn_glOrtho = &glOrtho;
decltype(&glScalef) fn_glScalef = &glScalef;
decltype(&glTranslatef) fn_glTranslatef = &glTranslatef;

struct Object
{
	enum Type
	{
		Entity,
		Chest,
		LargeChest
	};

	Type m_type;

	glm::mat4 m_projection;
	glm::mat4 m_modelview;

	Object(Type type) : m_type { type }
	{
		// Automatically fill it with the data of the current matrices
		glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(m_projection));
		glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(m_modelview));
	}
};

std::list<Object> objects;

// This function draws a 3D box outline from the current
// coordinates with offsets of -1 and 1 in each dimension
void drawBox(glm::vec4 color)
{
	glColor4fv(glm::value_ptr(color));
	
	glBegin(GL_LINES);
	{
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);

		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);

		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);

		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);

		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);

		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);

		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
	}
	glEnd();
}

// Hooked glOrtho function
void WINAPI hk_glOrtho(
	GLdouble left, GLdouble right,
	GLdouble bottom, GLdouble top,
	GLdouble zNear, GLdouble zFar)
{
	// Calling the original glOrtho function
	fn_glOrtho(left, right, bottom, top, zNear, zFar);

	// Called at the beginning of the GUI rendering
	if (zNear == 1000.0 and zFar == 3000.0)
	{
		if (objects.empty())
			return;

		// Saving all attributes
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		// Saving matrices
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		// Setting up the context
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_LINE_SMOOTH);
		glLineWidth(2.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (auto& object : objects)
		{
			glm::mat4& modelview = object.m_modelview;
			glm::mat4& projection = object.m_projection;

			glm::vec4 color = glm::vec4(1.0f);
			glm::vec3 translate = glm::vec3(1.0f);
			glm::vec3 scale = glm::vec3(1.0f);

			switch (object.m_type)
			{
				case Object::Entity:
					color = glm::vec4(0.18f, 1.0f, 0.52f, 1.0f);
					translate = glm::vec3(0.0f, -1.0f, 0.0f);
					scale = glm::vec3(0.5f, 1.0f, 0.5f);
				break;

				case Object::Chest:
					color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
					translate = glm::vec3(0.5f, 0.5f, 0.5f);
					scale = glm::vec3(0.5f, 0.5f, 0.5f);
				break;

				case Object::LargeChest:
					color = glm::vec4(0.91f, 0.87f, 0.42f, 1.0f);
					translate = glm::vec3(1.0f, 0.5f, 0.5f);
					scale = glm::vec3(1.0f, 0.5f, 0.5f);
				break;
			}

			// Transforming matrices (shifting a point to the center of object)
			modelview = glm::translate(modelview, translate);
			modelview = glm::scale(modelview, scale);

			// Setup matrices
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(glm::value_ptr(projection));

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(modelview));

			// Drawing a box from the current coordinates (matrixes)
			drawBox(color);
		}

		objects.clear();

		// Restoring matrices
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		// Restoring all attributes
		glPopAttrib();
	}
}

// Hooked glScalef function
void WINAPI hk_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	// Called at the beginning of drawing an entity (player, villager, witch)
	if (x == 0.9375f and y == 0.9375f and z == 0.9375f)
		objects.emplace_back(Object::Entity);

	// Calling the original glScalef function
	fn_glScalef(x, y, z);
}

// Hooked glTranslatef function
void WINAPI hk_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	// Called at the beginning of drawing a chest or ender chest
	if (x == 0.5f and y == 0.4375f and z == 0.9375f)
		objects.emplace_back(Object::Chest);

	// Called at the beginning of drawing a large chest
	else if (x == 1.0f and y == 0.4375f and z == 0.9375f)
		objects.emplace_back(Object::LargeChest);

	// Calling the original glTranslatef function
	fn_glTranslatef(x, y, z);
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		MH_Initialize();
		MH_CreateHook(&glOrtho, &hk_glOrtho,
			reinterpret_cast<void**>(&fn_glOrtho));
		MH_CreateHook(&glScalef, &hk_glScalef,
			reinterpret_cast<void**>(&fn_glScalef));
		MH_CreateHook(&glTranslatef, &hk_glTranslatef,
			reinterpret_cast<void**>(&fn_glTranslatef));
		return MH_EnableHook(MH_ALL_HOOKS) == MH_OK;

	case DLL_PROCESS_DETACH:
		MH_Uninitialize();
	}

	return TRUE;
}