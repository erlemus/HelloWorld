#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "renderer.h"
#include "shader.h"
#include "buffer.h"


void Run( GLFWwindow* window )
{
   std::cout << "OPENGL VERSION" << std::endl;
   std::cout << "  GL report: " << glGetString( GL_VERSION ) << std::endl;
   std::cout << "  GLFW report: " << glfwGetVersionString() << std::endl << std::endl;

   constexpr GLuint nverts = 4;
   GLfloat verts[ nverts ][ 2 ] = { {-0.5f,-0.5f },
   { 0.5f,-0.5f },
   { 0.5f, 0.5f },
   {-0.5f, 0.5f } };
   constexpr GLuint nidxs = 2;
   GLuint idxs[ nidxs ][ 3 ] = { { 0, 1, 2 }, { 2, 3, 0 } };

   GLfloat primitive_color[] = { 1.0f, 0.0f, 0.0f, 1.0f };

   // Init
   Renderer renderer;

   VertexArray va;
   VertexBuffer vb( verts, static_cast< unsigned int >( nverts * 2U * sizeof( GLfloat ) ) );
   VertexBufferLayout layout;
   layout.Push< float >( 2 );
   va.AddBuffer( vb, layout );

   IndexBuffer ib( idxs, nidxs * 3 );

   Shader shader( "resources/shaders/basic.shader" );
   shader.Bind();

   // index of the color channel to be modified (R[0], G[1], B[2])
   GLuint channelIdx = 0;

   // Size of the increment/decrement to be made to the current color channel.
   // This value is choose so transitions to each color stop in the sequence
   // Red->Yellow->Green->Cyan->Blue->Magenta will take ~3 secs @60Hz.
   GLfloat step = 0.0055f;

   // Main rendering loop until the user closes the window
   while( !glfwWindowShouldClose( window ) )
   {
      // Clear color buffer
      renderer.Clear();

      // Render here
      // 1. Call to draw using the current color
      shader.Bind();
      shader.SetUniform4f( "u_Color", primitive_color[ 0 ], primitive_color[ 1 ], primitive_color[ 2 ], primitive_color[ 3 ] );
      renderer.Draw( va, ib, shader );

      // 2. Modify the color for the next Draw call increment/decrement'ing the value of the current channel
      auto& cc = primitive_color[ channelIdx ];
      cc += step;

      //   if we are (a)increasing the channel value and we get to the max val; or (b)decreasing the channel value and we get to the min val
      //   then we need to switch to the previous mod3 channel index to continue increasing/decreasing using the 'step' with a flipped sign.
      if( cc <= 0.0f || cc >= 1.0f )
      {
         cc = ( cc <= 0.0f ) ? 0.0f : 1.0f;
         channelIdx = ( channelIdx + 2 ) % 3;
         step = -step;
      }

      // Swap front and back buffers
      glfwSwapBuffers( window );

      // Poll for and process events
      glfwPollEvents();
   }
}


int main( void )
{
   // Initialize the library
   if( !glfwInit() )
      return EXIT_FAILURE;

   // Create OpenGL 3.3 core profile
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

   // Create a windowed mode window and its OpenGL context
   GLFWwindow* window = glfwCreateWindow( 640, 480, "Hello OpenGL!", NULL, NULL );
   if( !window )
   {
      glfwTerminate();
      return EXIT_FAILURE;
   }

   // Make the window's context current
   glfwMakeContextCurrent( window );

   // Normally refresh @60Hz
   glfwSwapInterval( 1 );

   // Initialize GLEW after creating a valid OpenGL context
   if( glewInit() != GLEW_OK )
   {
      glfwTerminate();
      return EXIT_FAILURE;
   }

   // GL scope runner
   Run( window );

   glfwTerminate();
   return EXIT_SUCCESS;
}

