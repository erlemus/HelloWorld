#pragma once

#include <vector>


class IndexBuffer
{
public:
   IndexBuffer( const void* data, unsigned int count );
   virtual ~IndexBuffer();

   inline unsigned int GetCount() const { return m_count; }

   void Bind() const;
   void Unbind() const;

private:
   unsigned int m_iboID;
   unsigned int m_count;
};


class VertexBuffer
{
public:
   VertexBuffer( const void* data, unsigned int size );
   virtual ~VertexBuffer();

   void Bind() const;
   void Unbind() const;

private:
   unsigned int m_vboID;
};


struct VertexBufferAttrib
{
   unsigned int count;
   unsigned int type;
   unsigned char normalized;

   size_t GetTypeSize() const;
   size_t GetAttribSize() const;
};


class VertexBufferLayout
{
public:
   VertexBufferLayout() : m_stride( 0 ) {}

   inline const std::vector< VertexBufferAttrib >& GetAttribs() const { return m_attribs; }
   inline unsigned int GetStride() const { return m_stride; }

   template< typename T > void Push( unsigned int count ) { static_assert( false ); }
   template<> void Push< float >( unsigned int count ) { PushHelper( { count, GL_FLOAT, GL_FALSE } ); }
   template<> void Push< unsigned int >( unsigned int count ) { PushHelper( { count, GL_UNSIGNED_INT, GL_TRUE } ); }
   template<> void Push< unsigned char >( unsigned int count ) { PushHelper( { count, GL_UNSIGNED_BYTE, GL_TRUE } ); }

private:
   void PushHelper( VertexBufferAttrib&& element );

   std::vector< VertexBufferAttrib > m_attribs;
   unsigned int m_stride;
};


class VertexArray
{
public:
   VertexArray();
   virtual ~VertexArray();

   void Bind() const;
   void Unbind() const;
   void AddBuffer( const VertexBuffer& vb, const VertexBufferLayout& layout );

private:
   unsigned int m_vaoID;
};
