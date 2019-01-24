#pragma once

// Forward declare
class IndexBuffer;
class VertexArray;
class Shader;


// ------------------------------------------------------------------------------------------------
//   Error handling
// ------------------------------------------------------------------------------------------------
#if _DEBUG
#define ASSERT( x ) if( !( x ) ) __debugbreak();
#define GLCall( x ) GLErrorClear(); x; ASSERT( GLErrorCheck( #x, __FILE__, __LINE__ ) );
#else
#define ASSERT
#define GLCall
#endif //_DEBUG

bool GLErrorCheck( const char* function, const char* file, int line );
void GLErrorClear();


// ------------------------------------------------------------------------------------------------
//   class Renderer
// ------------------------------------------------------------------------------------------------
class Renderer
{
public:

   void Clear() const;
   void Draw( const VertexArray& va, const IndexBuffer& ib, const Shader& shader ) const;


private:
};
