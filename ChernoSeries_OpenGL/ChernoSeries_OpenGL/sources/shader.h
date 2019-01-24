#pragma once

#include <string>
#include <unordered_map>

// Forward declare
typedef unsigned int GLenum;


// ------------------------------------------------------------------------------------------------
// class Shader
// 
// This is a basic abstraction to vertex and fragment shaders
// ------------------------------------------------------------------------------------------------
class Shader
{
public:
   Shader( const std::string& filepath );
   virtual ~Shader();

   void Bind() const;
   void Unbind() const;

   void SetUniform1f( const std::string& name, float v0 );
   void SetUniform2f( const std::string& name, float v0, float v1 );
   void SetUniform3f( const std::string& name, float v0, float v1, float v2 );
   void SetUniform4f( const std::string& name, float v0, float v1, float v2, float v3 );

private:
   struct ShaderProgramSource
   {
      std::string vertShaderSrc;
      std::string fragShaderSrc;
   };

   ShaderProgramSource ParseShader();
   unsigned int CompileShader( GLenum type, const std::string& source );
   unsigned int CreateShaders( const std::string& vertShader, const std::string& fragShader );

   unsigned int m_shaderID;
   std::string m_filePath;
   std::unordered_map< std::string, int > m_uniformLocCache;
}
;