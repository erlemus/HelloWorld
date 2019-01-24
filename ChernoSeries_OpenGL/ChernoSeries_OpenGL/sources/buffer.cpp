#include <GL/glew.h>
#include "renderer.h"
#include "buffer.h"


// ------------------------------------------------------------------------------------------------
//   class IndexBuffer
// ------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer( const void* data, unsigned int count ) : m_count( count )
{
   ASSERT( sizeof( unsigned int ) == sizeof( GLuint ) );
   GLCall( glGenBuffers( 1, &m_iboID ) );
   GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_iboID ) );
   GLCall( glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( unsigned int ), data, GL_STATIC_DRAW ) );
}


IndexBuffer::~IndexBuffer()
{
   //GLCall( glDeleteBuffers( GL_ELEMENT_ARRAY_BUFFER, &m_iboID ) );
}


void IndexBuffer::Bind() const
{
   GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_iboID ) );
}


void IndexBuffer::Unbind() const
{
   GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
}


// ------------------------------------------------------------------------------------------------
//   class VertexBuffer
// ------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer( const void * data, unsigned int size )
{
   GLCall( glGenBuffers( 1, &m_vboID ) );
   GLCall( glBindBuffer( GL_ARRAY_BUFFER, m_vboID ) );
   GLCall( glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW ) );
}


VertexBuffer::~VertexBuffer()
{
   //GLCall( glDeleteBuffers( GL_ARRAY_BUFFER, &m_vboID ) );
}


void VertexBuffer::Bind() const
{
   GLCall( glBindBuffer( GL_ARRAY_BUFFER, m_vboID ) );
}


void VertexBuffer::Unbind() const
{
   GLCall( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
}


// ------------------------------------------------------------------------------------------------
//   class VertexBuffer
// ------------------------------------------------------------------------------------------------
VertexArray::VertexArray()
{
   GLCall( glGenVertexArrays( 1, &m_vaoID ) );
   GLCall( glBindVertexArray( m_vaoID ) );
}


VertexArray::~VertexArray()
{
   GLCall( glDeleteVertexArrays( 1, &m_vaoID ) );
}


void VertexArray::Bind() const
{
   GLCall( glBindVertexArray( m_vaoID ) );
}


void VertexArray::Unbind() const
{
   GLCall( glBindVertexArray( 0 ) );
}


void VertexArray::AddBuffer( const VertexBuffer& vb, const VertexBufferLayout& layout )
{
   Bind();
   vb.Bind();

   const std::vector< VertexBufferAttrib >& attribs = layout.GetAttribs();
   size_t num_attribs = attribs.size(), offset = 0;

   for( unsigned int i = 0; i < num_attribs; i++ )
   {
      const auto& attr = attribs[ i ];
      GLCall( glEnableVertexAttribArray( i /*attribIdx*/ ) );
      GLCall( glVertexAttribPointer( i /*attribIdx*/, attr.count, attr.type, attr.normalized, layout.GetStride(), reinterpret_cast< const void* >( offset ) ) );
      offset += attr.GetAttribSize();
   }
}


// ------------------------------------------------------------------------------------------------
//   Helpers to the vertex-related abstractions (i.e. VertexBuffer, VertexArray)
// ------------------------------------------------------------------------------------------------
size_t VertexBufferAttrib::GetTypeSize() const
{
   switch( type )
   {
      case GL_UNSIGNED_BYTE:  return sizeof( GLubyte );
      case GL_BYTE:           return sizeof( GLbyte );
      case GL_UNSIGNED_INT:   return sizeof( GLuint );
      case GL_INT:            return sizeof( GLint );
      case GL_FLOAT:          return sizeof( GLfloat );
      case GL_DOUBLE:         return sizeof( GLdouble );
      default: ASSERT( false ); return 0;
   }
}


size_t VertexBufferAttrib::GetAttribSize() const
{
   return count * GetTypeSize();
}


void VertexBufferLayout::PushHelper( VertexBufferAttrib && element )
{
   m_attribs.push_back( element );
   m_stride += static_cast< unsigned int >( element.GetAttribSize() );
}
