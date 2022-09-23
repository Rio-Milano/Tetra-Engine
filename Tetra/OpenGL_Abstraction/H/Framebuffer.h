#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include<glad/glad.h>
#include<crtdbg.h>
#include"glmIncludes.h"

class Framebuffer
{
public:
	explicit Framebuffer(const glm::vec<2, int>& viewPortSize)
	{
		//create the framebuffer
		glGenFramebuffers(1, &m_frameBufferID);

		//bind to the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

		/*
		*
		<--			Color Buffer			-->

		*/

		//create a texture attachment for the color buffer as we want to read from the texture
		glGenTextures(1, &m_colourBufferID);
		//bind to default texture unit
		glActiveTexture(GL_TEXTURE0);
		//bind texture buffer to 2d texture target
		glBindTexture(GL_TEXTURE_2D, m_colourBufferID);
		//assign memory on gpu for color buffer
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewPortSize.x, viewPortSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		//set texture sampeling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//attach the texture as a color buffer to the framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourBufferID, 0);
		//unbind texure from target
		glBindTexture(GL_TEXTURE_2D, 0);

		/*
		*
		<-			Depth and Stencil Buffer

		*/

		//create a render buffer object for stencil and depth testing
		glGenRenderbuffers(1, &m_depthStencilRenderbufferID);
		//bind to renderbuffer object
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRenderbufferID);
		//create memory for the render buffer
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewPortSize.x, viewPortSize.y);
		//attach the render buffer to the frame buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilRenderbufferID);
		//unbind from render buffer object
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		/*
		*
		* Check framebuffer is complete

		*/

		//check that framebuffer is complete if not trigger break point
		_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		//set framebuffer back to default
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	};

	~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_frameBufferID);
		glDeleteRenderbuffers(1, &m_depthStencilRenderbufferID);
		glDeleteTextures(1, &m_colourBufferID);
	};

	void SetFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
		ClearFramebuffer();
	};

	static void ResetFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const GLuint& GetColorBufferID()const
	{
		return m_colourBufferID;
	}




private:
	void ClearFramebuffer()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	GLuint m_frameBufferID{0};
	GLuint m_colourBufferID{0};
	GLuint m_depthStencilRenderbufferID{0};
};

#endif