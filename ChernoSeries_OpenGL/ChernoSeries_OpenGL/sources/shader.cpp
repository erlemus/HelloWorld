#include <GL/glew.h>
#include <unordered_map>

#include <fstream>
#include <iostream>
#include <sstream>
#include "renderer.h"
#include "shader.h"


Shader::Shader( const std::string& filepath )
   : m_shaderID( 0 ), m_filePath( filepath )
{
   ShaderProgramSource shaderSrc = ParseShader();
   std::cout << "VERTEX SHADER SOURCE" << std::endl << shaderSrc.vertShaderSrc << std::endl;
   std::cout << "FRAGMENT SHADER SOURCE" << std::endl << shaderSrc.fragShaderSrc << std::endl;
   m_shaderID = CreateShaders( shaderSrc.vertShaderSrc, shaderSrc.fragShaderSrc );
}


Shader::~Shader()
{
   GLCall( glDeleteProgram( m_shaderID ) );
}


void Shader::Bind() const
{
   GLCall( glUseProgram( m_shaderID ) );
}


void Shader::Unbind() const
{
   GLCall( glUseProgram( 0 ) );
}


Shader::ShaderProgramSource Shader::ParseShader()
{
   enum class ShaderType
   {
      NONE = -1,
      VERTEX = 0,
      FRAGMENT = 1,
   };

   std::string line;
   std::stringstream ss[ 2 ];
   ShaderType type = ShaderType::NONE;

   std::ifstream stream( m_filePath );
   while( getline( stream, line ) )
   {
      if( line.find( "#shader" ) != std::string::npos )
      {
         type = line.find( "vertex" ) != std::string::npos ? ShaderType::VERTEX :
            line.find( "fragment" ) != std::string::npos ? ShaderType::FRAGMENT : ShaderType::NONE;
      }
      else
      {
         ss[ static_cast< int >( type ) ] << line << std::endl;
      }
   }

   return { ss[ static_cast< int >( ShaderType::VERTEX ) ].str(),
      ss[ static_cast< int >( ShaderType::FRAGMENT ) ].str() };
}


unsigned int Shader::CompileShader( GLenum type, const std::string& source )
{
   GLuint id = glCreateShader( type );
   const char* src = source.c_str();
   GLCall( glShaderSource( id, 1, &src, NULL ) );
   GLCall( glCompileShader( id ) );

   GLint result;
   GLCall( glGetShaderiv( id, GL_COMPILE_STATUS, &result ) );
   if( result == GL_FALSE )
   {
      int length;
      GLCall( glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length ) );
      char* infoLog = ( char* ) alloca( length * sizeof( char ) );
      GLCall( glGetShaderInfoLog( id, length, &length, infoLog ) );
      std::cout << "Failed to compile shader (type:" << type << "  id:" << id << ")" << std::endl << infoLog << std::endl;
      GLCall( glDeleteShader( id ) );
      id = 0;
   }

   return id;
}


unsigned int Shader::CreateShaders( const std::string& vertShader, const std::string& fragShader )
{
   GLuint program = glCreateProgram();

   // Compile shaders
   GLuint vs = CompileShader( GL_VERTEX_SHADER, vertShader );
   GLuint fs = CompileShader( GL_FRAGMENT_SHADER, fragShader );

   // Link shaders
   GLCall( glAttachShader( program, vs ) );
   GLCall( glAttachShader( program, fs ) );
   GLCall( glLinkProgram( program ) );
   GLCall( glValidateProgram( program ) );

   // Brief clean up (keep them attached for debugging purposes)
   GLCall( glDeleteShader( vs ) );
   GLCall( glDeleteShader( fs ) );

   return program;
}


void Shader::SetUniform1f( const std::string& name, float v0 )
{
   int location = m_uniformLocCache.find( name ) == m_uniformLocCache.end() ?
      glGetUniformLocation( m_shaderID, name.c_str() ) : m_uniformLocCache[ name ];
   ASSERT( location != -1 );
   m_uniformLocCache[ name ] = location;
   GLCall( glUniform1f( location, v0 ) );
}


void Shader::SetUniform2f( const std::string& name, float v0, float v1 )
{
   int location = m_uniformLocCache.find( name ) == m_uniformLocCache.end() ?
     glGetUniformLocation( m_shaderID, name.c_str() ) : m_uniformLocCache[ name ];
   ASSERT( location != -1 );
   m_uniformLocCache[ name ] = location;
   GLCall( glUniform2f( location, v0, v1 ) );
}


void Shader::SetUniform3f( const std::string& name, float v0, float v1, float v2 )
{
   int location = m_uniformLocCache.find( name ) == m_uniformLocCache.end() ?
      glGetUniformLocation( m_shaderID, name.c_str() ) : m_uniformLocCache[ name ];
   ASSERT( location != -1 );
   m_uniformLocCache[ name ] = location;
   GLCall( glUniform3f( location, v0, v1, v2 ) );
}


void Shader::SetUniform4f( const std::string& name, float v0, float v1, float v2, float v3 )
{
   int location = m_uniformLocCache.find( name ) == m_uniformLocCache.end() ?
      glGetUniformLocation( m_shaderID, name.c_str() ) : m_uniformLocCache[ name ];
   ASSERT( location != -1 );
   m_uniformLocCache[ name ] = location;
   GLCall( glUniform4f( location, v0, v1, v2, v3 ) );
}
