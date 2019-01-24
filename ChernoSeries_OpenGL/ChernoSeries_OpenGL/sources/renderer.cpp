#include <GL/glew.h>
#include <iostream>
#include "buffer.h"
#include "renderer.h"
#include "shader.h"


bool GLErrorCheck( const char* function, const char* file, int line )
{
   bool fSucceeded = true;
   while( GLenum error = glGetError() )
   {
      std::cout << "GLerror (code 0x" << std::hex << error << ") in \"" << function << "\" at " << file << ":" << std::dec << line << std::endl;
      fSucceeded = false;
   }
   return fSucceeded;
}


void GLErrorClear()
{
   while( glGetError() != GL_NO_ERROR );
}

void Renderer::Clear() const
{
   glClear( GL_COLOR_BUFFER_BIT );
}


void Renderer::Draw( const VertexArray & va, const IndexBuffer & ib, const Shader & shader ) const
{
   shader.Bind();
   va.Bind();
   ib.Bind();
   GLCall( glDrawElements( GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, NULL ) );

#if _DEBUG
   shader.Unbind();
   va.Unbind();
   ib.Unbind();
#endif
}
